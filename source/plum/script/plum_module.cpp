#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "../core/color.h"
#include "../core/input.h"
#include "../core/screen.h"
#include "../core/engine.h"
#include "../core/blending.h"
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

        int sleep(lua_State* L)
        {
            auto ms = script::get<int>(L, 1);
#ifdef _WIN32
            Sleep(ms);
#else
            usleep(ms * 1000);
#endif
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
                script::instance(L).screen().setTitle(lua_tostring(L, 1));
            }
            
            return 0;
        }

        int rgb(lua_State* L)
        {
            auto r = script::get<int>(L, 1);
            auto g = script::get<int>(L, 2);
            auto b = script::get<int>(L, 3);
            auto a = script::get<int>(L, 4, 255);
            script::push(L, int(plum::rgb(r, g, b, a)));
            return 1;
        }

        int channels(lua_State* L)
        {
            uint8_t r, g, b, a;
            Color(script::get<int>(L, 1)).channels(r, g, b, a);
            script::push(L, r);
            script::push(L, g);
            script::push(L, b);
            script::push(L, a);
            return 4;
        }

        int hsv(lua_State* L)
        {
            auto h = script::get<int>(L, 1);
            auto s = script::get<int>(L, 2);
            auto v = script::get<int>(L, 3);
            auto a = script::get<int>(L, 4, 255);
            script::push(L, int(plum::hsv(h, s, v, a)));
            return 1;
        }
    }

    namespace script
    {
        void initLibrary(lua_State* L)
        {
            const luaL_Reg functions[] = {
                {"exit", exit},
                {"sleep", sleep},
                {"refresh", refresh},
                {"setTitle", setTitle},
                {nullptr, nullptr},
            };
            luaL_newmetatable(L, "plum");
            // Duplicate the metatable on the stack.
            lua_pushvalue(L, -1);
            // metatable.__index = metatable
            lua_setfield(L, -2, "__index");
            // Put the members into the metatable.
            luaL_setfuncs(L, functions, 0);

            // Create the 'color' table.
            lua_newtable(L);
            lua_pushvalue(L, -1);
            lua_setfield(L, -3, "color");

            script::push(L, int(Color::White));
            lua_setfield(L, -2, "White");
            script::push(L, int(Color::Red));
            lua_setfield(L, -2, "Red");
            script::push(L, int(Color::Green));
            lua_setfield(L, -2, "Green");
            script::push(L, int(Color::Blue));
            lua_setfield(L, -2, "Blue");
            script::push(L, int(Color::Magenta));
            lua_setfield(L, -2, "Magenta");
            script::push(L, int(Color::Cyan));
            lua_setfield(L, -2, "Cyan");
            script::push(L, int(Color::Yellow));
            lua_setfield(L, -2, "Yellow");
            script::push(L, int(Color::Black));
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

            script::push(L, int(BlendOpaque));
            lua_setfield(L, -2, "Opaque");
            script::push(L, int(BlendMerge));
            lua_setfield(L, -2, "Merge");
            script::push(L, int(BlendMerge));
            lua_setfield(L, -2, "Alpha");
            script::push(L, int(BlendPreserve));
            lua_setfield(L, -2, "Preserve");
            script::push(L, int(BlendAdd));
            lua_setfield(L, -2, "Add");
            script::push(L, int(BlendSubtract));
            lua_setfield(L, -2, "Subtract");

            // Done with 'blend' now.
            lua_pop(L, 1);

            // Pop and store the library.
            lua_setglobal(L, "plum");

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
            initCanvasObject(L);
            initPointObject(L);
            initRectObject(L);
            initTransformObject(L);
            initImageObject(L);
            initSpriteObject(L);
            initFontObject(L);
            initTilemapObject(L);
        }
    }
}