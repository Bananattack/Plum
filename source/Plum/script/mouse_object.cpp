
#include "../core/input.h"
#include "script.h"

namespace plum
{
    namespace
    {
        int mouseGetField(lua_State* L)
        {
            std::string fieldName(script::get<const char*>(L, 2));
            if(luaL_getmetafield(L, 1, std::string("get_" + fieldName).c_str()))
            {
                lua_pushvalue(L, 1);
                lua_call(L, 1, 1);
                return 1;
            }
            return luaL_getmetafield(L, 1, fieldName.c_str());
        }

        int mouseSetField(lua_State* L)
        {
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
                luaL_error(L, "Attempt to modify readonly field '%s' on plum_mouse.", fieldName.c_str());
                lua_pop(L, 1);
                return 0;
            }
            luaL_error(L, "Attempt to modify unknown field '%s' on plum_mouse.", fieldName.c_str());
            return 0;
        }

        int mouseToString(lua_State* L)
        {
            script::push(L, "(plum.mouse singleton)");
            return 1;
        }

        int mouseGetX(lua_State* L)
        {
            script::push(L, script::instance(L).mouse().getX());
            return 1;
        }

        int mouseGetY(lua_State* L)
        {
            script::push(L, script::instance(L).mouse().getY());
            return 1;
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
                lua_pushlightuserdata(L, &script::instance(L).mouse().getLeft());
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
                lua_pushlightuserdata(L, &script::instance(L).mouse().getMiddle());
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
                lua_pushlightuserdata(L, &script::instance(L).mouse().getRight());
                lua_call(L, 1, 1);
                return 1;
            }

            return 0;
        }

        int mouseGetWheelUp(lua_State* L)
        {
            lua_getglobal(L, "plum");
            lua_getfield(L, -1, "_Input");
            lua_CFunction inputConstructor = lua_tocfunction(L, -1);
            lua_pop(L, 2);

            if(inputConstructor)
            {
                lua_pushcfunction(L, inputConstructor);
                lua_pushlightuserdata(L, &script::instance(L).mouse().getWheelUp());
                lua_call(L, 1, 1);
                return 1;
            }

            return 0;
        }

        int mouseGetWheelDown(lua_State* L)
        {
            lua_getglobal(L, "plum");
            lua_getfield(L, -1, "_Input");
            lua_CFunction inputConstructor = lua_tocfunction(L, -1);
            lua_pop(L, 2);

            if(inputConstructor)
            {
                lua_pushcfunction(L, inputConstructor);
                lua_pushlightuserdata(L, &script::instance(L).mouse().getWheelDown());
                lua_call(L, 1, 1);
                return 1;
            }

            return 0;
        }

        const luaL_Reg functions[] = {
            { "__index", mouseGetField },
            { "__newindex",    mouseSetField },
            { "__tostring",    mouseToString },
            { "get_x", mouseGetX },
            { "get_y", mouseGetY },
            { "get_left", mouseGetLeft },
            { "get_middle", mouseGetMiddle },
            { "get_right", mouseGetRight },
            { "get_wheelUp", mouseGetWheelUp },
            { "get_wheelDown", mouseGetWheelUp },
            { nullptr, nullptr }
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
            luaL_setfuncs(L, functions, 0);
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