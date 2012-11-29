
#include "../engine/engine.h"
#include "../video/color.h"
#include "../video/blending.h"
#include "script.h"

namespace plum
{
    namespace
    {
        const char* const libraryName = "plum";

        int exit(lua_State* L)
        {
            int argumentCount = lua_gettop(L);

            if(argumentCount >= 1)
            {
                const char* message = lua_tostring(L, 1);
                if(message)
                {
                    script::instance(L).engine().quit(message);
                }
                else
                {
                    script::instance(L).engine().quit("nil");
                }
            }
            else
            {
                script::instance(L).engine().quit();
            }
            
            return 0;
        }

        int refresh(lua_State* L)
        {
            auto& script = script::instance(L);
            script.engine().refresh();
            return 0;
        }

        int setTitle(lua_State* L)
        {
            int argumentCount = lua_gettop(L);

            if(argumentCount >= 1)
            {
                script::instance(L).engine().setTitle(lua_tostring(L, 1));
            }
            
            return 0;
        }

        int rgb(lua_State* L)
        {
            int r = luaL_checkint(L, 1);
            int g = luaL_checkint(L, 2);
            int b = luaL_checkint(L, 3);
            int a = script::get<int>(L, 4, 255);
            script::push(L, plum::rgba(r, g, b, a).value);
            return 1;
        }

        int channels(lua_State* L)
        {
            int color = luaL_checkint(L, 1);
            ColorChannel r, g, b, a;
            plum::getRGBA(color, r, g, b, a);
            script::push(L, r);
            script::push(L, g);
            script::push(L, b);
            script::push(L, a);
            return 4;
        }

        int hsv(lua_State* L)
        {
            int h = luaL_checkint(L, 1);
            int s = luaL_checkint(L, 2);
            int v = luaL_checkint(L, 3);
            int a = script::get<int>(L, 4, 255);
            script::push(L, (int) plum::hsv(h, s, v, a).value);
            return 1;
        }

        int hookInput(lua_State* L)
        {
            // Check that first argument is an input object.
            // If so, happily grab the pointer.
            Input** inp = (Input**) luaL_checkudata(L, 1, "plum_input");

            // Get that function on the stack!
            if(!lua_isfunction(L, 2) && !lua_isnil(L, 2))
            {
                luaL_error(L, "Invalid argument #2 to plum.hookInput. Must be a valid function, or nil.");
                return 0;
            }

            // Nuke old callback reference (if any)
            {
                std::vector<Script::InputHook>& hooks(script::instance(L).inputHooks);
                int ref = LUA_NOREF;
                for(uint32_t i = 0; i < hooks.size(); i++)
                {
                    // Okay, convert our reference number input to userdata.
                    Script::InputHook& hook = hooks[i];
                    lua_rawgeti(L, LUA_REGISTRYINDEX, hook.inputRef);
                    Input** p = (Input**) luaL_checkudata(L, -1, "plum_input");

                    // If the passed argument is the same input object,
                    // Then discard that old references.
                    if(*p == *inp)
                    {
                        hooks.erase(hooks.begin() + i);

                        luaL_unref(L, LUA_REGISTRYINDEX, hook.inputRef);
                        luaL_unref(L, LUA_REGISTRYINDEX, hook.callbackRef);
                        break;
                    }
                }
            }
            // Store new callback reference stuff.
            // Unless nil, in which case we just remove old entries.
            // argh.
            if(!lua_isnil(L, 2))
            {
                Script::InputHook hook;

                lua_pushvalue(L, 1);
                hook.inputRef = luaL_ref(L, LUA_REGISTRYINDEX);

                lua_pushvalue(L, 2);
                hook.callbackRef = luaL_ref(L, LUA_REGISTRYINDEX);

                script::instance(L).inputHooks.push_back(hook);
            }
            return 0;
        }

        int unhookAllInput(lua_State* L)
        {
            std::vector<Script::InputHook>& hooks(script::instance(L).inputHooks);
            for(uint32_t i = 0; i < hooks.size(); i++)
            {
                Script::InputHook& hook = hooks[i];
                luaL_unref(L, LUA_REGISTRYINDEX, hook.inputRef);
                luaL_unref(L, LUA_REGISTRYINDEX, hook.callbackRef);
            }
            hooks.clear();
            return 0;
        }
    }

    namespace script
    {
        void initLibrary(lua_State* L)
        {
            const luaL_Reg functions[] = {
                {"exit", exit},
                {"refresh", refresh},
                {"setTitle", setTitle},
                {"hookInput", hookInput},
                {"unhookAllInput", unhookAllInput},
                {nullptr, nullptr},
            };
            luaL_register(L, libraryName, functions);

            // Create the 'color' table.
            lua_newtable(L);
            lua_pushvalue(L, -1);
            lua_setfield(L, -3, "color");

            lua_pushinteger(L, Color::White);
            lua_setfield(L, -2, "White");
            lua_pushinteger(L, Color::Red);
            lua_setfield(L, -2, "Red");
            lua_pushinteger(L, Color::Green);
            lua_setfield(L, -2, "Green");
            lua_pushinteger(L, Color::Blue);
            lua_setfield(L, -2, "Blue");
            lua_pushinteger(L, Color::Magenta);
            lua_setfield(L, -2, "Magenta");
            lua_pushinteger(L, Color::Cyan);
            lua_setfield(L, -2, "Cyan");
            lua_pushinteger(L, Color::Yellow);
            lua_setfield(L, -2, "Yellow");
            lua_pushinteger(L, Color::Black);
            lua_setfield(L, -2, "Black");

            lua_pushcfunction(L, rgb);
            lua_setfield(L, -2, "rgb");

            lua_pushcfunction(L, hsv);
            lua_setfield(L, -2, "hsv");

            lua_pushcfunction(L, channels);
            lua_setfield(L, -2, "channels");

            // Done with 'color' now.
            lua_pop(L, 1);

            // Create the 'blend' table.
            lua_newtable(L);
            lua_pushvalue(L, -1);
            lua_setfield(L, -3, "blend");

            lua_pushinteger(L, BlendOpaque);
            lua_setfield(L, -2, "Opaque");
            lua_pushinteger(L, BlendMerge);
            lua_setfield(L, -2, "Merge");
            lua_pushinteger(L, BlendMerge);
            lua_setfield(L, -2, "Alpha");
            lua_pushinteger(L, BlendAlpha);
            lua_setfield(L, -2, "Preserve");
            lua_pushinteger(L, BlendAdd);
            lua_setfield(L, -2, "Add");
            lua_pushinteger(L, BlendSubtract);
            lua_setfield(L, -2, "Subtract");

            // Done with 'blend' now.
            lua_pop(L, 1);

            // Pop the library.
            lua_pop(L, 1);

            // Load all the submodule and object definitions contained within Plum.
            initVideoModule(L);
            initTimerModule(L);

            initInputObject(L);
            initKeyboardModule(L);
            initMouseModule(L);

            initFontObject(L);
            initSoundObject(L);
            initSongObject(L);
            initFileObject(L);
            initFileObject(L);
            initDirectoryObject(L);
            initCanvasObject(L);
            initPointObject(L);
            initRectObject(L);
            initTransformObject(L);
            initImageObject(L);
            initSpritesheetObject(L);
            initFontObject(L);
            initTilemapObject(L);
        }
    }
}