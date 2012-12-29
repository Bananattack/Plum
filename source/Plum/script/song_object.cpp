#include "../core/audio.h"
#include "script.h"

namespace plum
{
    namespace script
    {
        template<> const char* meta<Channel>()
        {
            return "plum.Song";
        }
    }

    namespace
    {
        typedef Channel Self;

        int create(lua_State* L)
        {
            auto filename = script::get<const char*>(L, 1);

            Sound sound;
            script::instance(L).audio().loadSound(filename, true, sound);

            auto chan = new Channel();
            script::instance(L).audio().loadChannel(sound, *chan);

            script::push(L, chan, LUA_NOREF);
            return 1;
        }

        int gc(lua_State* L)
        {
            return script::wrapped<Self>(L, 1)->gc(L);
        }

        int index(lua_State* L)
        {
            return script::wrapped<Self>(L, 1)->index(L);
        }

        int newindex(lua_State* L)
        {
            return script::wrapped<Self>(L, 1)->newindex(L);
        }

        int tostring(lua_State* L)
        {
            return script::wrapped<Self>(L, 1)->tostring(L);
        }

        int play(lua_State* L)
        {
            auto chan = script::ptr<Self>(L, 1);
            chan->play();
            return 0;
        }

        int stop(lua_State* L)
        {
            auto chan = script::ptr<Self>(L, 1);
            chan->stop();
            return 0;
        }

        int get_playing(lua_State* L)
        {
            auto chan = script::ptr<Self>(L, 1);
            script::push(L, chan->isPlaying());
            return 1;
        }

        int set_playing(lua_State* L)
        {
            auto chan = script::ptr<Self>(L, 1);
            auto playing = script::get<bool>(L, 2);
            if(!playing && chan->isPlaying())
            {
                chan->stop();
            }
            else if(playing && !chan->isPlaying())
            {
                chan->play();
            }

            return 0;
        }

        int get_volume(lua_State* L)
        {
            auto chan = script::ptr<Self>(L, 1);
            script::push(L, chan->getVolume());
            return 1;
        }

        int set_volume(lua_State* L)
        {
            auto chan = script::ptr<Self>(L, 1);
            auto value = script::get<double>(L, 2);
            chan->setVolume(value);
            return 0;
        }
    }

    namespace script
    {
        void initSongObject(lua_State* L)
        {
            luaL_newmetatable(L, meta<Self>());
            // Duplicate the metatable on the stack.
            lua_pushvalue(L, -1);
            // metatable.__index = metatable
            lua_setfield(L, -2, "__index");

            // Put the members into the metatable.
            const luaL_Reg functions[] = {
                {"__index", index},
                {"__newindex", newindex},
                {"__tostring", tostring},
                {"__gc", gc},
                {"play", play},
                {"stop", stop},
                {"get_playing", get_playing},
                {"set_playing", set_playing},
                {"get_volume", get_volume},
                {"set_volume", set_volume},
                {nullptr, nullptr}
            };
            luaL_setfuncs(L, functions, 0);
            lua_pop(L, 1);

            // Push plum namespace.
            lua_getglobal(L, "plum");

            // plum.song = <function songNew>
            script::push(L, "Song");
            lua_pushcfunction(L, create);
            lua_settable(L, -3);

            // Pop plum namespace.
            lua_pop(L, 1);
        }
    }
}