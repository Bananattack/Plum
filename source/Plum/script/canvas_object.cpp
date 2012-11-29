
#include "script.h"
#include "../video/video.h"
#include "../video/canvas.h"

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
            if(lua_isnumber(L, 1) && lua_isnumber(L, 2))
            {
                auto w = script::get<int>(L, 1);
                auto h = script::get<int>(L, 2);

                script::push(L, new Canvas(w, h), LUA_NOREF);
                    
                return 1;
            }
            else if(lua_isstring(L, 1))
            {
                auto filename = script::get<const char*>(L, 1);

                script::push(L, Canvas::load(filename), LUA_NOREF);
                    
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
            lua_pushinteger(L, canvas->getPixel(x, y));
            return 1;
        }

        int setPixel(lua_State* L)
        {
            auto canvas = script::ptr<Self>(L, 1);
            auto x = script::get<int>(L, 2);
            auto y = script::get<int>(L, 3);
            auto color = Color(script::get<int>(L, 4, Color::White));
            auto mode = BlendMode(script::get<int>(L, 5, BlendAlpha));

            switch(mode)
            {
                case BlendOpaque:
                    canvas->setPixel<SoftOpaqueBlender>(x, y, color, SoftOpaqueBlender());
                    break;
                case BlendMerge:
                    canvas->setPixel<SoftMergeBlender>(x, y, color, SoftMergeBlender());
                    break;
                case BlendAlpha:
                    canvas->setPixel<SoftPreserveBlender>(x, y, color, SoftPreserveBlender());
                    break;
                case BlendAdd:
                    canvas->setPixel<SoftAddBlender>(x, y, color, SoftAddBlender());
                    break;
                case BlendSubtract:
                    canvas->setPixel<SoftSubtractBlender>(x, y, color, SoftSubtractBlender());
                    break;
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
            auto color = Color(script::get<int>(L, 6, Color::White));
            auto mode = BlendMode(script::get<int>(L, 7, BlendAlpha));

            switch(mode)
            {
                case BlendOpaque:
                    canvas->line<SoftOpaqueBlender>(x, y, x2, y2, color, SoftOpaqueBlender());
                    break;
                case BlendMerge:
                    canvas->line<SoftMergeBlender>(x, y, x2, y2, color, SoftMergeBlender());
                    break;
                case BlendAlpha:
                    canvas->line<SoftPreserveBlender>(x, y, x2, y2, color, SoftPreserveBlender());
                    break;
                case BlendAdd:
                    canvas->line<SoftAddBlender>(x, y, x2, y2, color, SoftAddBlender());
                    break;
                case BlendSubtract:
                    canvas->line<SoftSubtractBlender>(x, y, x2, y2, color, SoftSubtractBlender());
                    break;
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
            auto color = Color(script::get<int>(L, 6, Color::White));
            auto mode = BlendMode(script::get<int>(L, 7, BlendAlpha));

            switch(mode)
            {
                case BlendOpaque:
                    canvas->rect<SoftOpaqueBlender>(x, y, x2, y2, color, SoftOpaqueBlender());
                    break;
                case BlendMerge:
                    canvas->rect<SoftMergeBlender>(x, y, x2, y2, color, SoftMergeBlender());
                    break;
                case BlendAlpha:
                    canvas->rect<SoftPreserveBlender>(x, y, x2, y2, color, SoftPreserveBlender());
                    break;
                case BlendAdd:
                    canvas->rect<SoftAddBlender>(x, y, x2, y2, color, SoftAddBlender());
                    break;
                case BlendSubtract:
                    canvas->rect<SoftSubtractBlender>(x, y, x2, y2, color, SoftSubtractBlender());
                    break;
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
            auto color = Color(script::get<int>(L, 6, Color::White));
            auto mode = BlendMode(script::get<int>(L, 7, BlendAlpha));

            switch(mode)
            {
                case BlendOpaque:
                    canvas->solidRect<SoftOpaqueBlender>(x, y, x2, y2, color, SoftOpaqueBlender());
                    break;
                case BlendMerge:
                    canvas->solidRect<SoftMergeBlender>(x, y, x2, y2, color, SoftMergeBlender());
                    break;
                case BlendAlpha:
                    canvas->solidRect<SoftPreserveBlender>(x, y, x2, y2, color, SoftPreserveBlender());
                    break;
                case BlendAdd:
                    canvas->solidRect<SoftAddBlender>(x, y, x2, y2, color, SoftAddBlender());
                    break;
                case BlendSubtract:
                    canvas->solidRect<SoftSubtractBlender>(x, y, x2, y2, color, SoftSubtractBlender());
                    break;
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
            auto color = Color(script::get<int>(L, 6, Color::White));
            auto mode = BlendMode(script::get<int>(L, 7, BlendAlpha));

            switch(mode)
            {
                case BlendOpaque:
                    canvas->circle<SoftOpaqueBlender>(cx, cy, xRadius, yRadius, color, SoftOpaqueBlender());
                    break;
                case BlendMerge:
                    canvas->circle<SoftMergeBlender>(cx, cy, xRadius, yRadius, color, SoftMergeBlender());
                    break;
                case BlendAlpha:
                    canvas->circle<SoftPreserveBlender>(cx, cy, xRadius, yRadius, color, SoftPreserveBlender());
                    break;
                case BlendAdd:
                    canvas->circle<SoftAddBlender>(cx, cy, xRadius, yRadius, color, SoftAddBlender());
                    break;
                case BlendSubtract:
                    canvas->circle<SoftSubtractBlender>(cx, cy, xRadius, yRadius, color, SoftSubtractBlender());
                    break;
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
            auto color = Color(script::get<int>(L, 6, Color::White));
            auto mode = BlendMode(script::get<int>(L, 7, BlendAlpha));

            switch(mode)
            {
                case BlendOpaque:
                    canvas->solidCircle<SoftOpaqueBlender>(cx, cy, xRadius, yRadius, color, SoftOpaqueBlender());
                    break;
                case BlendMerge:
                    canvas->solidCircle<SoftMergeBlender>(cx, cy, xRadius, yRadius, color, SoftMergeBlender());
                    break;
                case BlendAlpha:
                    canvas->solidCircle<SoftPreserveBlender>(cx, cy, xRadius, yRadius, color, SoftPreserveBlender());
                    break;
                case BlendAdd:
                    canvas->solidCircle<SoftAddBlender>(cx, cy, xRadius, yRadius, color, SoftAddBlender());
                    break;
                case BlendSubtract:
                    canvas->solidCircle<SoftSubtractBlender>(cx, cy, xRadius, yRadius, color, SoftSubtractBlender());
                    break;
            }
            return 0;
        }

        int blit(lua_State* L)
        {
            auto canvas = script::ptr<Self>(L, 1);
            auto x = script::get<int>(L, 2);
            auto y = script::get<int>(L, 3);
            auto dest = script::ptr<Self>(L, 4);
            auto mode = BlendMode(script::get<int>(L, 5, BlendAlpha));
                
            switch(mode)
            {
                case BlendOpaque:
                    canvas->blit<SoftOpaqueBlender>(x, y, dest, SoftOpaqueBlender());
                    break;
                case BlendMerge:
                    canvas->blit<SoftMergeBlender>(x, y, dest, SoftMergeBlender());
                    break;
                case BlendAlpha:
                    canvas->blit<SoftPreserveBlender>(x, y, dest, SoftPreserveBlender());
                    break;
                case BlendAdd:
                    canvas->blit<SoftAddBlender>(x, y, dest, SoftAddBlender());
                    break;
                case BlendSubtract:
                    canvas->blit<SoftSubtractBlender>(x, y, dest, SoftSubtractBlender());
                    break;
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
            auto mode = BlendMode(script::get<int>(L, 7, BlendAlpha));
                
            switch(mode)
            {
                case BlendOpaque:
                    canvas->scaleBlit<SoftOpaqueBlender>(x, y, scaledWidth, scaledHeight, dest, SoftOpaqueBlender());
                    break;
                case BlendMerge:
                    canvas->scaleBlit<SoftMergeBlender>(x, y, scaledWidth, scaledHeight, dest, SoftMergeBlender());
                    break;
                case BlendAlpha:
                    canvas->scaleBlit<SoftPreserveBlender>(x, y, scaledWidth, scaledHeight, dest, SoftPreserveBlender());
                    break;
                case BlendAdd:
                    canvas->scaleBlit<SoftAddBlender>(x, y, scaledWidth, scaledHeight, dest, SoftAddBlender());
                    break;
                case BlendSubtract:
                    canvas->scaleBlit<SoftSubtractBlender>(x, y, scaledWidth, scaledHeight, dest, SoftSubtractBlender());
                    break;
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
            auto mode = BlendMode(script::get<int>(L, 6, BlendAlpha));
                
            switch(mode)
            {
                case BlendOpaque:
                    canvas->rotateBlit<SoftOpaqueBlender>(x, y, angle, dest, SoftOpaqueBlender());
                    break;
                case BlendMerge:
                    canvas->rotateBlit<SoftMergeBlender>(x, y, angle, dest, SoftMergeBlender());
                    break;
                case BlendAlpha:
                    canvas->rotateBlit<SoftPreserveBlender>(x, y, angle, dest, SoftPreserveBlender());
                    break;
                case BlendAdd:
                    canvas->rotateBlit<SoftAddBlender>(x, y, angle, dest, SoftAddBlender());
                    break;
                case BlendSubtract:
                    canvas->rotateBlit<SoftSubtractBlender>(x, y, angle, dest, SoftSubtractBlender());
                    break;
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
            auto mode = BlendMode(script::get<int>(L, 7, BlendAlpha));
                
            switch(mode)
            {
                case BlendOpaque:
                    canvas->rotateScaleBlit<SoftOpaqueBlender>(x, y, angle, scale, dest, SoftOpaqueBlender());
                    break;
                case BlendMerge:
                    canvas->rotateScaleBlit<SoftMergeBlender>(x, y, angle, scale, dest, SoftMergeBlender());
                    break;
                case BlendAlpha:
                    canvas->rotateScaleBlit<SoftPreserveBlender>(x, y, angle, scale, dest, SoftPreserveBlender());
                    break;
                case BlendAdd:
                    canvas->rotateScaleBlit<SoftAddBlender>(x, y, angle, scale, dest, SoftAddBlender());
                    break;
                case BlendSubtract:
                    canvas->rotateScaleBlit<SoftSubtractBlender>(x, y, angle, scale, dest, SoftSubtractBlender());
                    break;
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
            auto mode = BlendMode(script::get<int>(L, 9, BlendAlpha));
                
            switch(mode)
            {
                case BlendOpaque:
                    canvas->blitRegion<SoftOpaqueBlender>(sx, sy, sx2, sy2, dx, dy, dest, SoftOpaqueBlender());
                    break;
                case BlendMerge:
                    canvas->blitRegion<SoftMergeBlender>(sx, sy, sx2, sy2, dx, dy, dest, SoftMergeBlender());
                    break;
                case BlendAlpha:
                    canvas->blitRegion<SoftPreserveBlender>(sx, sy, sx2, sy2, dx, dy, dest, SoftPreserveBlender());
                    break;
                case BlendAdd:
                    canvas->blitRegion<SoftAddBlender>(sx, sy, sx2, sy2, dx, dy, dest, SoftAddBlender());
                    break;
                case BlendSubtract:
                    canvas->blitRegion<SoftSubtractBlender>(sx, sy, sx2, sy2, dx, dy, dest, SoftSubtractBlender());
                    break;
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
            auto mode = BlendMode(script::get<int>(L, 11, BlendAlpha));
                
            switch(mode)
            {
                case BlendOpaque:
                    canvas->scaleBlitRegion<SoftOpaqueBlender>(sx, sy, sx2, sy2, dx, dy, scw, sch, dest, SoftOpaqueBlender());
                    break;
                case BlendMerge:
                    canvas->scaleBlitRegion<SoftMergeBlender>(sx, sy, sx2, sy2, dx, dy, scw, sch, dest, SoftMergeBlender());
                    break;
                case BlendAlpha:
                    canvas->scaleBlitRegion<SoftPreserveBlender>(sx, sy, sx2, sy2, dx, dy, scw, sch, dest, SoftPreserveBlender());
                    break;
                case BlendAdd:
                    canvas->scaleBlitRegion<SoftAddBlender>(sx, sy, sx2, sy2, dx, dy, scw, sch, dest, SoftAddBlender());
                    break;
                case BlendSubtract:
                    canvas->scaleBlitRegion<SoftSubtractBlender>(sx, sy, sx2, sy2, dx, dy, scw, sch, dest, SoftSubtractBlender());
                    break;
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
            auto mode = BlendMode(script::get<int>(L, 10, BlendAlpha));
                
            switch(mode)
            {
                case BlendOpaque:
                    canvas->rotateBlitRegion<SoftOpaqueBlender>(sx, sy, sx2, sy2, dx, dy, angle, dest, SoftOpaqueBlender());
                    break;
                case BlendMerge:
                    canvas->rotateBlitRegion<SoftMergeBlender>(sx, sy, sx2, sy2, dx, dy, angle, dest, SoftMergeBlender());
                    break;
                case BlendAlpha:
                    canvas->rotateBlitRegion<SoftPreserveBlender>(sx, sy, sx2, sy2, dx, dy, angle, dest, SoftPreserveBlender());
                    break;
                case BlendAdd:
                    canvas->rotateBlitRegion<SoftAddBlender>(sx, sy, sx2, sy2, dx, dy, angle, dest, SoftAddBlender());
                    break;
                case BlendSubtract:
                    canvas->rotateBlitRegion<SoftSubtractBlender>(sx, sy, sx2, sy2, dx, dy, angle, dest, SoftSubtractBlender());
                    break;
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
            auto mode = BlendMode(script::get<int>(L, 11, BlendAlpha));
                
            switch(mode)
            {
                case BlendOpaque:
                    canvas->rotateScaleBlitRegion<SoftOpaqueBlender>(sx, sy, sx2, sy2, dx, dy, angle, scale, dest, SoftOpaqueBlender());
                    break;
                case BlendMerge:
                    canvas->rotateScaleBlitRegion<SoftMergeBlender>(sx, sy, sx2, sy2, dx, dy, angle, scale, dest, SoftMergeBlender());
                    break;
                case BlendAlpha:
                    canvas->rotateScaleBlitRegion<SoftPreserveBlender>(sx, sy, sx2, sy2, dx, dy, angle, scale, dest, SoftPreserveBlender());
                    break;
                case BlendAdd:
                    canvas->rotateScaleBlitRegion<SoftAddBlender>(sx, sy, sx2, sy2, dx, dy, angle, scale, dest, SoftAddBlender());
                    break;
                case BlendSubtract:
                    canvas->rotateScaleBlitRegion<SoftSubtractBlender>(sx, sy, sx2, sy2, dx, dy, angle, scale, dest, SoftSubtractBlender());
                    break;
            }
            return 0;
        }

        int gettrueWidth(lua_State* L)
        {
            auto canvas = script::ptr<Self>(L, 1);
            script::push(L, canvas->width);

            return 1;
        }

        int gettrueHeight(lua_State* L)
        {
            auto canvas = script::ptr<Self>(L, 1);
            script::push(L, canvas->height);

            return 1;
        }

        int getwidth(lua_State* L)
        {
            auto canvas = script::ptr<Self>(L, 1);
            script::push(L, canvas->occupiedWidth);

            return 1;
        }

        int getheight(lua_State* L)
        {
            auto canvas = script::ptr<Self>(L, 1);
            script::push(L, canvas->occupiedHeight);

            return 1;
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
                {"gettrueWidth", gettrueWidth},
                {"gettrueHeight", gettrueHeight},
                {"getwidth", getwidth},
                {"getheight", getheight},
                {nullptr, nullptr}
            };
            luaL_register(L, nullptr, functions);

            lua_pop(L, 1);

            // Push plum namespace.
            lua_getglobal(L, "plum");

            // plum[classname] = create
            lua_pushstring(L, "Canvas");
            lua_pushcfunction(L, create);
            lua_settable(L, -3);

            // Pop plum namespace.
            lua_pop(L, 1);

        }
    }
}
