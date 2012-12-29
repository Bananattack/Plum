
#include "../core/transform.h"
#include "script.h"

namespace plum
{
    namespace script
    {
        template<> const char* meta<Rect>()
        {
            return "plum.Rect";
        }
    }

    namespace
    {
        typedef Rect Self;
        int create(lua_State* L)
        {
            auto x = script::get<double>(L, 1);
            auto y = script::get<double>(L, 2);
            auto w = script::get<double>(L, 3);
            auto h = script::get<double>(L, 4);
            script::push(L, new Self(x, y, w, h), LUA_NOREF);
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

        int len(lua_State* L)
        {
            auto r = script::ptr<Self>(L, 1);
            script::push(L, 4);
            return 1;
        }

        int get_x(lua_State* L)
        {
            auto r = script::ptr<Self>(L, 1);
            script::push(L, r->x);
            return 1;
        }

        int get_y(lua_State* L)
        {
            auto r = script::ptr<Self>(L, 1);
            script::push(L, r->y);
            return 1;
        }

        int get_x2(lua_State* L)
        {
            auto r = script::ptr<Self>(L, 1);
            script::push(L, r->x + r->width);
            return 1;
        }

        int get_y2(lua_State* L)
        {
            auto r = script::ptr<Self>(L, 1);
            script::push(L, r->y + r->height);
            return 1;
        }

        int get_width(lua_State* L)
        {
            auto r = script::ptr<Self>(L, 1);
            script::push(L, r->width);
            return 1;
        }

        int get_height(lua_State* L)
        {
            auto r = script::ptr<Self>(L, 1);
            script::push(L, r->height);
            return 1;
        }

        int set_x(lua_State* L)
        {
            auto r = script::ptr<Self>(L, 1);
            auto val = script::get<double>(L, 2);
            r->x = val;
            return 0;
        }

        int set_y(lua_State* L)
        {
            auto r = script::ptr<Self>(L, 1);
            auto val = script::get<double>(L, 2);
            r->y = val;
            return 0;
        }

        int set_x2(lua_State* L)
        {
            auto r = script::ptr<Self>(L, 1);
            auto val = script::get<double>(L, 2);
            r->width = val - r->x;
            return 0;
        }

        int set_y2(lua_State* L)
        {
            auto r = script::ptr<Self>(L, 1);
            auto val = script::get<double>(L, 2);
            r->height = val - r->y;
            return 0;
        }

        int set_width(lua_State* L)
        {
            auto r = script::ptr<Self>(L, 1);
            auto val = script::get<double>(L, 2);
            r->width = val;
            return 1;
        }

        int set_height(lua_State* L)
        {
            auto r = script::ptr<Self>(L, 1);
            auto val = script::get<double>(L, 2);
            r->height = val;
            return 1;
        }

        int setOrigin(lua_State* L)
        {
            auto r = script::ptr<Self>(L, 1);
            auto x = script::get<double>(L, 2);
            auto y = script::get<double>(L, 3);
            r->x = x;
            r->y = y;
            return 0;
        }

        int setSize(lua_State* L)
        {
            auto r = script::ptr<Self>(L, 1);
            auto w = script::get<double>(L, 2);
            auto h = script::get<double>(L, 3);
            r->width = w;
            r->height = h;
            return 0;
        }

        int setRegion(lua_State* L)
        {
            auto r = script::ptr<Self>(L, 1);
            auto x = script::get<double>(L, 2);
            auto y = script::get<double>(L, 3);
            auto w = script::get<double>(L, 4);
            auto h = script::get<double>(L, 5);
            r->x = x;
            r->y = y;
            r->width = w;
            r->height = h;
            return 0;
        }

        int containsPoint(lua_State* L)
        {
            auto self = script::ptr<Self>(L, 1);
            auto point = script::ptr<Point>(L, 2);
            script::push(L, point->x >= self->x
                && point->x <= self->x + self->width
                && point->y >= self->y
                && point->y <= self->y + self->height);
            return 1;
        }

        int touchesSelf(lua_State* L)
        {
            auto self = script::ptr<Self>(L, 1);
            auto target = script::ptr<Self>(L, 2);
            script::push(L, self->x + self->width >= target->x
                && self->x <= target->x + target->width
                && self->y + self->height >= target->y
                && self->y <= target->y + target->height);
            return 1;
        }
    }

    namespace script
    {
        void initRectObject(lua_State* L)
        {
            luaL_newmetatable(L, meta<Self>());
            // Duplicate the metatable on the stack.
            lua_pushvalue(L, -1);
            // metatable.__index = metatable
            lua_setfield(L, -2, "__index");

            // Put the members into the metatable.
            const luaL_Reg functions[] = {
                {"__gc", gc},
                {"__len", len},
                {"__index", index},
                {"__newindex", newindex},
                {"__tostring", tostring},
                {"get_x", get_x},
                {"get_y", get_y},
                {"get_x2", get_x2},
                {"get_y2", get_y2},
                {"get_width", get_width},
                {"get_height", get_height},
                {"set_x", set_x},
                {"set_y", set_y},
                {"set_x2", set_x2},
                {"set_y2", set_y2},
                {"set_width", set_width},
                {"set_height", set_height},
                {"setOrigin", setOrigin},
                {"setSize", setSize},
                {"setRegion", setRegion},
                {"containsPoint", containsPoint},
                {"touchesSelf", touchesSelf},
                {nullptr, nullptr},
            };
            luaL_setfuncs(L, functions, 0);

            lua_pop(L, 1);
                
            // Push plum namespace.
            lua_getglobal(L, "plum");

            // plum.Self = <function create>
            script::push(L, "Rect");
            lua_pushcfunction(L, create);
            lua_settable(L, -3);

            // Pop plum namespace.
            lua_pop(L, 1);
        }
    }
}