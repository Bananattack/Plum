#include "../plum.h"

namespace Plum
{
	namespace ScriptLibrary
	{
		namespace RectObject
		{
			SCRIPT_OBJ_GETTER(index, Wrapper<Rect>*, libraryName)
			SCRIPT_OBJ_SETTER(newindex, Wrapper<Rect>*, libraryName)

			int create(lua_State* L)
			{
				double x = luaL_checknumber(L, 1);
				double y = luaL_checknumber(L, 2);
				double x2 = luaL_checknumber(L, 3);
				double y2 = luaL_checknumber(L, 4);
				PLUM_PUSH_DATA(L, Rect, new Rect(x, y, x2, y2), LUA_NOREF);
				return 1;
			}

			int gc(lua_State* L)
			{
				Wrapper<Rect>* r = PLUM_CHECK_DATA(L, 1, Rect);

				// Only delete if it doesn't belong to a parent of some sort.
				if(r->parentRef != LUA_NOREF)
				{
					delete r->data;
				}
				else
				{
					luaL_unref(L, LUA_REGISTRYINDEX, r->parentRef);
				}

				return 0;
			}

			int tostring(lua_State* L)
			{
				Wrapper<Rect>* r = PLUM_CHECK_DATA(L, 1, Rect);
				lua_pushfstring(L, "plum.Rect(%f, %f, %f, %f)", r->data->x, r->data->y, r->data->x2, r->data->y2);
				return 1;
			}

			int len(lua_State* L)
			{
				Wrapper<Rect>* r = PLUM_CHECK_DATA(L, 1, Rect);
				lua_pushnumber(L, 4);
				return 1;
			}

			int getx(lua_State* L)
			{
				Wrapper<Rect>* r = PLUM_CHECK_DATA(L, 1, Rect);
				lua_pushnumber(L, r->data->x);
				return 1;
			}

			int gety(lua_State* L)
			{
				Wrapper<Rect>* r = PLUM_CHECK_DATA(L, 1, Rect);
				lua_pushnumber(L, r->data->y);
				return 1;
			}

			int getx2(lua_State* L)
			{
				Wrapper<Rect>* r = PLUM_CHECK_DATA(L, 1, Rect);
				lua_pushnumber(L, r->data->x2);
				return 1;
			}

			int gety2(lua_State* L)
			{
				Wrapper<Rect>* r = PLUM_CHECK_DATA(L, 1, Rect);
				lua_pushnumber(L, r->data->y2);
				return 1;
			}

			int setx(lua_State* L)
			{
				Wrapper<Rect>* r = PLUM_CHECK_DATA(L, 1, Rect);
				double val = luaL_checknumber(L, 2);
				r->data->x = val;
				return 0;
			}

			int sety(lua_State* L)
			{
				Wrapper<Rect>* r = PLUM_CHECK_DATA(L, 1, Rect);
				double val = luaL_checknumber(L, 2);
				r->data->y = val;
				return 0;
			}

			int setx2(lua_State* L)
			{
				Wrapper<Rect>* r = PLUM_CHECK_DATA(L, 1, Rect);
				double val = luaL_checknumber(L, 2);
				r->data->x2 = val;
				return 0;
			}

			int sety2(lua_State* L)
			{
				Wrapper<Rect>* r = PLUM_CHECK_DATA(L, 1, Rect);
				double val = luaL_checknumber(L, 2);
				r->data->y2 = val;
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
				PLUM_BIND_FUNC(getx2)
				PLUM_BIND_FUNC(gety2)
				PLUM_BIND_FUNC(setx)
				PLUM_BIND_FUNC(sety)
				PLUM_BIND_FUNC(setx2)
				PLUM_BIND_FUNC(sety2)
				{ "get1", getx }, // self[1]
				{ "get2", gety }, // self[2]
				{ "get3", getx2 }, // self[3]
				{ "get4", gety2 }, // self[4]
				{ "set1", setx }, // self[1] = val
				{ "set2", sety }, // self[2] = val
				{ "set3", setx2 }, // self[3] = val
				{ "set4", sety2 }, // self[4] = val
				PLUM_BIND_FUNC_END_NULL()

				lua_pop(L, 1);
				
				// Push plum namespace.
				lua_getglobal(L, "plum");

				// plum.Rect = <function create>
				lua_pushstring(L, "Rect");
				lua_pushcfunction(L, create);
				lua_settable(L, -3);

				// Pop plum namespace.
				lua_pop(L, 1);
			}
		}
	}
}