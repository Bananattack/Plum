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

        void initSongObject(lua_State* L)
        {
            luaL_newmetatable(L, meta<Channel>());
            // Duplicate the metatable on the stack.
            lua_pushvalue(L, -1);
            // metatable.__index = metatable
            lua_setfield(L, -2, "__index");

            // Put the members into the metatable.
            const luaL_Reg functions[] = {
				{"__gc", [](lua_State* L) { return script::wrapped<Channel>(L, 1)->gc(L); }},
                {"__index", [](lua_State* L) { return script::wrapped<Channel>(L, 1)->index(L); }},
                {"__newindex", [](lua_State* L) { return script::wrapped<Channel>(L, 1)->newindex(L); }},
                {"__tostring", [](lua_State* L) { return script::wrapped<Channel>(L, 1)->tostring(L); }},
				{"__pairs", [](lua_State* L) { return script::wrapped<Channel>(L, 1)->pairs(L); }},
                {"play", [](lua_State* L)
                {
                    auto chan = script::ptr<Channel>(L, 1);
                    chan->play();
                    return 0;
                }},
                {"stop", [](lua_State* L)
                {
                    auto chan = script::ptr<Channel>(L, 1);
                    chan->stop();
                    return 0;
                }},
                {"get_playing", [](lua_State* L)
                {
                    auto chan = script::ptr<Channel>(L, 1);
                    script::push(L, chan->isPlaying());
                    return 1;
                }},
                {"set_playing", [](lua_State* L)
                {
                    auto chan = script::ptr<Channel>(L, 1);
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
                }},
                {"get_volume", [](lua_State* L)
                {
                    auto chan = script::ptr<Channel>(L, 1);
                    script::push(L, chan->getVolume());
                    return 1;
                }},
                {"set_volume", [](lua_State* L)
                {
                    auto chan = script::ptr<Channel>(L, 1);
                    auto value = script::get<double>(L, 2);
                    chan->setVolume(value);
                    return 0;
                }},
                {nullptr, nullptr}
            };
            luaL_setfuncs(L, functions, 0);
            lua_pop(L, 1);

            // Push plum namespace.
            lua_getglobal(L, "plum");

            // plum.song = <function songNew>
            script::push(L, "Song");
            lua_pushcfunction(L, [](lua_State* L)
            {
                auto filename = script::get<const char*>(L, 1);

                Sound sound;
                script::instance(L).audio().loadSound(filename, true, sound);

                auto chan = new Channel();
                script::instance(L).audio().loadChannel(sound, *chan);

                script::push(L, chan, LUA_NOREF);
                return 1;
            });
            lua_settable(L, -3);

            // Pop plum namespace.
            lua_pop(L, 1);
        }
    }
}