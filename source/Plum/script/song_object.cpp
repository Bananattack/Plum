#include "../audio/audio.h"
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
            chan->setLooped(true);

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

        int getplaying(lua_State* L)
        {
            auto chan = script::ptr<Self>(L, 1);
            script::push(L, chan->isPlaying());
            return 1;
        }

        int setplaying(lua_State* L)
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

        int getvolume(lua_State* L)
        {
            auto chan = script::ptr<Self>(L, 1);
            script::push(L, chan->getVolume());
            return 1;
        }

        int setvolume(lua_State* L)
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
                {"getplaying", getplaying},
                {"setplaying", setplaying},
                {"getvolume", getvolume},
                {"setvolume", setvolume},
                {nullptr, nullptr}
            };
            luaL_register(L, nullptr, functions);
            lua_pop(L, 1);

            // Push plum namespace.
            lua_getglobal(L, "plum");

            // plum.song = <function songNew>
            lua_pushstring(L, "Song");
            lua_pushcfunction(L, create);
            lua_settable(L, -3);

            // Pop plum namespace.
            lua_pop(L, 1);
        }
    }
}