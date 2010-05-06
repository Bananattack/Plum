#include "../plum.h"

namespace Plum
{
	namespace ScriptLibrary
	{
		namespace PointObject
		{
			SCRIPT_OBJ_GETTER(index, Wrapper<Point>*, libraryName)
			SCRIPT_OBJ_SETTER(newindex, Wrapper<Point>*, libraryName)

			int create(lua_State* L)
			{
				double x = luaL_checknumber(L, 1);
				double y = luaL_checknumber(L, 2);
				PLUM_PUSH_DATA(L, Point, new Point(x, y), LUA_NOREF);
				return 1;
			}

			int gc(lua_State* L)
			{
				Wrapper<Point>* p = PLUM_CHECK_DATA(L, 1, Point);

				// Only delete if it doesn't belong to a parent of some sort.
				if(p->parentRef != LUA_NOREF)
				{
					delete p->data;
				}
				else
				{
					luaL_unref(L, LUA_REGISTRYINDEX, p->parentRef);
				}

				return 0;
			}

			int tostring(lua_State* L)
			{
				Wrapper<Point>* p = PLUM_CHECK_DATA(L, 1, Point);
				lua_pushfstring(L, "plum.Point(%f, %f)", p->data->x, p->data->y);
				return 1;
			}

			int len(lua_State* L)
			{
				Wrapper<Point>* p = PLUM_CHECK_DATA(L, 1, Point);
				lua_pushnumber(L, 2);
				return 1;
			}

			int getx(lua_State* L)
			{
				Wrapper<Point>* p = PLUM_CHECK_DATA(L, 1, Point);
				lua_pushnumber(L, p->data->x);
				return 1;
			}

			int gety(lua_State* L)
			{
				Wrapper<Point>* p = PLUM_CHECK_DATA(L, 1, Point);
				lua_pushnumber(L, p->data->y);
				return 1;
			}

			int setx(lua_State* L)
			{
				Wrapper<Point>* p = PLUM_CHECK_DATA(L, 1, Point);
				double val = luaL_checknumber(L, 2);
				p->data->x = val;
				return 0;
			}

			int sety(lua_State* L)
			{
				Wrapper<Point>* p = PLUM_CHECK_DATA(L, 1, Point);
				double val = luaL_checknumber(L, 2);
				p->data->y = val;
				return 0;
			}

            int setLocation(lua_State* L)
            {
				Wrapper<Point>* p = PLUM_CHECK_DATA(L, 1, Point);
				double x = luaL_checknumber(L, 2);
                double y = luaL_checknumber(L, 3);
                p->data->x = x;
				p->data->y = y;
				return 0;
            }

			void openLibrary(lua_State* L)
			{
				luaL_newmetatable(L, libraryName);
				// Duplicate the metatable on the stack.
				lua_pushvalue(L, -1);
				// metatable.__index = metatable
				lua_setfield(L, -2, "__index");

				// Put the members into the metatable.
				PLUM_BIND_FUNC_BEGIN()
				PLUM_BIND_META(gc)
				PLUM_BIND_META(len)
				PLUM_BIND_META(index)
				PLUM_BIND_META(newindex)
				PLUM_BIND_META(tostring)
				PLUM_BIND_FUNC(getx)
				PLUM_BIND_FUNC(gety)
				PLUM_BIND_FUNC(setx)
				PLUM_BIND_FUNC(sety)
                PLUM_BIND_FUNC(setLocation)
				PLUM_BIND_FUNC_END_NULL()

				lua_pop(L, 1);
				
				// Push plum namespace.
				lua_getglobal(L, "plum");

				// plum.Point = <function create>
				lua_pushstring(L, "Point");
				lua_pushcfunction(L, create);
				lua_settable(L, -3);

				// Pop plum namespace.
				lua_pop(L, 1);
			}
		}
	}
}