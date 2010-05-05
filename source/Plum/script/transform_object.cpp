#include "../plum.h"

namespace Plum
{
	namespace ScriptLibrary
	{
		namespace TransformObject
		{
			Point ZeroPoint;
			Point UnitPoint;

			const int REF_POSITION = 1;
			const int REF_CLIP = 2;
			const int REF_SCALE = 3;
			const int REF_PIVOT = 4;

			SCRIPT_OBJ_GETTER(index, Wrapper<Transform>*, libraryName)
			SCRIPT_OBJ_SETTER(newindex, Wrapper<Transform>*, libraryName)

			void rawGetExtValue(lua_State* L, Wrapper<Transform>* w, int key)
			{
				// Push ext table.
				PLUM_GET_EXT(L, w, Transform);
				// Get value.
				lua_rawgeti(L, -1, key);
				// Remove ext from stack, while keeping value on top of the stack.
				lua_remove(L, -2);
			}

			void rawSetExtValue(lua_State* L, Wrapper<Transform>* w, int key)
			{
				// Push ext table.
				PLUM_GET_EXT(L, w, Transform);
				// Push the value.
				lua_pushvalue(L, -2);
				// Set value.
				lua_rawseti(L, -2, key);
				// Pop ext.
				lua_pop(L, 1);
			}

			int create(lua_State* L)
			{
				const char* const CONSTRUCT_ERR = "Bad plum.Transform constructor call.";

				// Validate constructor argument, must be table or nil.
				int argumentType = lua_type(L, 1);

				if(argumentType != LUA_TNONE && argumentType != LUA_TNIL && argumentType != LUA_TTABLE)
				{
					luaL_error(L, "%s Argument must be a table or nil. Got %s value instead.", CONSTRUCT_ERR, luaL_typename(L, 1));
				}

				// Create transform object.
				Transform* t = new Transform();
				// Push transform object.
				Wrapper<Transform>* w = PLUM_PUSH_DATA(L, Transform, t, LUA_NOREF);

				// Push argument.
				lua_pushvalue(L, 1);
				if(argumentType == LUA_TTABLE)
				{
					// position
					lua_getfield(L, -1, "position");
					if(!lua_isnil(L, -1))
					{
						if(!PLUM_IS_DATA(L, -1, Point))
						{
							luaL_error(L, "%s 'position' must be a plum.Point or nil. Got %s value instead.", CONSTRUCT_ERR, luaL_typename(L, -1));
						}
						Wrapper<Point>* p = (Wrapper<Point>*) lua_touserdata(L, -1);
						t->position = p->data;
					}
					else
					{
						t->position = &ZeroPoint;
					}
					rawSetExtValue(L, w, REF_POSITION);
					lua_pop(L, 1);

					// clip
					lua_getfield(L, -1, "clip");
					if(!lua_isnil(L, -1))
					{
						if(!PLUM_IS_DATA(L, -1, Rect))
						{
							luaL_error(L, "%s 'clip' must be a plum.Rect or nil. Got %s value instead.", CONSTRUCT_ERR, luaL_typename(L, -1));
						}
						Wrapper<Rect>* r = (Wrapper<Rect>*) lua_touserdata(L, -1);
						t->clip = r->data;
					}
					rawSetExtValue(L, w, REF_CLIP);
					lua_pop(L, 1);

					// scale
					lua_getfield(L, -1, "scale");
					if(!lua_isnil(L, -1))
					{
						if(!PLUM_IS_DATA(L, -1, Point))
						{
							luaL_error(L, "%s 'scale' must be a plum.Point or nil. Got %s value instead.", CONSTRUCT_ERR, luaL_typename(L, -1));
						}
						Wrapper<Point>* p = (Wrapper<Point>*) lua_touserdata(L, -1);
						t->scale = p->data;
					}
					else
					{
						t->position = &UnitPoint;
					}
					rawSetExtValue(L, w, REF_SCALE);
					lua_pop(L, 1);

					// pivot
					lua_getfield(L, -1, "pivot");
					if(!lua_isnil(L, -1))
					{
						if(!PLUM_IS_DATA(L, -1, Point))
						{
							luaL_error(L, "%s 'pivot' must be a plum.Point or nil. Got %s value instead.", CONSTRUCT_ERR, luaL_typename(L, -1));
						}
						Wrapper<Point>* p = (Wrapper<Point>*) lua_touserdata(L, -1);
						t->pivot = p->data;
					}
					else
					{
						t->position = &ZeroPoint;
					}
					rawSetExtValue(L, w, REF_PIVOT);
					lua_pop(L, 1);

					// angle
					lua_getfield(L, -1, "angle");
					if(lua_isnumber(L, -1))
					{
						t->angle = lua_tonumber(L, -1);
					}
					else if(!lua_isnil(L, -1))
					{
						luaL_error(L, "%s angle must be a number.", CONSTRUCT_ERR);
					}
					lua_pop(L, 1);

					// mode
					lua_getfield(L, -1, "mode");
					if(lua_isnumber(L, -1))
					{
						t->mode = (BlendMode) lua_tointeger(L, -1);
					}
					else if(!lua_isnil(L, -1))
					{
						luaL_error(L, "%s mode must be an integer.", CONSTRUCT_ERR);
					}
					lua_pop(L, 1);

					// tint
					lua_getfield(L, -1, "tint");
					if(lua_isnumber(L, -1))
					{
						t->tint = lua_tointeger(L, -1);	
					}
					else if(!lua_isnil(L, -1))
					{
						luaL_error(L, "%s tint must be an integer.", CONSTRUCT_ERR);
					}
					lua_pop(L, 1);
				}
				lua_pop(L, 1);

				return 1;
			}

			int gc(lua_State* L)
			{
				Wrapper<Transform>* t = PLUM_CHECK_DATA(L, 1, Transform);

				// Discard ext table.
				luaL_unref(L, LUA_REGISTRYINDEX, t->extRef);

				// Only delete if it doesn't belong to a parent of some sort.
				if(t->parentRef != LUA_NOREF)
				{
					delete t->data;
				}
				else
				{
					luaL_unref(L, LUA_REGISTRYINDEX, t->parentRef);
				}
				return 0;
			}

			int tostring(lua_State* L)
			{
				PLUM_CHECK_DATA(L, 1, Transform);
				lua_pushstring(L, "(plum.Transform object)");
				return 1;
			}

			int getmirror(lua_State* L)
			{
				Wrapper<Transform>* t = PLUM_CHECK_DATA(L, 1, Transform);
				lua_pushboolean(L, t->data->mirror);
				return 1;
			}

			int setmirror(lua_State* L)
			{
				Wrapper<Transform>* t = PLUM_CHECK_DATA(L, 1, Transform);
				bool mirror = lua_toboolean(L, 2) != 0;
				t->data->mirror = mirror;
				return 0;
			}

			int getangle(lua_State* L)
			{
				Wrapper<Transform>* t = PLUM_CHECK_DATA(L, 1, Transform);
				lua_pushnumber(L, t->data->angle);
				return 1;
			}

			int setangle(lua_State* L)
			{
				Wrapper<Transform>* t = PLUM_CHECK_DATA(L, 1, Transform);
				double value = luaL_checknumber(L, 2);
				t->data->angle = value;
				return 0;
			}

			int getmode(lua_State* L)
			{
				Wrapper<Transform>* t = PLUM_CHECK_DATA(L, 1, Transform);
				lua_pushinteger(L, t->data->mode);
				return 1;
			}

			int setmode(lua_State* L)
			{
				Wrapper<Transform>* t = PLUM_CHECK_DATA(L, 1, Transform);
				int value = luaL_checkinteger(L, 2);
				t->data->mode = (BlendMode) value;
				return 0;
			}

			int gettint(lua_State* L)
			{
				Wrapper<Transform>* t = PLUM_CHECK_DATA(L, 1, Transform);
				lua_pushinteger(L, t->data->tint);
				return 1;
			}

			int settint(lua_State* L)
			{
				Wrapper<Transform>* t = PLUM_CHECK_DATA(L, 1, Transform);
				int value = luaL_checkinteger(L, 2);
				t->data->tint = value;
				return 0;
			}

			int getposition(lua_State* L)
			{
				Wrapper<Transform>* t = PLUM_CHECK_DATA(L, 1, Transform);
				rawGetExtValue(L, t, REF_POSITION);
				return 1;
			}

			int getclip(lua_State* L)
			{
				Wrapper<Transform>* t = PLUM_CHECK_DATA(L, 1, Transform);
				rawGetExtValue(L, t, REF_CLIP);
				return 1;
			}

			int getscale(lua_State* L)
			{
				Wrapper<Transform>* t = PLUM_CHECK_DATA(L, 1, Transform);
				rawGetExtValue(L, t, REF_SCALE);
				return 1;
			}

			int getpivot(lua_State* L)
			{
				Wrapper<Transform>* t = PLUM_CHECK_DATA(L, 1, Transform);
				rawGetExtValue(L, t, REF_PIVOT);
				return 1;
			}

			int setposition(lua_State* L)
			{
				Wrapper<Transform>* t = PLUM_CHECK_DATA(L, 1, Transform);
				if(lua_isnil(L, 2))
				{
					lua_pushnil(L);
					t->data->position = &ZeroPoint;
				}
				else
				{
					Wrapper<Point>* value = PLUM_CHECK_DATA(L, 2, Point);
					lua_pushvalue(L, 2);
					t->data->position = value->data;
				}
				rawSetExtValue(L, t, REF_POSITION);
				return 0;
			}

			int setclip(lua_State* L)
			{
				Wrapper<Transform>* t = PLUM_CHECK_DATA(L, 1, Transform);
				if(lua_isnil(L, 2))
				{
					lua_pushnil(L);
					t->data->clip = NULL;
				}
				else
				{
					Wrapper<Rect>* value = PLUM_CHECK_DATA(L, 2, Rect);
					lua_pushvalue(L, 2);
					t->data->clip = value->data;
				}
				rawSetExtValue(L, t, REF_CLIP);
				return 0;
			}

			int setscale(lua_State* L)
			{
				Wrapper<Transform>* t = PLUM_CHECK_DATA(L, 1, Transform);
				if(lua_isnil(L, 2))
				{
					lua_pushnil(L);
					t->data->scale = &UnitPoint;
				}
				else
				{
					Wrapper<Point>* value = PLUM_CHECK_DATA(L, 2, Point);
					lua_pushvalue(L, 2);
					t->data->scale = value->data;
				}
				rawSetExtValue(L, t, REF_SCALE);
				return 0;
			}

			int setpivot(lua_State* L)
			{
				Wrapper<Transform>* t = PLUM_CHECK_DATA(L, 1, Transform);
				if(lua_isnil(L, 2))
				{
					lua_pushnil(L);
					t->data->pivot = &ZeroPoint;
				}
				else
				{
					Wrapper<Point>* value = PLUM_CHECK_DATA(L, 2, Point);
					lua_pushvalue(L, 2);
					t->data->pivot = value->data;
				}
				rawSetExtValue(L, t, REF_PIVOT);
				return 0;
			}

			void openLibrary(lua_State* L)
			{
				// Defaults for points.
				ZeroPoint.x = ZeroPoint.y = 1;
				UnitPoint.x = UnitPoint.y = 1;

				luaL_newmetatable(L, libraryName);
				// Duplicate the metatable on the stack.
				lua_pushvalue(L, -1);
				// metatable.__index = metatable
				lua_setfield(L, -2, "__index");

				// Put the members into the metatable.
				PLUM_BIND_FUNC_BEGIN()
				PLUM_BIND_META(gc)
				PLUM_BIND_META(index)
				PLUM_BIND_META(newindex)
				PLUM_BIND_META(tostring)
				PLUM_BIND_FUNC(getmirror)
				PLUM_BIND_FUNC(setmirror)
				PLUM_BIND_FUNC(getangle)
				PLUM_BIND_FUNC(setangle)
				PLUM_BIND_FUNC(getmode)
				PLUM_BIND_FUNC(setmode)
				PLUM_BIND_FUNC(gettint)
				PLUM_BIND_FUNC(settint)
				PLUM_BIND_FUNC(getposition)
				PLUM_BIND_FUNC(getclip)
				PLUM_BIND_FUNC(getscale)
				PLUM_BIND_FUNC(getpivot)
				PLUM_BIND_FUNC(setposition)
				PLUM_BIND_FUNC(setclip)
				PLUM_BIND_FUNC(setscale)
				PLUM_BIND_FUNC(setpivot)
				PLUM_BIND_FUNC_END_NULL()

				lua_pop(L, 1);
				
				// Push plum namespace.
				lua_getglobal(L, "plum");

				// plum.Transform = <function create>
				lua_pushstring(L, "Transform");
				lua_pushcfunction(L, create);
				lua_settable(L, -3);

				// Pop plum namespace.
				lua_pop(L, 1);
			}
		}
	}
}