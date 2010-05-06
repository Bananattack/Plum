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
				double w = luaL_checknumber(L, 3);
				double h = luaL_checknumber(L, 4);
				PLUM_PUSH_DATA(L, Rect, new Rect(x, y, w, h), LUA_NOREF);
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
				lua_pushfstring(L, "plum.Rect(%f, %f, %f, %f)", r->data->x, r->data->y, r->data->width, r->data->height);
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
				lua_pushnumber(L, r->data->x + r->data->width);
				return 1;
			}

			int gety2(lua_State* L)
			{
				Wrapper<Rect>* r = PLUM_CHECK_DATA(L, 1, Rect);
				lua_pushnumber(L, r->data->y + r->data->height);
				return 1;
			}

			int getwidth(lua_State* L)
			{
				Wrapper<Rect>* r = PLUM_CHECK_DATA(L, 1, Rect);
				lua_pushnumber(L, r->data->width);
				return 1;
			}

			int getheight(lua_State* L)
			{
				Wrapper<Rect>* r = PLUM_CHECK_DATA(L, 1, Rect);
				lua_pushnumber(L, r->data->height);
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
				r->data->width = val - r->data->x;
				return 0;
			}

			int sety2(lua_State* L)
			{
				Wrapper<Rect>* r = PLUM_CHECK_DATA(L, 1, Rect);
				double val = luaL_checknumber(L, 2);
				r->data->height = val - r->data->y;
				return 0;
			}

			int setwidth(lua_State* L)
			{
				Wrapper<Rect>* r = PLUM_CHECK_DATA(L, 1, Rect);
				double val = luaL_checknumber(L, 2);
                r->data->width = val;
				return 1;
			}

			int setheight(lua_State* L)
			{
				Wrapper<Rect>* r = PLUM_CHECK_DATA(L, 1, Rect);
				double val = luaL_checknumber(L, 2);
                r->data->height = val;
				return 1;
			}

			int setLocation(lua_State* L)
			{
				Wrapper<Rect>* r = PLUM_CHECK_DATA(L, 1, Rect);
				double x = luaL_checknumber(L, 2);
				double y = luaL_checknumber(L, 3);
				r->data->x = x;
				r->data->y = y;
				return 0;
			}

			int setSize(lua_State* L)
			{
				Wrapper<Rect>* r = PLUM_CHECK_DATA(L, 1, Rect);
				double w = luaL_checknumber(L, 2);
				double h = luaL_checknumber(L, 3);
				r->data->width = w;
				r->data->height = h;
				return 0;
			}

            int setRegion(lua_State* L)
            {
				Wrapper<Rect>* r = PLUM_CHECK_DATA(L, 1, Rect);
				double x = luaL_checknumber(L, 2);
                double y = luaL_checknumber(L, 3);
                double w = luaL_checknumber(L, 4);
                double h = luaL_checknumber(L, 5);
                r->data->x = x;
				r->data->y = y;
                r->data->width = w;
                r->data->height = h;
				return 0;
            }

			int containsPoint(lua_State* L)
			{
				Wrapper<Rect>* r = PLUM_CHECK_DATA(L, 1, Rect);
                Wrapper<Point>* p = PLUM_CHECK_DATA(L, 2, Point);
                Point* point = p->data;
                Rect* self = r->data;
                lua_pushboolean(L, point->x >= self->x
			        && point->x <= self->x + self->width
			        && point->y >= self->y
			        && point->y <= self->y + self->height);
                return 1;
			}

			int touchesRect(lua_State* L)
			{
				Wrapper<Rect>* r = PLUM_CHECK_DATA(L, 1, Rect);
                Wrapper<Rect>* r2 = PLUM_CHECK_DATA(L, 2, Rect);
                Rect* self = r->data;
                Rect* target = r2->data;
		        lua_pushboolean(L, self->x + self->width >= target->x
			        && self->x <= target->x + target->width
			        && self->y + self->height >= target->y
			        && self->y <= target->y + target->height);
                return 1;
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
                PLUM_BIND_FUNC(getwidth)
                PLUM_BIND_FUNC(getheight)
				PLUM_BIND_FUNC(setx)
				PLUM_BIND_FUNC(sety)
				PLUM_BIND_FUNC(setx2)
				PLUM_BIND_FUNC(sety2)
                PLUM_BIND_FUNC(setwidth)
                PLUM_BIND_FUNC(setheight)
				PLUM_BIND_FUNC(setLocation)
				PLUM_BIND_FUNC(setSize)
                PLUM_BIND_FUNC(setRegion)
                PLUM_BIND_FUNC(containsPoint)
                PLUM_BIND_FUNC(touchesRect)
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