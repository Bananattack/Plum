#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "../core/file.h"
#include "../core/color.h"
#include "../core/input.h"
#include "../core/timer.h"
#include "../core/screen.h"
#include "../core/engine.h"
#include "../core/blending.h"
#include "script.h"

namespace plum
{
    namespace script
    {
        void initLibrary(lua_State* L)
        {
            const luaL_Reg functions[] = {
                {"exit", [](lua_State* L)
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
                }},
                {"sleep", [](lua_State* L)
                {
                    auto ms = script::get<int>(L, 1);
#ifdef _WIN32
                    Sleep(ms);
#else
                    usleep(ms * 1000);
#endif
                    return 0;
                }},
                {"refresh", [](lua_State* L)
                {
                    auto& script = script::instance(L);

                    if(lua_isfunction(L, 1))
                    {
                        // Create ref to function.
                        lua_pushvalue(L, 1);
                        int ref = luaL_ref(L, LUA_REGISTRYINDEX);

                        // Create hook that calls referenced function.
                        bool done = false;
                        auto hook = script.engine().addUpdateHook([L, ref, &done]()
                        {
                            lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
                            lua_call(L, 0, 1);
                            if(lua_toboolean(L, -1))
                            {
                                done = true;
                            }
                            lua_pop(L, 1);
                        });

                        // Call refresh until done.
                        while(!done)
                        {
                            script.engine().refresh();
                        }

                        luaL_unref(L, LUA_REGISTRYINDEX, ref);
                    }
                    else
                    {
                        script.engine().refresh();
                    }
                    return 0;
                }},
                {nullptr, nullptr},
            };
            luaL_newmetatable(L, "plum");
            // Duplicate the metatable on the stack.
            lua_pushvalue(L, -1);
            // metatable.__index = metatable
            lua_setfield(L, -2, "__index");
            // Put the members into the metatable.
            luaL_setfuncs(L, functions, 0);

            // Create the 'arg' table.
            lua_newtable(L);
            lua_pushvalue(L, -1);
            lua_setfield(L, -3, "arg");
            {
                auto& script = script::instance(L);
                for(int i = 0; i < script.argc(); i++)
                {
                    script::push(L, script.argv()[i]);
                    lua_rawseti(L, -2, i);
                }
            }

            // Done with 'arg' now.
            lua_pop(L, 1);

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
            lua_pushcfunction(L, [](lua_State* L)
            {
                auto r = script::get<int>(L, 1);
                auto g = script::get<int>(L, 2);
                auto b = script::get<int>(L, 3);
                auto a = script::get<int>(L, 4, 255);
                script::push(L, int(plum::rgb(r, g, b, a)));
                return 1;
            });
            lua_setfield(L, -2, "rgb");
            lua_pushcfunction(L, [](lua_State* L)
            {
                auto h = script::get<int>(L, 1);
                auto s = script::get<int>(L, 2);
                auto v = script::get<int>(L, 3);
                auto a = script::get<int>(L, 4, 255);
                script::push(L, int(plum::hsv(h, s, v, a)));
                return 1;
            });
            lua_setfield(L, -2, "hsv");
            lua_pushcfunction(L, ([](lua_State* L)
            {
                uint8_t r, g, b, a;
                Color(script::get<int>(L, 1)).channels(r, g, b, a);
                script::push(L, r);
                script::push(L, g);
                script::push(L, b);
                script::push(L, a);
                return 4;
            }));
            lua_setfield(L, -2, "channels");
            lua_pop(L, 1);

            // Create the 'blend' table.
            lua_newtable(L);
            lua_pushvalue(L, -1);
            lua_setfield(L, -3, "blend");
            script::push(L, int(BlendMode::Opaque));
            lua_setfield(L, -2, "Opaque");
            script::push(L, int(BlendMode::Merge));
            lua_setfield(L, -2, "Merge");
            script::push(L, int(BlendMode::Merge));
            lua_setfield(L, -2, "Alpha");
            script::push(L, int(BlendMode::Preserve));
            lua_setfield(L, -2, "Preserve");
            script::push(L, int(BlendMode::Add));
            lua_setfield(L, -2, "Add");
            script::push(L, int(BlendMode::Subtract));
            lua_setfield(L, -2, "Subtract");
            lua_pop(L, 1);

            // Create the 'open' table.
            lua_newtable(L);
            lua_pushvalue(L, -1);
            lua_setfield(L, -3, "open");
            script::push(L, int(FileOpenMode::Read));
            lua_setfield(L, -2, "Read");
            script::push(L, int(FileOpenMode::Write));
            lua_setfield(L, -2, "Write");
            script::push(L, int(FileOpenMode::Append));
            lua_setfield(L, -2, "Append");
            lua_pop(L, 1);

            // Create the 'seek' table.
            lua_newtable(L);
            lua_pushvalue(L, -1);
            lua_setfield(L, -3, "seek");
            script::push(L, int(FileSeekMode::Start));
            lua_setfield(L, -2, "Start");
            script::push(L, int(FileSeekMode::Current));
            lua_setfield(L, -2, "Current");
            script::push(L, int(FileSeekMode::End));
            lua_setfield(L, -2, "End");
            lua_pop(L, 1);

            // Create the 'speed' table.
            lua_newtable(L);
            lua_pushvalue(L, -1);
            lua_setfield(L, -3, "speed");
            script::push(L, int(TimerSpeed::Normal));
            lua_setfield(L, -2, "Normal");
            script::push(L, int(TimerSpeed::Fast));
            lua_setfield(L, -2, "Fast");
            script::push(L, int(TimerSpeed::Slow));
            lua_setfield(L, -2, "Slow");
            lua_pop(L, 1);

            // Pop and store the library.
            lua_setglobal(L, "plum");

            // Load all the submodule and object definitions contained within Plum.
            initTimerModule(L);

            initCanvasObject(L);
            initInputObject(L);
            initAxisObject(L);
            initMouseObject(L);
            initFileObject(L);
            initImageObject(L);
            initSheetObject(L);
            initSongObject(L);
            initSoundObject(L);
            initScreenObject(L);
            initSpriteObject(L);
            initTilemapObject(L);
            initJoystickObject(L);
            initTransformObject(L);
        }
    }
}