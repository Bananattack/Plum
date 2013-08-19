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

        void initCanvasObject(lua_State* L)
        {
            luaL_newmetatable(L, meta<Canvas>());
            // Duplicate the metatable on the stack.
            lua_pushvalue(L, -1);
            // metatable.__index = metatable
            lua_setfield(L, -2, "__index");

            // Put the members into the metatable.
            const luaL_Reg functions[] = {
                {"__gc", [](lua_State* L) { return script::wrapped<Canvas>(L, 1)->gc(L); }},
                {"__index", [](lua_State* L) { return script::wrapped<Canvas>(L, 1)->index(L); }},
                {"__newindex", [](lua_State* L) { return script::wrapped<Canvas>(L, 1)->newindex(L); }},
                {"__tostring", [](lua_State* L) { return script::wrapped<Canvas>(L, 1)->tostring(L); }},
                {"__pairs", [](lua_State* L) { return script::wrapped<Canvas>(L, 1)->pairs(L); }},
                {"restoreClipRegion", [](lua_State* L)
                {
                    auto canvas = script::ptr<Canvas>(L, 1);
                    canvas->restoreClipRegion();
                    return 0;
                }},
                {"setClipRegion", [](lua_State* L)
                {
                    auto canvas = script::ptr<Canvas>(L, 1);
                    auto x = script::get<int>(L, 2);
                    auto y = script::get<int>(L, 3);
                    auto x2 = script::get<int>(L, 4);
                    auto y2 = script::get<int>(L, 5);
                    canvas->setClipRegion(x, y, x2, y2);
                    return 0;
                }},
                {"get", [](lua_State* L)
                {
                    auto canvas = script::ptr<Canvas>(L, 1);
                    auto x = script::get<int>(L, 2);
                    auto y = script::get<int>(L, 3);
                    script::push(L, int(canvas->get(x, y)));
                    return 1;
                }},
                {"dot", [](lua_State* L)
                {
                    auto canvas = script::ptr<Canvas>(L, 1);
                    auto x = script::get<int>(L, 2);
                    auto y = script::get<int>(L, 3);
                    auto color = Color(script::get<int>(L, 4));
                    auto mode = BlendMode(script::get<int>(L, 5, int(BlendMode::Preserve)));

                    switch(mode)
                    {
                        case BlendMode::Opaque: canvas->dot<BlendMode::Opaque>(x, y, color); break;
                        case BlendMode::Merge: canvas->dot<BlendMode::Merge>(x, y, color); break;
                        case BlendMode::Preserve: canvas->dot<BlendMode::Preserve>(x, y, color); break;
                        case BlendMode::Add: canvas->dot<BlendMode::Add>(x, y, color); break;
                        case BlendMode::Subtract: canvas->dot<BlendMode::Subtract>(x, y, color); break;
                    }
                    return 0;
                }},
                {"clear", [](lua_State* L)
                {
                    auto canvas = script::ptr<Canvas>(L, 1);
                    auto color = script::get<int>(L, 2);

                    canvas->clear(color);
                
                    return 0;
                }},
                {"flip", [](lua_State* L)
                {
                    auto canvas = script::ptr<Canvas>(L, 1);
                    auto h = script::get<bool>(L, 2) != 0;
                    auto v = script::get<bool>(L, 3) != 0;
                    canvas->flip(h, v);
                    return 0;
                }},
                {"replaceColor", [](lua_State* L)
                {
                    auto canvas = script::ptr<Canvas>(L, 1);
                    auto find = Color(script::get<int>(L, 2));
                    auto replace = Color(script::get<int>(L, 3));
                    canvas->replaceColor(find, replace);
                    return 0;
                }},
                {"line", [](lua_State* L)
                {
                    auto canvas = script::ptr<Canvas>(L, 1);
                    auto x = script::get<int>(L, 2);
                    auto y = script::get<int>(L, 3);
                    auto x2 = script::get<int>(L, 4);
                    auto y2 = script::get<int>(L, 5);
                    auto color = Color(script::get<int>(L, 6));
                    auto mode = BlendMode(script::get<int>(L, 7, int(BlendMode::Preserve)));

                    switch(mode)
                    {
                        case BlendMode::Opaque: canvas->line<BlendMode::Opaque>(x, y, x2, y2, color); break;
                        case BlendMode::Merge: canvas->line<BlendMode::Merge>(x, y, x2, y2, color); break;
                        case BlendMode::Preserve: canvas->line<BlendMode::Preserve>(x, y, x2, y2, color); break;
                        case BlendMode::Add: canvas->line<BlendMode::Add>(x, y, x2, y2, color); break;
                        case BlendMode::Subtract: canvas->line<BlendMode::Subtract>(x, y, x2, y2, color); break;
                    }
                    return 0;
                }},
                {"rect", [](lua_State* L)
                {
                    auto canvas = script::ptr<Canvas>(L, 1);
                    auto x = script::get<int>(L, 2);
                    auto y = script::get<int>(L, 3);
                    auto x2 = script::get<int>(L, 4);
                    auto y2 = script::get<int>(L, 5);
                    auto color = Color(script::get<int>(L, 6));
                    auto mode = BlendMode(script::get<int>(L, 7, int(BlendMode::Preserve)));

                    switch(mode)
                    {
                        case BlendMode::Opaque: canvas->rect<BlendMode::Opaque>(x, y, x2, y2, color); break;
                        case BlendMode::Merge: canvas->rect<BlendMode::Merge>(x, y, x2, y2, color); break;
                        case BlendMode::Preserve: canvas->rect<BlendMode::Preserve>(x, y, x2, y2, color); break;
                        case BlendMode::Add: canvas->rect<BlendMode::Add>(x, y, x2, y2, color); break;
                        case BlendMode::Subtract: canvas->rect<BlendMode::Subtract>(x, y, x2, y2, color); break;
                    }
                    return 0;
                }},
                {"fillRect", [](lua_State* L)
                {
                    auto canvas = script::ptr<Canvas>(L, 1);
                    auto x = script::get<int>(L, 2);
                    auto y = script::get<int>(L, 3);
                    auto x2 = script::get<int>(L, 4);
                    auto y2 = script::get<int>(L, 5);
                    auto color = Color(script::get<int>(L, 6));
                    auto mode = BlendMode(script::get<int>(L, 7, int(BlendMode::Preserve)));

                    switch(mode)
                    {
                        case BlendMode::Opaque: canvas->fillRect<BlendMode::Opaque>(x, y, x2, y2, color); break;
                        case BlendMode::Merge: canvas->fillRect<BlendMode::Merge>(x, y, x2, y2, color); break;
                        case BlendMode::Preserve: canvas->fillRect<BlendMode::Preserve>(x, y, x2, y2, color); break;
                        case BlendMode::Add: canvas->fillRect<BlendMode::Add>(x, y, x2, y2, color); break;
                        case BlendMode::Subtract: canvas->fillRect<BlendMode::Subtract>(x, y, x2, y2, color); break;
                    }
                    return 0;
                }},
                {"ellipse", [](lua_State* L)
                {
                    auto canvas = script::ptr<Canvas>(L, 1);
                    auto cx = script::get<int>(L, 2);
                    auto cy = script::get<int>(L, 3);
                    auto xRadius = script::get<int>(L, 4);
                    auto yRadius = script::get<int>(L, 5);
                    auto color = Color(script::get<int>(L, 6));
                    auto mode = BlendMode(script::get<int>(L, 7, int(BlendMode::Preserve)));

                    switch(mode)
                    {
                        case BlendMode::Opaque: canvas->ellipse<BlendMode::Opaque>(cx, cy, xRadius, yRadius, color); break;
                        case BlendMode::Merge: canvas->ellipse<BlendMode::Merge>(cx, cy, xRadius, yRadius, color); break;
                        case BlendMode::Preserve: canvas->ellipse<BlendMode::Preserve>(cx, cy, xRadius, yRadius, color); break;
                        case BlendMode::Add: canvas->ellipse<BlendMode::Add>(cx, cy, xRadius, yRadius, color); break;
                        case BlendMode::Subtract: canvas->ellipse<BlendMode::Subtract>(cx, cy, xRadius, yRadius, color); break;
                    }
                    return 0;
                }},
                {"fillEllipse", [](lua_State* L)
                {
                    auto canvas = script::ptr<Canvas>(L, 1);
                    auto cx = script::get<int>(L, 2);
                    auto cy = script::get<int>(L, 3);
                    auto xRadius = script::get<int>(L, 4);
                    auto yRadius = script::get<int>(L, 5);
                    auto color = Color(script::get<int>(L, 6));
                    auto mode = BlendMode(script::get<int>(L, 7, int(BlendMode::Preserve)));

                    switch(mode)
                    {
                        case BlendMode::Opaque: canvas->fillEllipse<BlendMode::Opaque>(cx, cy, xRadius, yRadius, color); break;
                        case BlendMode::Merge: canvas->fillEllipse<BlendMode::Merge>(cx, cy, xRadius, yRadius, color); break;
                        case BlendMode::Preserve: canvas->fillEllipse<BlendMode::Preserve>(cx, cy, xRadius, yRadius, color); break;
                        case BlendMode::Add: canvas->fillEllipse<BlendMode::Add>(cx, cy, xRadius, yRadius, color); break;
                        case BlendMode::Subtract: canvas->fillEllipse<BlendMode::Subtract>(cx, cy, xRadius, yRadius, color); break;
                    }
                    return 0;
                }},
                {"blit", [](lua_State* L)
                {
                    auto canvas = script::ptr<Canvas>(L, 1);
                    auto x = script::get<int>(L, 2);
                    auto y = script::get<int>(L, 3);
                    auto dest = script::ptr<Canvas>(L, 4);
                    auto mode = BlendMode(script::get<int>(L, 5, int(BlendMode::Preserve)));

                    switch(mode)
                    {
                        case BlendMode::Opaque: canvas->blit<BlendMode::Opaque>(x, y, *dest); break;
                        case BlendMode::Merge: canvas->blit<BlendMode::Merge>(x, y, *dest); break;
                        case BlendMode::Preserve: canvas->blit<BlendMode::Preserve>(x, y, *dest); break;
                        case BlendMode::Add: canvas->blit<BlendMode::Add>(x, y, *dest); break;
                        case BlendMode::Subtract: canvas->blit<BlendMode::Subtract>(x, y, *dest); break;
                    }
                    return 0;
                }},
                {"scaleBlit", [](lua_State* L)
                {
                    auto canvas = script::ptr<Canvas>(L, 1);
                    auto x = script::get<int>(L, 2);
                    auto y = script::get<int>(L, 3);
                    auto scaledWidth = script::get<int>(L, 4);
                    auto scaledHeight = script::get<int>(L, 5);
                    auto dest = script::ptr<Canvas>(L, 6);
                    auto mode = BlendMode(script::get<int>(L, 7, int(BlendMode::Preserve)));

                    switch(mode)
                    {
                        case BlendMode::Opaque: canvas->scaleBlit<BlendMode::Opaque>(x, y, scaledWidth, scaledHeight, *dest); break;
                        case BlendMode::Merge: canvas->scaleBlit<BlendMode::Merge>(x, y, scaledWidth, scaledHeight, *dest); break;
                        case BlendMode::Preserve: canvas->scaleBlit<BlendMode::Preserve>(x, y, scaledWidth, scaledHeight, *dest); break;
                        case BlendMode::Add: canvas->scaleBlit<BlendMode::Add>(x, y, scaledWidth, scaledHeight, *dest); break;
                        case BlendMode::Subtract: canvas->scaleBlit<BlendMode::Subtract>(x, y, scaledWidth, scaledHeight, *dest); break;
                    }
                    return 0;
                }},
                {"rotateBlit", [](lua_State* L)
                {
                    auto canvas = script::ptr<Canvas>(L, 1);
                    auto x = script::get<int>(L, 2);
                    auto y = script::get<int>(L, 3);
                    auto angle = script::get<double>(L, 4);
                    auto dest = script::ptr<Canvas>(L, 5);
                    auto mode = BlendMode(script::get<int>(L, 6, int(BlendMode::Preserve)));
             
                    switch(mode)
                    {
                        case BlendMode::Opaque: canvas->rotateBlit<BlendMode::Opaque>(x, y, angle, *dest); break;
                        case BlendMode::Merge: canvas->rotateBlit<BlendMode::Merge>(x, y, angle, *dest); break;
                        case BlendMode::Preserve: canvas->rotateBlit<BlendMode::Preserve>(x, y, angle, *dest); break;
                        case BlendMode::Add: canvas->rotateBlit<BlendMode::Add>(x, y, angle, *dest); break;
                        case BlendMode::Subtract: canvas->rotateBlit<BlendMode::Subtract>(x, y, angle, *dest); break;
                    }
                    return 0;
                }},
                {"rotateScaleBlit", [](lua_State* L)
                {
                    auto canvas = script::ptr<Canvas>(L, 1);
                    auto x = script::get<int>(L, 2);
                    auto y = script::get<int>(L, 3);
                    auto angle = script::get<double>(L, 4);
                    auto scale = script::get<double>(L, 5);
                    auto dest = script::ptr<Canvas>(L, 6);
                    auto mode = BlendMode(script::get<int>(L, 7, int(BlendMode::Preserve)));
                
                    switch(mode)
                    {
                        case BlendMode::Opaque: canvas->rotateScaleBlit<BlendMode::Opaque>(x, y, angle, scale, *dest); break;
                        case BlendMode::Merge: canvas->rotateScaleBlit<BlendMode::Merge>(x, y, angle, scale, *dest); break;
                        case BlendMode::Preserve: canvas->rotateScaleBlit<BlendMode::Preserve>(x, y, angle, scale, *dest); break;
                        case BlendMode::Add: canvas->rotateScaleBlit<BlendMode::Add>(x, y, angle, scale, *dest); break;
                        case BlendMode::Subtract: canvas->rotateScaleBlit<BlendMode::Subtract>(x, y, angle, scale, *dest); break;
                    }
                    return 0;
                }},
                {"blitRegion", [](lua_State* L)
                {
                    auto canvas = script::ptr<Canvas>(L, 1);
                    auto sx = script::get<int>(L, 2);
                    auto sy = script::get<int>(L, 3);
                    auto sx2 = script::get<int>(L, 4);
                    auto sy2 = script::get<int>(L, 5);
                    auto dx = script::get<int>(L, 6);
                    auto dy = script::get<int>(L, 7);
                    auto dest = script::ptr<Canvas>(L, 8);
                    auto mode = BlendMode(script::get<int>(L, 9, int(BlendMode::Preserve)));

                    switch(mode)
                    {
                        case BlendMode::Opaque: canvas->blitRegion<BlendMode::Opaque>(sx, sy, sx2, sy2, dx, dy, *dest); break;
                        case BlendMode::Merge: canvas->blitRegion<BlendMode::Merge>(sx, sy, sx2, sy2, dx, dy, *dest); break;
                        case BlendMode::Preserve: canvas->blitRegion<BlendMode::Preserve>(sx, sy, sx2, sy2, dx, dy, *dest); break;
                        case BlendMode::Add: canvas->blitRegion<BlendMode::Add>(sx, sy, sx2, sy2, dx, dy, *dest); break;
                        case BlendMode::Subtract: canvas->blitRegion<BlendMode::Subtract>(sx, sy, sx2, sy2, dx, dy, *dest); break;
                    }
                    return 0;
                }},
                {"scaleBlitRegion", [](lua_State* L)
                {
                    auto canvas = script::ptr<Canvas>(L, 1);
                    auto sx = script::get<int>(L, 2);
                    auto sy = script::get<int>(L, 3);
                    auto sx2 = script::get<int>(L, 4);
                    auto sy2 = script::get<int>(L, 5);
                    auto dx = script::get<int>(L, 6);
                    auto dy = script::get<int>(L, 7);
                    auto scw = script::get<int>(L, 8);
                    auto sch = script::get<int>(L, 9);
                    auto dest = script::ptr<Canvas>(L, 10);
                    auto mode = BlendMode(script::get<int>(L, 11, int(BlendMode::Preserve)));

                    switch(mode)
                    {
                        case BlendMode::Opaque: canvas->scaleBlitRegion<BlendMode::Opaque>(sx, sy, sx2, sy2, dx, dy, scw, sch, *dest); break;
                        case BlendMode::Merge: canvas->scaleBlitRegion<BlendMode::Merge>(sx, sy, sx2, sy2, dx, dy, scw, sch, *dest); break;
                        case BlendMode::Preserve: canvas->scaleBlitRegion<BlendMode::Preserve>(sx, sy, sx2, sy2, dx, dy, scw, sch, *dest); break;
                        case BlendMode::Add: canvas->scaleBlitRegion<BlendMode::Add>(sx, sy, sx2, sy2, dx, dy, scw, sch, *dest); break;
                        case BlendMode::Subtract: canvas->scaleBlitRegion<BlendMode::Subtract>(sx, sy, sx2, sy2, dx, dy, scw, sch, *dest); break;
                    }
                    return 0;
                }},
                {"rotateBlitRegion", [](lua_State* L)
                {
                    auto canvas = script::ptr<Canvas>(L, 1);
                    auto sx = script::get<int>(L, 2);
                    auto sy = script::get<int>(L, 3);
                    auto sx2 = script::get<int>(L, 4);
                    auto sy2 = script::get<int>(L, 5);
                    auto dx = script::get<int>(L, 6);
                    auto dy = script::get<int>(L, 7);
                    auto angle = script::get<double>(L, 8);
                    auto dest = script::ptr<Canvas>(L, 9);
                    auto mode = BlendMode(script::get<int>(L, 10, int(BlendMode::Preserve)));

                    switch(mode)
                    {
                        case BlendMode::Opaque: canvas->rotateBlitRegion<BlendMode::Opaque>(sx, sy, sx2, sy2, dx, dy, angle, *dest); break;
                        case BlendMode::Merge: canvas->rotateBlitRegion<BlendMode::Merge>(sx, sy, sx2, sy2, dx, dy, angle, *dest); break;
                        case BlendMode::Preserve: canvas->rotateBlitRegion<BlendMode::Preserve>(sx, sy, sx2, sy2, dx, dy, angle, *dest); break;
                        case BlendMode::Add: canvas->rotateBlitRegion<BlendMode::Add>(sx, sy, sx2, sy2, dx, dy, angle, *dest); break;
                        case BlendMode::Subtract: canvas->rotateBlitRegion<BlendMode::Subtract>(sx, sy, sx2, sy2, dx, dy, angle, *dest); break;
                    }
                    return 0;
                }},
                {"rotateScaleBlitRegion", [](lua_State* L)
                {
                    auto canvas = script::ptr<Canvas>(L, 1);
                    auto sx = script::get<int>(L, 2);
                    auto sy = script::get<int>(L, 3);
                    auto sx2 = script::get<int>(L, 4);
                    auto sy2 = script::get<int>(L, 5);
                    auto dx = script::get<int>(L, 6);
                    auto dy = script::get<int>(L, 7);
                    auto angle = script::get<double>(L, 8);
                    auto scale = script::get<double>(L, 9);
                    auto dest = script::ptr<Canvas>(L, 10);
                    auto mode = BlendMode(script::get<int>(L, 11, int(BlendMode::Preserve)));
            
                    switch(mode)
                    {
                        case BlendMode::Opaque: canvas->rotateScaleBlitRegion<BlendMode::Opaque>(sx, sy, sx2, sy2, dx, dy, angle, scale, *dest); break;
                        case BlendMode::Merge: canvas->rotateScaleBlitRegion<BlendMode::Merge>(sx, sy, sx2, sy2, dx, dy, angle, scale, *dest); break;
                        case BlendMode::Preserve: canvas->rotateScaleBlitRegion<BlendMode::Preserve>(sx, sy, sx2, sy2, dx, dy, angle, scale, *dest); break;
                        case BlendMode::Add: canvas->rotateScaleBlitRegion<BlendMode::Add>(sx, sy, sx2, sy2, dx, dy, angle, scale, *dest); break;
                        case BlendMode::Subtract: canvas->rotateScaleBlitRegion<BlendMode::Subtract>(sx, sy, sx2, sy2, dx, dy, angle, scale, *dest); break;
                    }
                    return 0;
                }},
                {"get_trueWidth", [](lua_State* L)
                {
                    auto canvas = script::ptr<Canvas>(L, 1);
                    script::push(L, canvas->getTrueWidth());

                    return 1;
                }},
                {"get_trueHeight", [](lua_State* L)
                {
                    auto canvas = script::ptr<Canvas>(L, 1);
                    script::push(L, canvas->getTrueHeight());

                    return 1;
                }},
                {"get_width", [](lua_State* L)
                {
                    auto canvas = script::ptr<Canvas>(L, 1);
                    script::push(L, canvas->getWidth());

                    return 1;
                }},
                {"get_height", [](lua_State* L)
                {
                    auto canvas = script::ptr<Canvas>(L, 1);
                    script::push(L, canvas->getHeight());

                    return 1;
                }},
                {"get_opacity", [](lua_State* L)
                {
                    auto canvas = script::ptr<Canvas>(L, 1);
                    script::push(L, canvas->getOpacity());

                    return 1;
                }},
                {"set_opacity", [](lua_State* L)
                {
                    auto canvas = script::ptr<Canvas>(L, 1);
                    auto value = script::get<int>(L, 2);
                    canvas->setOpacity(value);

                    return 0;
                }},
                {nullptr, nullptr}
            };
            luaL_setfuncs(L, functions, 0);

            lua_pop(L, 1);

            // Push plum namespace.
            lua_getglobal(L, "plum");

            // plum[classname] = create
            script::push(L, "Canvas");
            lua_pushcfunction(L, [](lua_State* L)
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
            });
            lua_settable(L, -3);

            // Pop plum namespace.
            lua_pop(L, 1);

        }
    }
}
