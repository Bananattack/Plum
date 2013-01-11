
#include "../core/video.h"
#include "script.h"

namespace plum
{
    namespace
    {
        const char* Meta = "plum.screen";
        int index(lua_State* L)
        {
            luaL_checkudata(L, 1, Meta);
            std::string fieldName(script::get<const char*>(L, 2));
            if(luaL_getmetafield(L, 1, std::string("get_" + fieldName).c_str()))
            {
                lua_pushvalue(L, 1);
                lua_call(L, 1, 1);
                return 1;
            }
            return luaL_getmetafield(L, 1, fieldName.c_str());
        }

        int newindex(lua_State* L)
        {
            luaL_checkudata(L, 1, Meta);
            std::string fieldName(script::get<const char*>(L, 2));
            /* L, 3 is the value to set. */
            if(luaL_getmetafield(L, 1, std::string("set_" + fieldName).c_str()))
            {
                lua_pushvalue(L, 1);
                lua_pushvalue(L, 3);
                lua_call(L, 2, 0);
                return 0;
            }
            if(luaL_getmetafield(L, 1, std::string("get_" + fieldName).c_str()))
            {
                luaL_error(L, "Attempt to modify readonly field '%s' on plum_video.", fieldName.c_str());
                lua_pop(L, 1);
                return 0;
            }
            luaL_error(L, "Attempt to modify unknown field '%s' on plum_video.", fieldName.c_str());
            return 0;
        }


        int tostring(lua_State* L)
        {
            luaL_checkudata(L, 1, Meta);
            script::push(L, "(plum.Video singleton)");
            return 1;
        }

        int clear(lua_State* L)
        {
            luaL_checkudata(L, 1, Meta);
            Color color = script::get<int>(L, 2);

            script::instance(L).screen().clear(color);
            return 0;
        }

        int setPixel(lua_State* L)
        {
            luaL_checkudata(L, 1, Meta);
            int x = script::get<int>(L, 2);
            int y = script::get<int>(L, 3);
            Color color = script::get<int>(L, 4);
            BlendMode mode = (BlendMode) script::get<int>(L, 5, BlendPreserve);

            script::instance(L).screen().setPixel(x, y, color, mode);
            return 0;
        }

        int rect(lua_State* L)
        {
            luaL_checkudata(L, 1, Meta);
            int x = script::get<int>(L, 2);
            int y = script::get<int>(L, 3);
            int x2 = script::get<int>(L, 4);
            int y2 = script::get<int>(L, 5);
            Color color = script::get<int>(L, 6);
            BlendMode mode = (BlendMode) script::get<int>(L, 7, BlendPreserve);

            script::instance(L).screen().rect(x, y, x2, y2, color, mode);
            return 0;
        }

        int solidRect(lua_State* L)
        {
            luaL_checkudata(L, 1, Meta);
            int x = script::get<int>(L, 2);
            int y = script::get<int>(L, 3);
            int x2 = script::get<int>(L, 4);
            int y2 = script::get<int>(L, 5);
            Color color = script::get<int>(L, 6);
            BlendMode mode = (BlendMode) script::get<int>(L, 7, BlendPreserve);

            script::instance(L).screen().solidRect(x, y, x2, y2, color, mode);
            return 0;
        }

        int line(lua_State* L)
        {
            luaL_checkudata(L, 1, Meta);
            int x = script::get<int>(L, 2);
            int y = script::get<int>(L, 3);
            int x2 = script::get<int>(L, 4);
            int y2 = script::get<int>(L, 5);
            Color color = script::get<int>(L, 6);
            BlendMode mode = (BlendMode) script::get<int>(L, 7, BlendPreserve);

            script::instance(L).screen().line(x, y, x2, y2, color, mode);
            return 0;
        }

        int horizontalGradientRect(lua_State* L)
        {
            luaL_checkudata(L, 1, Meta);
            int x = script::get<int>(L, 2);
            int y = script::get<int>(L, 3);
            int x2 = script::get<int>(L, 4);
            int y2 = script::get<int>(L, 5);
            Color color = script::get<int>(L, 6);
            Color color2 = script::get<int>(L, 7);
            BlendMode mode = (BlendMode) script::get<int>(L, 8, BlendPreserve);

            script::instance(L).screen().horizontalGradientRect(x, y, x2, y2, color, color2, mode);
            return 0;
        }

        int verticalGradientRect(lua_State* L)
        {
            luaL_checkudata(L, 1, Meta);
            int x = script::get<int>(L, 2);
            int y = script::get<int>(L, 3);
            int x2 = script::get<int>(L, 4);
            int y2 = script::get<int>(L, 5);
            Color color = script::get<int>(L, 6);
            Color color2 = script::get<int>(L, 7);
            BlendMode mode = (BlendMode) script::get<int>(L, 8, BlendPreserve);

            script::instance(L).screen().verticalGradientRect(x, y, x2, y2, color, color2, mode);
            return 0;
        }


        int circle(lua_State* L)
        {
            luaL_checkudata(L, 1, Meta);
            int x = script::get<int>(L, 2);
            int y = script::get<int>(L, 3);
            int xrad = script::get<int>(L, 4);
            int yrad = script::get<int>(L, 5);
            Color color = script::get<int>(L, 6);
            BlendMode mode = (BlendMode) script::get<int>(L, 7, BlendPreserve);

            script::instance(L).screen().circle(x, y, xrad, yrad, color, mode);
            return 0;
        }

        int solidCircle(lua_State* L)
        {
            luaL_checkudata(L, 1, Meta);
            int x = script::get<int>(L, 2);
            int y = script::get<int>(L, 3);
            int xrad = script::get<int>(L, 4);
            int yrad = script::get<int>(L, 5);
            Color color = script::get<int>(L, 6);
            BlendMode mode = (BlendMode) script::get<int>(L, 7, BlendPreserve);

            script::instance(L).screen().solidCircle(x, y, xrad, yrad, color, mode);
            return 0;
        }

        int get_width(lua_State* L)
        {
            luaL_checkudata(L, 1, Meta);
            script::push(L, script::instance(L).screen().getWidth());
            return 1;
        }

        int get_height(lua_State* L)
        {
            luaL_checkudata(L, 1, Meta);
            script::push(L, script::instance(L).screen().getHeight());
            return 1;
        }

        int get_opacity(lua_State* L)
        {
            luaL_checkudata(L, 1, Meta);
            script::push(L, getOpacity());
            return 1;
        }

        int set_opacity(lua_State* L)
        {
            luaL_checkudata(L, 1, Meta);
            int v = script::get<int>(L, 2);
            setOpacity(v);
            return 0;
        }

        const luaL_Reg functions[] = {
            {"__index", index},
            {"__newindex", newindex},
            {"__tostring", tostring},
            {"clear", clear},
            {"setPixel", setPixel},
            {"line", line},
            {"rect", rect},
            {"solidRect", solidRect},
            {"circle", circle},
            {"solidCircle", solidCircle},
            {"horizontalGradientRect", horizontalGradientRect},
            {"verticalGradientRect", verticalGradientRect},
            {"get_width", get_width},
            {"get_height", get_height},
            {"get_opacity", get_opacity},
            {"set_opacity", set_opacity},
            {nullptr, nullptr},
        };
    }

    namespace script
    {
        void initVideoModule(lua_State* L)
        {
            // Load timer metatable
            luaL_newmetatable(L, Meta);
            // Duplicate the metatable on the stack.
            lua_pushvalue(L, -1);
            // metatable.__index = metatable
            lua_setfield(L, -2, "__index");
            // Put the members into the metatable.
            luaL_setfuncs(L, functions, 0);
            lua_pop(L, 1);

            // Push plum namespace.
            lua_getglobal(L, "plum");

            // Create video
            lua_newuserdata(L, 0); // empty userdata (change this if it causes problems).
            lua_pushvalue(L, -1);
            lua_setfield(L, -3, "screen");

            luaL_getmetatable(L, Meta);
            lua_setmetatable(L, -2);

            // Pop video.
            lua_pop(L, 1);

            // Pop plum namespace.
            lua_pop(L, 1);
        }
    }
}