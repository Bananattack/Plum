
#include "../core/transform.h"
#include "script.h"

namespace plum
{
    namespace script
    {
        template<> const char* meta<Transform>()
        {
            return "plum.Transform";
        }
    }

    namespace
    {
        typedef Transform Self;
        Point ZeroPoint;
        Point UnitPoint;

        enum
        {
            REF_POSITION = 1,
            REF_CLIP,
            REF_SCALE,
            REF_PIVOT
        };

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
            Self* t = new Self();
            // Push transform object.
            auto w = script::push(L, t, LUA_NOREF);

            // Push argument.
            lua_pushvalue(L, 1);
            if(argumentType == LUA_TTABLE)
            {
                // position
                lua_getfield(L, -1, "position");
                if(!script::is<nullptr_t>(L, -1))
                {
                    if(!script::is<Point>(L, -1))
                    {
                        luaL_error(L, "%s 'position' must be a plum.Point or nil. Got %s value instead.", CONSTRUCT_ERR, luaL_typename(L, -1));
                    }
                    t->position = script::ptr<Point>(L, -1);
                }
                else
                {
                    t->position = &ZeroPoint;
                }
                w->setAttribute(L, REF_POSITION);
                lua_pop(L, 1);

                // clip
                lua_getfield(L, -1, "clip");
                if(!script::is<nullptr_t>(L, -1))
                {
                    if(!script::is<Rect>(L, -1))
                    {
                        luaL_error(L, "%s 'clip' must be a plum.Rect or nil. Got %s value instead.", CONSTRUCT_ERR, luaL_typename(L, -1));
                    }
                    t->clip = script::ptr<Rect>(L, -1);
                }
                w->setAttribute(L, REF_CLIP);
                lua_pop(L, 1);

                // scale
                lua_getfield(L, -1, "scale");
                if(!script::is<nullptr_t>(L, -1))
                {
                    if(!script::is<Point>(L, -1))
                    {
                        luaL_error(L, "%s 'scale' must be a plum.Point or nil. Got %s value instead.", CONSTRUCT_ERR, luaL_typename(L, -1));
                    }
                    t->position = script::ptr<Point>(L, -1);
                }
                else
                {
                    t->position = &UnitPoint;
                }
                w->setAttribute(L, REF_SCALE);
                lua_pop(L, 1);

                // pivot
                lua_getfield(L, -1, "pivot");
                if(!script::is<nullptr_t>(L, -1))
                {
                    if(!script::is<Point>(L, -1))
                    {
                        luaL_error(L, "%s 'pivot' must be a plum.Point or nil. Got %s value instead.", CONSTRUCT_ERR, luaL_typename(L, -1));
                    }
                    t->pivot = script::ptr<Point>(L, -1);
                }
                else
                {
                    t->position = &ZeroPoint;
                }
                w->setAttribute(L, REF_PIVOT);
                lua_pop(L, 1);

                // angle
                lua_getfield(L, -1, "angle");
                if(script::is<double>(L, -1))
                {
                    t->angle = script::get<double>(L, -1);
                }
                else if(!script::is<nullptr_t>(L, -1))
                {
                    luaL_error(L, "%s angle must be a number.", CONSTRUCT_ERR);
                }
                lua_pop(L, 1);

                // mode
                lua_getfield(L, -1, "mode");
                if(script::is<int>(L, -1))
                {
                    t->mode = (BlendMode) script::get<int>(L, -1);
                }
                else if(!script::is<nullptr_t>(L, -1))
                {
                    luaL_error(L, "%s mode must be an integer.", CONSTRUCT_ERR);
                }
                lua_pop(L, 1);

                // tint
                lua_getfield(L, -1, "tint");
                if(script::is<int>(L, -1))
                {
                    t->tint = script::get<int>(L, -1);    
                }
                else if(!script::is<nullptr_t>(L, -1))
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

        int getmirror(lua_State* L)
        {
            auto t = script::ptr<Transform>(L, 1);
            script::push(L, t->mirror);
            return 1;
        }

        int setmirror(lua_State* L)
        {
            auto t = script::ptr<Transform>(L, 1);
            auto mirror = script::get<bool>(L, 2);
            t->mirror = mirror;
            return 0;
        }

        int getangle(lua_State* L)
        {
            auto t = script::ptr<Transform>(L, 1);
            script::push(L, t->angle);
            return 1;
        }

        int setangle(lua_State* L)
        {
            auto t = script::ptr<Transform>(L, 1);
            auto value = script::get<double>(L, 2);
            t->angle = value;
            return 0;
        }

        int getmode(lua_State* L)
        {
            auto t = script::ptr<Transform>(L, 1);
            script::push(L, (int) t->mode);
            return 1;
        }

        int setmode(lua_State* L)
        {
            auto t = script::ptr<Transform>(L, 1);
            auto value = script::get<int>(L, 2);
            t->mode = (BlendMode) value;
            return 0;
        }

        int gettint(lua_State* L)
        {
            auto t = script::ptr<Transform>(L, 1);
            script::push(L, (int) t->tint);
            return 1;
        }

        int settint(lua_State* L)
        {
            auto t = script::ptr<Transform>(L, 1);
            auto value = script::get<int>(L, 2);
            t->tint = value;
            return 0;
        }

        int getposition(lua_State* L)
        {
            script::wrapped<Transform>(L, 1)->getAttribute(L, REF_POSITION);
            return 1;
        }

        int getclip(lua_State* L)
        {
            script::wrapped<Transform>(L, 1)->getAttribute(L, REF_CLIP);
            return 1;
        }

        int getscale(lua_State* L)
        {
            script::wrapped<Transform>(L, 1)->getAttribute(L, REF_SCALE);
            return 1;
        }

        int getpivot(lua_State* L)
        {
            script::wrapped<Transform>(L, 1)->getAttribute(L, REF_PIVOT);
            return 1;
        }

        int setposition(lua_State* L)
        {
            auto wrapper = script::wrapped<Transform>(L, 1);
            if(lua_isnil(L, 2))
            {
                script::push(L, nullptr);
                wrapper->data->position = &ZeroPoint;
            }
            else
            {
                auto value = script::ptr<Point>(L, 2);
                lua_pushvalue(L, 2);
                wrapper->data->position = value;
            }
            wrapper->setAttribute(L, REF_POSITION);
            return 0;
        }

        int setclip(lua_State* L)
        {
            auto wrapper = script::wrapped<Transform>(L, 1);
            if(lua_isnil(L, 2))
            {
                script::push(L, nullptr);
                wrapper->data->clip = nullptr;
            }
            else
            {
                auto value = script::ptr<Rect>(L, 2);
                lua_pushvalue(L, 2);
                wrapper->data->clip = value;
            }
            wrapper->setAttribute(L, REF_CLIP);
            return 0;
        }

        int setscale(lua_State* L)
        {
            auto wrapper = script::wrapped<Transform>(L, 1);
            if(lua_isnil(L, 2))
            {
                script::push(L, nullptr);
                wrapper->data->scale = &UnitPoint;
            }
            else
            {
                auto value = script::ptr<Point>(L, 2);
                lua_pushvalue(L, 2);
                wrapper->data->scale = value;
            }
            wrapper->setAttribute(L, REF_SCALE);
            return 0;
        }

        int setpivot(lua_State* L)
        {
            auto wrapper = script::wrapped<Transform>(L, 1);
            if(lua_isnil(L, 2))
            {
                script::push(L, nullptr);
                wrapper->data->pivot = &ZeroPoint;
            }
            else
            {
                auto value = script::ptr<Point>(L, 2);
                lua_pushvalue(L, 2);
                wrapper->data->pivot = value;
            }
            wrapper->setAttribute(L, REF_PIVOT);
            return 0;
        }
    }

    namespace script
    {
        void initTransformObject(lua_State* L)
        {
            // Defaults for points.
            ZeroPoint.x = ZeroPoint.y = 0;
            UnitPoint.x = UnitPoint.y = 1;

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
                {"getmirror", getmirror},
                {"setmirror", setmirror},
                {"getangle", getangle},
                {"setangle", setangle},
                {"getmode", getmode},
                {"setmode", setmode},
                {"gettint", gettint},
                {"settint", settint},
                {"getposition", getposition},
                {"getclip", getclip},
                {"getscale", getscale},
                {"getpivot", getpivot},
                {"setposition", setposition},
                {"setclip", setclip},
                {"setscale", setscale},
                {"setpivot", setpivot},
                {nullptr, nullptr}
            };
            luaL_setfuncs(L, functions, 0);

            lua_pop(L, 1);
                
            // Push plum namespace.
            lua_getglobal(L, "plum");

            // plum.Transform = <function create>
            script::push(L, "Transform");
            lua_pushcfunction(L, create);
            lua_settable(L, -3);

            // Pop plum namespace.
            lua_pop(L, 1);
        }
    }
}