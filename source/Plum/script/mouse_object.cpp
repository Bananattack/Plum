
#include "../core/engine.h"
#include "script.h"

namespace plum
{
    namespace
    {
        int mouseGetField(lua_State* L)
        {
            std::string fieldName = luaL_checkstring(L, 2);
            if(luaL_getmetafield(L, 1, std::string("get" + fieldName).c_str()))
            {
                lua_pushvalue(L, 1);
                lua_call(L, 1, 1);
                return 1;
            }
            return luaL_getmetafield(L, 1, fieldName.c_str());
        }

        int mouseSetField(lua_State* L)
        {
            std::string fieldName = luaL_checkstring(L, 2);
            /* L, 3 is the value to set. */
            if(luaL_getmetafield(L, 1, std::string("set" + fieldName).c_str()))
            {
                lua_pushvalue(L, 1);
                lua_pushvalue(L, 3);
                lua_call(L, 2, 0);
                return 0;
            }
            if(luaL_getmetafield(L, 1, std::string("get" + fieldName).c_str()))
            {
                luaL_error(L, "Attempt to modify readonly field '%s' on plum_mouse.", fieldName.c_str());
                lua_pop(L, 1);
                return 0;
            }
            luaL_error(L, "Attempt to modify unknown field '%s' on plum_mouse.", fieldName.c_str());
            return 0;
        }

        int mouseToString(lua_State* L)
        {
            lua_pushstring(L, "(plum.mouse singleton)");
            return 1;
        }

        int mouseGetX(lua_State* L)
        {
            lua_pushnumber(L, script::instance(L).engine().mouse.x);
            return 1;
        }

        int mouseGetY(lua_State* L)
        {
            lua_pushnumber(L, script::instance(L).engine().mouse.y);
            return 1;
        }

        int mouseGetWheelPosition(lua_State* L)
        {
            lua_pushinteger(L, script::instance(L).engine().mouse.wheelPosition);
            return 1;
        }

        int mouseSetWheelPosition(lua_State* L)
        {
            int value = luaL_checkint(L, 2);
            script::instance(L).engine().mouse.wheelPosition = value;
            return 0;
        }

        int mouseGetLeft(lua_State* L)
        {
            lua_getglobal(L, "plum");
            lua_getfield(L, -1, "_Input");
            lua_CFunction inputConstructor = lua_tocfunction(L, -1);
            lua_pop(L, 2);

            if(inputConstructor)
            {
                lua_pushcfunction(L, inputConstructor);
                lua_pushlightuserdata(L, &(script::instance(L).engine().mouse.button[MOUSE_LEFT]));
                lua_call(L, 1, 1);
                return 1;
            }

            return 0;
        }

        int mouseGetMiddle(lua_State* L)
        {
            lua_getglobal(L, "plum");
            lua_getfield(L, -1, "_Input");
            lua_CFunction inputConstructor = lua_tocfunction(L, -1);
            lua_pop(L, 2);

            if(inputConstructor)
            {
                lua_pushcfunction(L, inputConstructor);
                lua_pushlightuserdata(L, &(script::instance(L).engine().mouse.button[MOUSE_MIDDLE]));
                lua_call(L, 1, 1);
                return 1;
            }

            return 0;
        }

        int mouseGetRight(lua_State* L)
        {
            lua_getglobal(L, "plum");
            lua_getfield(L, -1, "_Input");
            lua_CFunction inputConstructor = lua_tocfunction(L, -1);
            lua_pop(L, 2);

            if(inputConstructor)
            {
                lua_pushcfunction(L, inputConstructor);
                lua_pushlightuserdata(L, &(script::instance(L).engine().mouse.button[MOUSE_RIGHT]));
                lua_call(L, 1, 1);
                return 1;
            }

            return 0;
        }

        int mouseGetHide(lua_State* L)
        {
            lua_pushboolean(L, SDL_ShowCursor(SDL_QUERY) == SDL_DISABLE);
            return 1;
        }

        int mouseSetHide(lua_State* L)
        {
            SDL_ShowCursor((lua_toboolean(L, 2) != 0) ? SDL_DISABLE : SDL_ENABLE);
            return 0;
        }

        const luaL_Reg mouseMembers[] = {
            { "__index", mouseGetField },
            { "__newindex",    mouseSetField },
            { "__tostring",    mouseToString },
            { "getx", mouseGetX },
            { "gety", mouseGetY },
            { "getwheelPosition", mouseGetWheelPosition },
            { "setwheelPosition", mouseSetWheelPosition },
            { "getleft", mouseGetLeft },
            { "getmiddle", mouseGetMiddle },
            { "getright", mouseGetRight },
            { "gethide", mouseGetHide },
            { "sethide", mouseSetHide },
            { NULL, NULL }
        };
    }

    namespace script
    {
        void initMouseModule(lua_State* L)
        {
            luaL_newmetatable(L, "plum_mouse");
            // Duplicate the metatable on the stack.
            lua_pushvalue(L, -1);
            // metatable.__index = metatable
            lua_setfield(L, -2, "__index");
            // Put the members into the metatable.
            luaL_register(L, NULL, mouseMembers);
            lua_pop(L, 1);

            // Push plum namespace.
            lua_getglobal(L, "plum");

            // Create mouse
            lua_newtable(L);
            lua_pushvalue(L, -1);
            lua_setfield(L, -3, "mouse");

            luaL_getmetatable(L, "plum_mouse");
            lua_setmetatable(L, -2);

            // Pop mouse.
            lua_pop(L, 1);

            // Pop plum namespace.
            lua_pop(L, 1);
        }
    }
}