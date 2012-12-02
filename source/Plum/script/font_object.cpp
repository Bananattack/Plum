
#include "../core/font.h"
#include "script.h"

namespace plum
{
    namespace script
    {
        template<> const char* meta<Font>()
        {
            return "plum.Font";
        }
    }

    namespace
    {
        typedef Font Self;
        int create(lua_State* L)
        {
            if(lua_isstring(L, 1))
            {
                auto filename = script::get<const char*>(L, 1);
                script::push(L, new Self(filename), LUA_NOREF);
                return 1;
            }
            luaL_error(L, "Attempt to call plum.Font constructor with invalid argument types.\r\nMust be (string filename).");
            return 0;
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

        int print(lua_State* L)
        {
            auto font = script::ptr<Self>(L, 1);
            auto x = script::get<int>(L, 2);
            auto y = script::get<int>(L, 3);
            auto message = script::get<const char*>(L, 4);
            BlendMode mode = (BlendMode) script::get<int>(L, 5, BlendAlpha);
            Color tint = script::get<int>(L, 6, Color::White);

            font->print(x, y, message, mode, tint);
            return 0;
        }

        int printRight(lua_State* L)
        {
            auto font = script::ptr<Self>(L, 1);
            auto x = script::get<int>(L, 2);
            auto y = script::get<int>(L, 3);
            auto message = script::get<const char*>(L, 4);
            BlendMode mode = (BlendMode) script::get<int>(L, 5, BlendAlpha);
            Color tint = script::get<int>(L, 6, Color::White);

            font->printRight(x, y, message, mode, tint);
            return 0;
        }

        int printCenter(lua_State* L)
        {
            auto font = script::ptr<Self>(L, 1);
            auto x = script::get<int>(L, 2);
            auto y = script::get<int>(L, 3);
            auto message = script::get<const char*>(L, 4);
            BlendMode mode = (BlendMode) script::get<int>(L, 5, BlendAlpha);
            Color tint = script::get<int>(L, 6, Color::White);

            font->printCenter(x, y, message, mode, tint);
            return 0;
        }

        int enableVariableWidth(lua_State* L)
        {
            auto font = script::ptr<Self>(L, 1);
            font->enableVariableWidth();
            return 0;
        }

        int lineWidth(lua_State* L)
        {
            auto font = script::ptr<Self>(L, 1);
            auto message = script::get<const char*>(L, 2);
            auto lineIndex = script::get<int>(L, 3);
            if(message)
            {
                script::push(L, font->lineWidth(message, lineIndex));
            }
            else
            {
                script::push(L, 0);
            }
            return 1;
        }

        int textWidth(lua_State* L)
        {
            auto font = script::ptr<Self>(L, 1);
            auto message = script::get<const char*>(L, 2);
            script::push(L, font->textWidth(message));
            return 1;
        }

        int textHeight(lua_State* L)
        {
            auto font = script::ptr<Self>(L, 1);
            auto message = script::get<const char*>(L, 2);
            script::push(L, font->textHeight(message));
            return 1;
        }

        int wrapText(lua_State* L)
        {
            auto font = script::ptr<Self>(L, 1);
            auto message = script::get<const char*>(L, 2);
            int lineLength = script::get<int>(L, 3);
            script::push(L, font->wrapText(message, lineLength).c_str());
            return 1;
        }

        int getwidth(lua_State* L)
        {
            auto font = script::ptr<Self>(L, 1);
            script::push(L, font->width);
            return 1;
        }

        int getheight(lua_State* L)
        {
            auto font = script::ptr<Self>(L, 1);
            lua_pushinteger(L, font->height);
            return 1;
        }
    }

    namespace script
    {
        void initFontObject(lua_State* L)
        {
            luaL_newmetatable(L, meta<Self>());
            // Duplicate the metatable on the stack.
            lua_pushvalue(L, -1);
            // metatable.__index = metatable
            lua_setfield(L, -2, "__index");

            // Put the members into the metatable.
            const luaL_Reg functions[] = {
                {"__gc", gc},
                {"__index", index},
                {"__newindex", newindex},
                {"__tostring", tostring},
                {"enableVariableWidth", enableVariableWidth},
                {"textWidth", textWidth},
                {"lineWidth", lineWidth},
                {"print", print},
                {"printRight", printRight},
                {"printCenter", printCenter},
                {"wrapText", wrapText},
                {"getwidth", getwidth},
                {"getheight", getheight},
                {nullptr, nullptr},
            };
            luaL_register(L, nullptr, functions);

            lua_pop(L, 1);

            // Push plum namespace.
            lua_getglobal(L, "plum");

            // plum[classname] = create
            lua_pushstring(L, "Font");
            lua_pushcfunction(L, create);
            lua_settable(L, -3);

            // Pop plum namespace.
            lua_pop(L, 1);

        }
    }
}
