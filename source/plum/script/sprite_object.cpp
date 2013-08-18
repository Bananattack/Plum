#include "../core/sprite.h"
#include "script.h"

namespace plum
{
    namespace script
    {
        template<> const char* meta<Sprite>()
        {
            return "plum.Sprite";
        }

        void initSpriteObject(lua_State* L)
        {
            luaL_newmetatable(L, meta<Sprite>());
            // Duplicate the metatable on the stack.
            lua_pushvalue(L, -1);
            // metatable.__index = metatable
            lua_setfield(L, -2, "__index");

            // Put the members into the metatable.
            const luaL_Reg functions[] = {
				{"__gc", [](lua_State* L) { return script::wrapped<Sprite>(L, 1)->gc(L); }},
                {"__index", [](lua_State* L) { return script::wrapped<Sprite>(L, 1)->index(L); }},
                {"__newindex", [](lua_State* L) { return script::wrapped<Sprite>(L, 1)->newindex(L); }},
                {"__tostring", [](lua_State* L) { return script::wrapped<Sprite>(L, 1)->tostring(L); }},
				{"__pairs", [](lua_State* L) { return script::wrapped<Sprite>(L, 1)->pairs(L); }},
                {"clear", [](lua_State* L)
                {
                    auto sprite = script::ptr<Sprite>(L, 1);
                    sprite->setSize(0);
                    return 0;
                }},
                {"add", [](lua_State* L)
                {
                    auto sprite = script::ptr<Sprite>(L, 1);
                    auto x = script::get<int>(L, 2);
                    auto y = script::get<int>(L, 3);
                    auto frame = script::get<int>(L, 4);
                    auto transform = script::is<nullptr_t>(L, 5) ? nullptr : script::ptr<Transform>(L, 5);

                    if(transform)
                    {
                        sprite->add(x, y, frame, *transform);
                    }
                    else
                    {
                        sprite->add(x, y, frame, Transform());
                    }
                    return 0;
                }},
                {"update", [](lua_State* L)
                {
                    auto sprite = script::ptr<Sprite>(L, 1);
                    auto index = script::get<int>(L, 2);
                    auto x = script::get<int>(L, 3);
                    auto y = script::get<int>(L, 4);
                    auto frame = script::get<int>(L, 5);
                    auto transform = script::is<nullptr_t>(L, 6) ? nullptr : script::ptr<Transform>(L, 6);

                    if(transform)
                    {
                        sprite->update(size_t(index), x, y, frame, *transform);
                    }
                    else
                    {
                        sprite->update(size_t(index), x, y, frame, Transform());
                    }
                    return 0;
                }},
                {"draw", [](lua_State* L)
                {
                    auto sprite = script::ptr<Sprite>(L, 1);
                    auto image = script::ptr<Image>(L, 2);
                    auto sheet = script::ptr<Sheet>(L, 3);
                    auto x = script::get<int>(L, 4);
                    auto y = script::get<int>(L, 5);
                    auto screen = script::ptr<Screen>(L, 6);

                    sprite->draw(*image, *sheet, x, y, *screen);
                    return 0;
                }},
                {"get_length", [](lua_State* L)
                {
                    auto sprite = script::ptr<Sprite>(L, 1);

                    script::push(L, int(sprite->getSize()));
                    return 1;
                }},
                {"get_capacity", [](lua_State* L)
                {
                    auto sprite = script::ptr<Sprite>(L, 1);

                    script::push(L, int(sprite->getCapacity()));
                    return 1;
                }},
                {"set_length", [](lua_State* L)
                {
                    auto sprite = script::ptr<Sprite>(L, 1);
                    auto value = script::get<int>(L, 2);

                    sprite->setSize(size_t(value));
                    return 0;
                }},
                {"set_capacity", [](lua_State* L)
                {
                    auto sprite = script::ptr<Sprite>(L, 1);
                    auto value = script::get<int>(L, 2);

                    sprite->setCapacity(size_t(value));
                    return 0;
                }},
                {nullptr, nullptr}
            };
            luaL_setfuncs(L, functions, 0);
            lua_pop(L, 1);

            // Push plum namespace.
            lua_getglobal(L, "plum");

            // plum.Sprite = <function create>
            script::push(L, "Sprite");
            lua_pushcfunction(L, [](lua_State* L)
            {
                auto sprite = new Sprite();
                script::push(L, sprite, LUA_NOREF);
                return 1;
            });
            lua_settable(L, -3);

            // Pop plum namespace.
            lua_pop(L, 1);
        }
    }
}