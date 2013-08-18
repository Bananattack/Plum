#include "script.h"
#include "../core/canvas.h"

namespace plum
{
    namespace script
    {
        template<> const char* meta<Canvas>()
        {
            return "plum.Canvas";
        }
    }

    namespace
    {
        typedef Canvas Self;

        int create(lua_State* L)
        {
            if(script::is<int>(L, 1) && script::is<int>(L, 2))
            {
                auto w = script::get<int>(L, 1);
                auto h = script::get<int>(L, 2);

                script::push(L, new Canvas(w, h), LUA_NOREF);
                    
                return 1;
            }
            else if(script::is<const char*>(L, 1))
            {
                auto filename = script::get<const char*>(L, 1);

                script::push(L, new Canvas(Canvas::load(filename)), LUA_NOREF);
                    
                return 1;
            }
            else
            {
                luaL_error(L, "Attempt to call plum.Canvas constructor with invalid argument types.\r\nMust be (string filename) or (int w, int h).");
                return 0;
            }
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

        int pairs(lua_State* L)
        {
            return script::wrapped<Self>(L, 1)->pairs(L);
        }

        int restoreClipRegion(lua_State* L)
        {
            auto canvas = script::ptr<Self>(L, 1);
            canvas->restoreClipRegion();
            return 0;
        }

        int setClipRegion(lua_State* L)
        {
            auto canvas = script::ptr<Self>(L, 1);
            auto x = script::get<int>(L, 2);
            auto y = script::get<int>(L, 3);
            auto x2 = script::get<int>(L, 4);
            auto y2 = script::get<int>(L, 5);
            canvas->setClipRegion(x, y, x2, y2);
            return 0;
        }

        int getPixel(lua_State* L)
        {
            auto canvas = script::ptr<Self>(L, 1);
            auto x = script::get<int>(L, 2);
            auto y = script::get<int>(L, 3);
            script::push(L, int(canvas->get(x, y)));
            return 1;
        }

        int setPixel(lua_State* L)
        {
            auto canvas = script::ptr<Self>(L, 1);
            auto x = script::get<int>(L, 2);
            auto y = script::get<int>(L, 3);
            auto color = Color(script::get<int>(L, 4));
            auto mode = BlendMode(script::get<int>(L, 5, BlendPreserve));

            switch(mode)
            {
                case BlendOpaque: canvas->dot<BlendOpaque>(x, y, color); break;
                case BlendMerge: canvas->dot<BlendMerge>(x, y, color); break;
                case BlendPreserve: canvas->dot<BlendPreserve>(x, y, color); break;
                case BlendAdd: canvas->dot<BlendAdd>(x, y, color); break;
                case BlendSubtract: canvas->dot<BlendSubtract>(x, y, color); break;
            }
            return 0;
        }

        int clear(lua_State* L)
        {
            auto canvas = script::ptr<Self>(L, 1);
            auto color = script::get<int>(L, 2);

            canvas->clear(color);
                
            return 0;
        }

        int replaceColor(lua_State* L)
        {
            auto canvas = script::ptr<Self>(L, 1);
            auto find = Color(script::get<int>(L, 2));
            auto replace = Color(script::get<int>(L, 3));
            canvas->replaceColor(find, replace);
            return 0;
        }

        int flip(lua_State* L)
        {
            auto canvas = script::ptr<Self>(L, 1);
            auto h = script::get<bool>(L, 2) != 0;
            auto v = script::get<bool>(L, 3) != 0;
            canvas->flip(h, v);
            return 0;
        }

        int line(lua_State* L)
        {
            auto canvas = script::ptr<Self>(L, 1);
            auto x = script::get<int>(L, 2);
            auto y = script::get<int>(L, 3);
            auto x2 = script::get<int>(L, 4);
            auto y2 = script::get<int>(L, 5);
            auto color = Color(script::get<int>(L, 6));
            auto mode = BlendMode(script::get<int>(L, 7, BlendPreserve));

            switch(mode)
            {
                case BlendOpaque: canvas->line<BlendOpaque>(x, y, x2, y2, color); break;
                case BlendMerge: canvas->line<BlendMerge>(x, y, x2, y2, color); break;
                case BlendPreserve: canvas->line<BlendPreserve>(x, y, x2, y2, color); break;
                case BlendAdd: canvas->line<BlendAdd>(x, y, x2, y2, color); break;
                case BlendSubtract: canvas->line<BlendSubtract>(x, y, x2, y2, color); break;
            }
            return 0;
        }

        int rect(lua_State* L)
        {
            auto canvas = script::ptr<Self>(L, 1);
            auto x = script::get<int>(L, 2);
            auto y = script::get<int>(L, 3);
            auto x2 = script::get<int>(L, 4);
            auto y2 = script::get<int>(L, 5);
            auto color = Color(script::get<int>(L, 6));
            auto mode = BlendMode(script::get<int>(L, 7, BlendPreserve));

            switch(mode)
            {
                case BlendOpaque: canvas->rect<BlendOpaque>(x, y, x2, y2, color); break;
                case BlendMerge: canvas->rect<BlendMerge>(x, y, x2, y2, color); break;
                case BlendPreserve: canvas->rect<BlendPreserve>(x, y, x2, y2, color); break;
                case BlendAdd: canvas->rect<BlendAdd>(x, y, x2, y2, color); break;
                case BlendSubtract: canvas->rect<BlendSubtract>(x, y, x2, y2, color); break;
            }
            return 0;
        }

        int solidRect(lua_State* L)
        {
            auto canvas = script::ptr<Self>(L, 1);
            auto x = script::get<int>(L, 2);
            auto y = script::get<int>(L, 3);
            auto x2 = script::get<int>(L, 4);
            auto y2 = script::get<int>(L, 5);
            auto color = Color(script::get<int>(L, 6));
            auto mode = BlendMode(script::get<int>(L, 7, BlendPreserve));

            switch(mode)
            {
                case BlendOpaque: canvas->fillRect<BlendOpaque>(x, y, x2, y2, color); break;
                case BlendMerge: canvas->fillRect<BlendMerge>(x, y, x2, y2, color); break;
                case BlendPreserve: canvas->fillRect<BlendPreserve>(x, y, x2, y2, color); break;
                case BlendAdd: canvas->fillRect<BlendAdd>(x, y, x2, y2, color); break;
                case BlendSubtract: canvas->fillRect<BlendSubtract>(x, y, x2, y2, color); break;
            }
            return 0;
        }

        int circle(lua_State* L)
        {
            auto canvas = script::ptr<Self>(L, 1);
            auto cx = script::get<int>(L, 2);
            auto cy = script::get<int>(L, 3);
            auto xRadius = script::get<int>(L, 4);
            auto yRadius = script::get<int>(L, 5);
            auto color = Color(script::get<int>(L, 6));
            auto mode = BlendMode(script::get<int>(L, 7, BlendPreserve));

            switch(mode)
            {
                case BlendOpaque: canvas->ellipse<BlendOpaque>(cx, cy, xRadius, yRadius, color); break;
                case BlendMerge: canvas->ellipse<BlendMerge>(cx, cy, xRadius, yRadius, color); break;
                case BlendPreserve: canvas->ellipse<BlendPreserve>(cx, cy, xRadius, yRadius, color); break;
                case BlendAdd: canvas->ellipse<BlendAdd>(cx, cy, xRadius, yRadius, color); break;
                case BlendSubtract: canvas->ellipse<BlendSubtract>(cx, cy, xRadius, yRadius, color); break;
            }
            return 0;
        }

        int solidCircle(lua_State* L)
        {
            auto canvas = script::ptr<Self>(L, 1);
            auto cx = script::get<int>(L, 2);
            auto cy = script::get<int>(L, 3);
            auto xRadius = script::get<int>(L, 4);
            auto yRadius = script::get<int>(L, 5);
            auto color = Color(script::get<int>(L, 6));
            auto mode = BlendMode(script::get<int>(L, 7, BlendPreserve));

            switch(mode)
            {
                case BlendOpaque: canvas->fillEllipse<BlendOpaque>(cx, cy, xRadius, yRadius, color); break;
                case BlendMerge: canvas->fillEllipse<BlendMerge>(cx, cy, xRadius, yRadius, color); break;
                case BlendPreserve: canvas->fillEllipse<BlendPreserve>(cx, cy, xRadius, yRadius, color); break;
                case BlendAdd: canvas->fillEllipse<BlendAdd>(cx, cy, xRadius, yRadius, color); break;
                case BlendSubtract: canvas->fillEllipse<BlendSubtract>(cx, cy, xRadius, yRadius, color); break;
            }
            return 0;
        }

        int blit(lua_State* L)
        {
            auto canvas = script::ptr<Self>(L, 1);
            auto x = script::get<int>(L, 2);
            auto y = script::get<int>(L, 3);
            auto dest = script::ptr<Self>(L, 4);
            auto mode = BlendMode(script::get<int>(L, 5, BlendPreserve));

            switch(mode)
            {
                case BlendOpaque: canvas->blit<BlendOpaque>(x, y, *dest); break;
                case BlendMerge: canvas->blit<BlendMerge>(x, y, *dest); break;
                case BlendPreserve: canvas->blit<BlendPreserve>(x, y, *dest); break;
                case BlendAdd: canvas->blit<BlendAdd>(x, y, *dest); break;
                case BlendSubtract: canvas->blit<BlendSubtract>(x, y, *dest); break;
            }
            return 0;
        }

        int scaleBlit(lua_State* L)
        {
            auto canvas = script::ptr<Self>(L, 1);
            auto x = script::get<int>(L, 2);
            auto y = script::get<int>(L, 3);
            auto scaledWidth = script::get<int>(L, 4);
            auto scaledHeight = script::get<int>(L, 5);
            auto dest = script::ptr<Self>(L, 6);
            auto mode = BlendMode(script::get<int>(L, 7, BlendPreserve));

            switch(mode)
            {
                case BlendOpaque: canvas->scaleBlit<BlendOpaque>(x, y, scaledWidth, scaledHeight, *dest); break;
                case BlendMerge: canvas->scaleBlit<BlendMerge>(x, y, scaledWidth, scaledHeight, *dest); break;
                case BlendPreserve: canvas->scaleBlit<BlendPreserve>(x, y, scaledWidth, scaledHeight, *dest); break;
                case BlendAdd: canvas->scaleBlit<BlendAdd>(x, y, scaledWidth, scaledHeight, *dest); break;
                case BlendSubtract: canvas->scaleBlit<BlendSubtract>(x, y, scaledWidth, scaledHeight, *dest); break;
            }
            return 0;
        }

        int rotateBlit(lua_State* L)
        {
            auto canvas = script::ptr<Self>(L, 1);
            auto x = script::get<int>(L, 2);
            auto y = script::get<int>(L, 3);
            auto angle = script::get<double>(L, 4);
            auto dest = script::ptr<Self>(L, 5);
            auto mode = BlendMode(script::get<int>(L, 6, BlendPreserve));
             
            switch(mode)
            {
                case BlendOpaque: canvas->rotateBlit<BlendOpaque>(x, y, angle, *dest); break;
                case BlendMerge: canvas->rotateBlit<BlendMerge>(x, y, angle, *dest); break;
                case BlendPreserve: canvas->rotateBlit<BlendPreserve>(x, y, angle, *dest); break;
                case BlendAdd: canvas->rotateBlit<BlendAdd>(x, y, angle, *dest); break;
                case BlendSubtract: canvas->rotateBlit<BlendSubtract>(x, y, angle, *dest); break;
            }
            return 0;
        }

        int rotateScaleBlit(lua_State* L)
        {
            auto canvas = script::ptr<Self>(L, 1);
            auto x = script::get<int>(L, 2);
            auto y = script::get<int>(L, 3);
            auto angle = script::get<double>(L, 4);
            auto scale = script::get<double>(L, 5);
            auto dest = script::ptr<Self>(L, 6);
            auto mode = BlendMode(script::get<int>(L, 7, BlendPreserve));
                
            switch(mode)
            {
                case BlendOpaque: canvas->rotateScaleBlit<BlendOpaque>(x, y, angle, scale, *dest); break;
                case BlendMerge: canvas->rotateScaleBlit<BlendMerge>(x, y, angle, scale, *dest); break;
                case BlendPreserve: canvas->rotateScaleBlit<BlendPreserve>(x, y, angle, scale, *dest); break;
                case BlendAdd: canvas->rotateScaleBlit<BlendAdd>(x, y, angle, scale, *dest); break;
                case BlendSubtract: canvas->rotateScaleBlit<BlendSubtract>(x, y, angle, scale, *dest); break;
            }
            return 0;
        }

        int blitRegion(lua_State* L)
        {
            auto canvas = script::ptr<Self>(L, 1);
            auto sx = script::get<int>(L, 2);
            auto sy = script::get<int>(L, 3);
            auto sx2 = script::get<int>(L, 4);
            auto sy2 = script::get<int>(L, 5);
            auto dx = script::get<int>(L, 6);
            auto dy = script::get<int>(L, 7);
            auto dest = script::ptr<Self>(L, 8);
            auto mode = BlendMode(script::get<int>(L, 9, BlendPreserve));

            switch(mode)
            {
                case BlendOpaque: canvas->blitRegion<BlendOpaque>(sx, sy, sx2, sy2, dx, dy, *dest); break;
                case BlendMerge: canvas->blitRegion<BlendMerge>(sx, sy, sx2, sy2, dx, dy, *dest); break;
                case BlendPreserve: canvas->blitRegion<BlendPreserve>(sx, sy, sx2, sy2, dx, dy, *dest); break;
                case BlendAdd: canvas->blitRegion<BlendAdd>(sx, sy, sx2, sy2, dx, dy, *dest); break;
                case BlendSubtract: canvas->blitRegion<BlendSubtract>(sx, sy, sx2, sy2, dx, dy, *dest); break;
            }
            return 0;
        }

        int scaleBlitRegion(lua_State* L)
        {
            auto canvas = script::ptr<Self>(L, 1);
            auto sx = script::get<int>(L, 2);
            auto sy = script::get<int>(L, 3);
            auto sx2 = script::get<int>(L, 4);
            auto sy2 = script::get<int>(L, 5);
            auto dx = script::get<int>(L, 6);
            auto dy = script::get<int>(L, 7);
            auto scw = script::get<int>(L, 8);
            auto sch = script::get<int>(L, 9);
            auto dest = script::ptr<Self>(L, 10);
            auto mode = BlendMode(script::get<int>(L, 11, BlendPreserve));

            switch(mode)
            {
                case BlendOpaque: canvas->scaleBlitRegion<BlendOpaque>(sx, sy, sx2, sy2, dx, dy, scw, sch, *dest); break;
                case BlendMerge: canvas->scaleBlitRegion<BlendMerge>(sx, sy, sx2, sy2, dx, dy, scw, sch, *dest); break;
                case BlendPreserve: canvas->scaleBlitRegion<BlendPreserve>(sx, sy, sx2, sy2, dx, dy, scw, sch, *dest); break;
                case BlendAdd: canvas->scaleBlitRegion<BlendAdd>(sx, sy, sx2, sy2, dx, dy, scw, sch, *dest); break;
                case BlendSubtract: canvas->scaleBlitRegion<BlendSubtract>(sx, sy, sx2, sy2, dx, dy, scw, sch, *dest); break;
            }
            return 0;
        }

        int rotateBlitRegion(lua_State* L)
        {
            auto canvas = script::ptr<Self>(L, 1);
            auto sx = script::get<int>(L, 2);
            auto sy = script::get<int>(L, 3);
            auto sx2 = script::get<int>(L, 4);
            auto sy2 = script::get<int>(L, 5);
            auto dx = script::get<int>(L, 6);
            auto dy = script::get<int>(L, 7);
            auto angle = script::get<double>(L, 8);
            auto dest = script::ptr<Self>(L, 9);
            auto mode = BlendMode(script::get<int>(L, 10, BlendPreserve));

            switch(mode)
            {
                case BlendOpaque: canvas->rotateBlitRegion<BlendOpaque>(sx, sy, sx2, sy2, dx, dy, angle, *dest); break;
                case BlendMerge: canvas->rotateBlitRegion<BlendMerge>(sx, sy, sx2, sy2, dx, dy, angle, *dest); break;
                case BlendPreserve: canvas->rotateBlitRegion<BlendPreserve>(sx, sy, sx2, sy2, dx, dy, angle, *dest); break;
                case BlendAdd: canvas->rotateBlitRegion<BlendAdd>(sx, sy, sx2, sy2, dx, dy, angle, *dest); break;
                case BlendSubtract: canvas->rotateBlitRegion<BlendSubtract>(sx, sy, sx2, sy2, dx, dy, angle, *dest); break;
            }
            return 0;
        }

        int rotateScaleBlitRegion(lua_State* L)
        {
            auto canvas = script::ptr<Self>(L, 1);
            auto sx = script::get<int>(L, 2);
            auto sy = script::get<int>(L, 3);
            auto sx2 = script::get<int>(L, 4);
            auto sy2 = script::get<int>(L, 5);
            auto dx = script::get<int>(L, 6);
            auto dy = script::get<int>(L, 7);
            auto angle = script::get<double>(L, 8);
            auto scale = script::get<double>(L, 9);
            auto dest = script::ptr<Self>(L, 10);
            auto mode = BlendMode(script::get<int>(L, 11, BlendPreserve));
            
            switch(mode)
            {
                case BlendOpaque: canvas->rotateScaleBlitRegion<BlendOpaque>(sx, sy, sx2, sy2, dx, dy, angle, scale, *dest); break;
                case BlendMerge: canvas->rotateScaleBlitRegion<BlendMerge>(sx, sy, sx2, sy2, dx, dy, angle, scale, *dest); break;
                case BlendPreserve: canvas->rotateScaleBlitRegion<BlendPreserve>(sx, sy, sx2, sy2, dx, dy, angle, scale, *dest); break;
                case BlendAdd: canvas->rotateScaleBlitRegion<BlendAdd>(sx, sy, sx2, sy2, dx, dy, angle, scale, *dest); break;
                case BlendSubtract: canvas->rotateScaleBlitRegion<BlendSubtract>(sx, sy, sx2, sy2, dx, dy, angle, scale, *dest); break;
            }
            return 0;
        }

        int getTrueWidth(lua_State* L)
        {
            auto canvas = script::ptr<Self>(L, 1);
            script::push(L, canvas->getTrueWidth());

            return 1;
        }

        int getTrueHeight(lua_State* L)
        {
            auto canvas = script::ptr<Self>(L, 1);
            script::push(L, canvas->getTrueHeight());

            return 1;
        }

        int get_width(lua_State* L)
        {
            auto canvas = script::ptr<Self>(L, 1);
            script::push(L, canvas->getWidth());

            return 1;
        }

        int get_height(lua_State* L)
        {
            auto canvas = script::ptr<Self>(L, 1);
            script::push(L, canvas->getHeight());

            return 1;
        }

        int get_opacity(lua_State* L)
        {
            auto canvas = script::ptr<Self>(L, 1);
            script::push(L, canvas->getOpacity());

            return 1;
        }

        int set_opacity(lua_State* L)
        {
            auto canvas = script::ptr<Self>(L, 1);
            auto value = script::get<int>(L, 2);
            canvas->setOpacity(value);

            return 0;
        }
    }

    namespace script
    {
        void initCanvasObject(lua_State* L)
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
                {"__pairs", pairs},
                {"restoreClipRegion", restoreClipRegion},
                {"setClipRegion", setClipRegion},
                {"getPixel", getPixel},
                {"setPixel", setPixel},
                {"clear", clear},
                {"flip", flip},
                {"replaceColor", replaceColor},
                {"line", line},
                {"rect", rect},
                {"solidRect", solidRect},
                {"circle", circle},
                {"solidCircle", solidCircle},
                {"blit", blit},
                {"scaleBlit", scaleBlit},
                {"rotateBlit", rotateBlit},
                {"rotateScaleBlit", rotateScaleBlit},
                {"blitRegion", blitRegion},
                {"scaleBlitRegion", scaleBlitRegion},
                {"rotateBlitRegion", rotateBlitRegion},
                {"rotateScaleBlitRegion", rotateScaleBlitRegion},
                {"get_trueWidth", getTrueWidth},
                {"get_trueHeight", getTrueHeight},
                {"get_width", get_width},
                {"get_height", get_height},
                {"get_opacity", get_opacity},
                {"set_opacity", set_opacity},
                {nullptr, nullptr}
            };
            luaL_setfuncs(L, functions, 0);

            lua_pop(L, 1);

            // Push plum namespace.
            lua_getglobal(L, "plum");

            // plum[classname] = create
            script::push(L, "Canvas");
            lua_pushcfunction(L, create);
            lua_settable(L, -3);

            // Pop plum namespace.
            lua_pop(L, 1);

        }
    }
}
