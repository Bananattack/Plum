#include "../plum.h"

namespace Plum
{
	namespace ScriptLibrary
	{
		namespace CanvasObject
		{
			SCRIPT_OBJ_GETTER(index, Wrapper<Canvas>*, libraryName)
			SCRIPT_OBJ_SETTER(newindex, Wrapper<Canvas>*, libraryName)

			int create(lua_State* L)
			{
				if(lua_isnumber(L, 1) && lua_isnumber(L, 2))
				{
					int w = lua_tointeger(L, 1);
					int h = lua_tointeger(L, 2);

					PLUM_PUSH_DATA(L, Canvas, new Canvas(w, h), LUA_NOREF);
					
					return 1;
				}
				else if(lua_isstring(L, 1))
				{
					const char* filename = lua_tostring(L, 1);

					PLUM_PUSH_DATA(L, Canvas, new Canvas(filename), LUA_NOREF);
					
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
				Wrapper<Canvas>* canvas = PLUM_CHECK_DATA(L, 1, Canvas);

				// Only delete if it doesn't belong to a parent of some sort.
				if(canvas->parentRef != LUA_NOREF)
				{
					delete canvas->data;
				}
				else
				{
					luaL_unref(L, LUA_REGISTRYINDEX, canvas->parentRef);
				}

				return 0;
			}

			int tostring(lua_State* L)
			{
				PLUM_CHECK_DATA(L, 1, Canvas);
				lua_pushstring(L, "(plum.Canvas object)");
				return 1;
			}

			int restoreClipRegion(lua_State* L)
			{
				Wrapper<Canvas>* canvas = PLUM_CHECK_DATA(L, 1, Canvas);
				canvas->data->restoreClipRegion();
				return 0;
			}

			int setClipRegion(lua_State* L)
			{
				Wrapper<Canvas>* canvas = PLUM_CHECK_DATA(L, 1, Canvas);
				int x = luaL_checkint(L, 2);
				int y = luaL_checkint(L, 3);
				int x2 = luaL_checkint(L, 4);
				int y2 = luaL_checkint(L, 5);
				canvas->data->setClipRegion(x, y, x2, y2);
				return 0;
			}

			int getPixel(lua_State* L)
			{
				Wrapper<Canvas>* canvas = PLUM_CHECK_DATA(L, 1, Canvas);
				int x = luaL_checkint(L, 2);
				int y = luaL_checkint(L, 3);
				lua_pushinteger(L, canvas->data->getPixel(x, y));
				return 1;
			}

			int setPixel(lua_State* L)
			{
				Wrapper<Canvas>* canvas = PLUM_CHECK_DATA(L, 1, Canvas);
				int x = luaL_checkint(L, 2);
				int y = luaL_checkint(L, 3);
				Color color = luaL_checkint(L, 4);
				BlendMode mode = (BlendMode) luaL_optint(L, 5, getBlendMode());

				switch(mode)
				{
					case BlendOpaque:
						canvas->data->setPixel<SoftOpaqueBlender>(x, y, color, SoftOpaqueBlender());
						break;
					case BlendMerge:
						canvas->data->setPixel<SoftMergeBlender>(x, y, color, SoftMergeBlender());
						break;
					case BlendPreserve:
						canvas->data->setPixel<SoftPreserveBlender>(x, y, color, SoftPreserveBlender());
						break;
					case BlendAdd:
						canvas->data->setPixel<SoftAddBlender>(x, y, color, SoftAddBlender());
						break;
					case BlendSubtract:
						canvas->data->setPixel<SoftSubtractBlender>(x, y, color, SoftSubtractBlender());
						break;
				}
				return 0;
			}

			int clear(lua_State* L)
			{
				Wrapper<Canvas>* canvas = PLUM_CHECK_DATA(L, 1, Canvas);
				Color color = luaL_checkint(L, 2);

				canvas->data->clear(color);
				
				return 0;
			}

			int replaceColor(lua_State* L)
			{
				Wrapper<Canvas>* canvas = PLUM_CHECK_DATA(L, 1, Canvas);
				int find = luaL_checkint(L, 2);
				int replace = luaL_checkint(L, 3);
				canvas->data->replaceColor(find, replace);
				return 0;
			}

			int flip(lua_State* L)
			{
				Wrapper<Canvas>* canvas = PLUM_CHECK_DATA(L, 1, Canvas);
				bool h = lua_toboolean(L, 2) != 0;
				bool v = lua_toboolean(L, 3) != 0;
				canvas->data->flip(h, v);
				return 0;
			}

			int line(lua_State* L)
			{
				Wrapper<Canvas>* canvas = PLUM_CHECK_DATA(L, 1, Canvas);
				int x = luaL_checkint(L, 2);
				int y = luaL_checkint(L, 3);
				int x2 = luaL_checkint(L, 4);
				int y2 = luaL_checkint(L, 5);
				Color color = luaL_checkint(L, 6);
				BlendMode mode = (BlendMode) luaL_optint(L, 7, getBlendMode());

				switch(mode)
				{
					case BlendOpaque:
						canvas->data->line<SoftOpaqueBlender>(x, y, x2, y2, color, SoftOpaqueBlender());
						break;
					case BlendMerge:
						canvas->data->line<SoftMergeBlender>(x, y, x2, y2, color, SoftMergeBlender());
						break;
					case BlendPreserve:
						canvas->data->line<SoftPreserveBlender>(x, y, x2, y2, color, SoftPreserveBlender());
						break;
					case BlendAdd:
						canvas->data->line<SoftAddBlender>(x, y, x2, y2, color, SoftAddBlender());
						break;
					case BlendSubtract:
						canvas->data->line<SoftSubtractBlender>(x, y, x2, y2, color, SoftSubtractBlender());
						break;
				}
				return 0;
			}

			int rect(lua_State* L)
			{
				Wrapper<Canvas>* canvas = PLUM_CHECK_DATA(L, 1, Canvas);
				int x = luaL_checkint(L, 2);
				int y = luaL_checkint(L, 3);
				int x2 = luaL_checkint(L, 4);
				int y2 = luaL_checkint(L, 5);
				Color color = luaL_checkint(L, 6);
				BlendMode mode = (BlendMode) luaL_optint(L, 7, getBlendMode());

				switch(mode)
				{
					case BlendOpaque:
						canvas->data->rect<SoftOpaqueBlender>(x, y, x2, y2, color, SoftOpaqueBlender());
						break;
					case BlendMerge:
						canvas->data->rect<SoftMergeBlender>(x, y, x2, y2, color, SoftMergeBlender());
						break;
					case BlendPreserve:
						canvas->data->rect<SoftPreserveBlender>(x, y, x2, y2, color, SoftPreserveBlender());
						break;
					case BlendAdd:
						canvas->data->rect<SoftAddBlender>(x, y, x2, y2, color, SoftAddBlender());
						break;
					case BlendSubtract:
						canvas->data->rect<SoftSubtractBlender>(x, y, x2, y2, color, SoftSubtractBlender());
						break;
				}
				return 0;
			}

			int solidRect(lua_State* L)
			{
				Wrapper<Canvas>* canvas = PLUM_CHECK_DATA(L, 1, Canvas);
				int x = luaL_checkint(L, 2);
				int y = luaL_checkint(L, 3);
				int x2 = luaL_checkint(L, 4);
				int y2 = luaL_checkint(L, 5);
				Color color = luaL_checkint(L, 6);
				BlendMode mode = (BlendMode) luaL_optint(L, 7, getBlendMode());

				switch(mode)
				{
					case BlendOpaque:
						canvas->data->solidRect<SoftOpaqueBlender>(x, y, x2, y2, color, SoftOpaqueBlender());
						break;
					case BlendMerge:
						canvas->data->solidRect<SoftMergeBlender>(x, y, x2, y2, color, SoftMergeBlender());
						break;
					case BlendPreserve:
						canvas->data->solidRect<SoftPreserveBlender>(x, y, x2, y2, color, SoftPreserveBlender());
						break;
					case BlendAdd:
						canvas->data->solidRect<SoftAddBlender>(x, y, x2, y2, color, SoftAddBlender());
						break;
					case BlendSubtract:
						canvas->data->solidRect<SoftSubtractBlender>(x, y, x2, y2, color, SoftSubtractBlender());
						break;
				}
				return 0;
			}

			int circle(lua_State* L)
			{
				Wrapper<Canvas>* canvas = PLUM_CHECK_DATA(L, 1, Canvas);
				int cx = luaL_checkint(L, 2);
				int cy = luaL_checkint(L, 3);
				int xRadius = luaL_checkint(L, 4);
				int yRadius = luaL_checkint(L, 5);
				Color color = luaL_checkint(L, 6);
				BlendMode mode = (BlendMode) luaL_optint(L, 7, getBlendMode());

				switch(mode)
				{
					case BlendOpaque:
						canvas->data->circle<SoftOpaqueBlender>(cx, cy, xRadius, yRadius, color, SoftOpaqueBlender());
						break;
					case BlendMerge:
						canvas->data->circle<SoftMergeBlender>(cx, cy, xRadius, yRadius, color, SoftMergeBlender());
						break;
					case BlendPreserve:
						canvas->data->circle<SoftPreserveBlender>(cx, cy, xRadius, yRadius, color, SoftPreserveBlender());
						break;
					case BlendAdd:
						canvas->data->circle<SoftAddBlender>(cx, cy, xRadius, yRadius, color, SoftAddBlender());
						break;
					case BlendSubtract:
						canvas->data->circle<SoftSubtractBlender>(cx, cy, xRadius, yRadius, color, SoftSubtractBlender());
						break;
				}
				return 0;
			}

			int solidCircle(lua_State* L)
			{
				Wrapper<Canvas>* canvas = PLUM_CHECK_DATA(L, 1, Canvas);
				int cx = luaL_checkint(L, 2);
				int cy = luaL_checkint(L, 3);
				int xRadius = luaL_checkint(L, 4);
				int yRadius = luaL_checkint(L, 5);
				Color color = luaL_checkint(L, 6);
				BlendMode mode = (BlendMode) luaL_optint(L, 7, getBlendMode());

				switch(mode)
				{
					case BlendOpaque:
						canvas->data->solidCircle<SoftOpaqueBlender>(cx, cy, xRadius, yRadius, color, SoftOpaqueBlender());
						break;
					case BlendMerge:
						canvas->data->solidCircle<SoftMergeBlender>(cx, cy, xRadius, yRadius, color, SoftMergeBlender());
						break;
					case BlendPreserve:
						canvas->data->solidCircle<SoftPreserveBlender>(cx, cy, xRadius, yRadius, color, SoftPreserveBlender());
						break;
					case BlendAdd:
						canvas->data->solidCircle<SoftAddBlender>(cx, cy, xRadius, yRadius, color, SoftAddBlender());
						break;
					case BlendSubtract:
						canvas->data->solidCircle<SoftSubtractBlender>(cx, cy, xRadius, yRadius, color, SoftSubtractBlender());
						break;
				}
				return 0;
			}

			int blit(lua_State* L)
			{
				Wrapper<Canvas>* canvas = PLUM_CHECK_DATA(L, 1, Canvas);
				int x = luaL_checkint(L, 2);
				int y = luaL_checkint(L, 3);
				Wrapper<Canvas>* dest = PLUM_CHECK_DATA(L, 4, Canvas);
				BlendMode mode = (BlendMode) luaL_optint(L, 5, getBlendMode());
				
				switch(mode)
				{
					case BlendOpaque:
						canvas->data->blit<SoftOpaqueBlender>(x, y, dest->data, SoftOpaqueBlender());
						break;
					case BlendMerge:
						canvas->data->blit<SoftMergeBlender>(x, y, dest->data, SoftMergeBlender());
						break;
					case BlendPreserve:
						canvas->data->blit<SoftPreserveBlender>(x, y, dest->data, SoftPreserveBlender());
						break;
					case BlendAdd:
						canvas->data->blit<SoftAddBlender>(x, y, dest->data, SoftAddBlender());
						break;
					case BlendSubtract:
						canvas->data->blit<SoftSubtractBlender>(x, y, dest->data, SoftSubtractBlender());
						break;
				}
				return 0;
			}

			int scaleBlit(lua_State* L)
			{
				Wrapper<Canvas>* canvas = PLUM_CHECK_DATA(L, 1, Canvas);
				int x = luaL_checkint(L, 2);
				int y = luaL_checkint(L, 3);
				int scaledWidth = luaL_checkint(L, 4);
				int scaledHeight = luaL_checkint(L, 5);
				Wrapper<Canvas>* dest = PLUM_CHECK_DATA(L, 6, Canvas);
				BlendMode mode = (BlendMode) luaL_optint(L, 7, getBlendMode());
				
				switch(mode)
				{
					case BlendOpaque:
						canvas->data->scaleBlit<SoftOpaqueBlender>(x, y, scaledWidth, scaledHeight, dest->data, SoftOpaqueBlender());
						break;
					case BlendMerge:
						canvas->data->scaleBlit<SoftMergeBlender>(x, y, scaledWidth, scaledHeight, dest->data, SoftMergeBlender());
						break;
					case BlendPreserve:
						canvas->data->scaleBlit<SoftPreserveBlender>(x, y, scaledWidth, scaledHeight, dest->data, SoftPreserveBlender());
						break;
					case BlendAdd:
						canvas->data->scaleBlit<SoftAddBlender>(x, y, scaledWidth, scaledHeight, dest->data, SoftAddBlender());
						break;
					case BlendSubtract:
						canvas->data->scaleBlit<SoftSubtractBlender>(x, y, scaledWidth, scaledHeight, dest->data, SoftSubtractBlender());
						break;
				}
				return 0;
			}

			int rotateBlit(lua_State* L)
			{
				Wrapper<Canvas>* canvas = PLUM_CHECK_DATA(L, 1, Canvas);
				int x = luaL_checkint(L, 2);
				int y = luaL_checkint(L, 3);
				double angle = luaL_checknumber(L, 4);
				Wrapper<Canvas>* dest = PLUM_CHECK_DATA(L, 5, Canvas);
				BlendMode mode = (BlendMode) luaL_optint(L, 6, getBlendMode());
				
				switch(mode)
				{
					case BlendOpaque:
						canvas->data->rotateBlit<SoftOpaqueBlender>(x, y, angle, dest->data, SoftOpaqueBlender());
						break;
					case BlendMerge:
						canvas->data->rotateBlit<SoftMergeBlender>(x, y, angle, dest->data, SoftMergeBlender());
						break;
					case BlendPreserve:
						canvas->data->rotateBlit<SoftPreserveBlender>(x, y, angle, dest->data, SoftPreserveBlender());
						break;
					case BlendAdd:
						canvas->data->rotateBlit<SoftAddBlender>(x, y, angle, dest->data, SoftAddBlender());
						break;
					case BlendSubtract:
						canvas->data->rotateBlit<SoftSubtractBlender>(x, y, angle, dest->data, SoftSubtractBlender());
						break;
				}
				return 0;
			}

			int rotateScaleBlit(lua_State* L)
			{
				Wrapper<Canvas>* canvas = PLUM_CHECK_DATA(L, 1, Canvas);
				int x = luaL_checkint(L, 2);
				int y = luaL_checkint(L, 3);
				double angle = luaL_checknumber(L, 4);
				double scale = luaL_checknumber(L, 5);
				Wrapper<Canvas>* dest = PLUM_CHECK_DATA(L, 6, Canvas);
				BlendMode mode = (BlendMode) luaL_optint(L, 7, getBlendMode());
				
				switch(mode)
				{
					case BlendOpaque:
						canvas->data->rotateScaleBlit<SoftOpaqueBlender>(x, y, angle, scale, dest->data, SoftOpaqueBlender());
						break;
					case BlendMerge:
						canvas->data->rotateScaleBlit<SoftMergeBlender>(x, y, angle, scale, dest->data, SoftMergeBlender());
						break;
					case BlendPreserve:
						canvas->data->rotateScaleBlit<SoftPreserveBlender>(x, y, angle, scale, dest->data, SoftPreserveBlender());
						break;
					case BlendAdd:
						canvas->data->rotateScaleBlit<SoftAddBlender>(x, y, angle, scale, dest->data, SoftAddBlender());
						break;
					case BlendSubtract:
						canvas->data->rotateScaleBlit<SoftSubtractBlender>(x, y, angle, scale, dest->data, SoftSubtractBlender());
						break;
				}
				return 0;
			}

			int blitRegion(lua_State* L)
			{
				Wrapper<Canvas>* canvas = PLUM_CHECK_DATA(L, 1, Canvas);
				int sx = luaL_checkint(L, 2);
				int sy = luaL_checkint(L, 3);
				int sx2 = luaL_checkint(L, 4);
				int sy2 = luaL_checkint(L, 5);
				int dx = luaL_checkint(L, 6);
				int dy = luaL_checkint(L, 7);
				Wrapper<Canvas>* dest = PLUM_CHECK_DATA(L, 8, Canvas);
				BlendMode mode = (BlendMode) luaL_optint(L, 9, getBlendMode());
				
				switch(mode)
				{
					case BlendOpaque:
						canvas->data->blitRegion<SoftOpaqueBlender>(sx, sy, sx2, sy2, dx, dy, dest->data, SoftOpaqueBlender());
						break;
					case BlendMerge:
						canvas->data->blitRegion<SoftMergeBlender>(sx, sy, sx2, sy2, dx, dy, dest->data, SoftMergeBlender());
						break;
					case BlendPreserve:
						canvas->data->blitRegion<SoftPreserveBlender>(sx, sy, sx2, sy2, dx, dy, dest->data, SoftPreserveBlender());
						break;
					case BlendAdd:
						canvas->data->blitRegion<SoftAddBlender>(sx, sy, sx2, sy2, dx, dy, dest->data, SoftAddBlender());
						break;
					case BlendSubtract:
						canvas->data->blitRegion<SoftSubtractBlender>(sx, sy, sx2, sy2, dx, dy, dest->data, SoftSubtractBlender());
						break;
				}
				return 0;
			}

			int scaleBlitRegion(lua_State* L)
			{
				Wrapper<Canvas>* canvas = PLUM_CHECK_DATA(L, 1, Canvas);
				int sx = luaL_checkint(L, 2);
				int sy = luaL_checkint(L, 3);
				int sx2 = luaL_checkint(L, 4);
				int sy2 = luaL_checkint(L, 5);
				int dx = luaL_checkint(L, 6);
				int dy = luaL_checkint(L, 7);
				int scw = luaL_checkint(L, 8);
				int sch = luaL_checkint(L, 9);
				Wrapper<Canvas>* dest = PLUM_CHECK_DATA(L, 10, Canvas);
				BlendMode mode = (BlendMode) luaL_optint(L, 11, getBlendMode());
				
				switch(mode)
				{
					case BlendOpaque:
						canvas->data->scaleBlitRegion<SoftOpaqueBlender>(sx, sy, sx2, sy2, dx, dy, scw, sch, dest->data, SoftOpaqueBlender());
						break;
					case BlendMerge:
						canvas->data->scaleBlitRegion<SoftMergeBlender>(sx, sy, sx2, sy2, dx, dy, scw, sch, dest->data, SoftMergeBlender());
						break;
					case BlendPreserve:
						canvas->data->scaleBlitRegion<SoftPreserveBlender>(sx, sy, sx2, sy2, dx, dy, scw, sch, dest->data, SoftPreserveBlender());
						break;
					case BlendAdd:
						canvas->data->scaleBlitRegion<SoftAddBlender>(sx, sy, sx2, sy2, dx, dy, scw, sch, dest->data, SoftAddBlender());
						break;
					case BlendSubtract:
						canvas->data->scaleBlitRegion<SoftSubtractBlender>(sx, sy, sx2, sy2, dx, dy, scw, sch, dest->data, SoftSubtractBlender());
						break;
				}
				return 0;
			}

			int rotateBlitRegion(lua_State* L)
			{
				Wrapper<Canvas>* canvas = PLUM_CHECK_DATA(L, 1, Canvas);
				int sx = luaL_checkint(L, 2);
				int sy = luaL_checkint(L, 3);
				int sx2 = luaL_checkint(L, 4);
				int sy2 = luaL_checkint(L, 5);
				int dx = luaL_checkint(L, 6);
				int dy = luaL_checkint(L, 7);
				double angle = luaL_checkint(L, 8);
				Wrapper<Canvas>* dest = PLUM_CHECK_DATA(L, 9, Canvas);
				BlendMode mode = (BlendMode) luaL_optint(L, 10, getBlendMode());
				
				switch(mode)
				{
					case BlendOpaque:
						canvas->data->rotateBlitRegion<SoftOpaqueBlender>(sx, sy, sx2, sy2, dx, dy, angle, dest->data, SoftOpaqueBlender());
						break;
					case BlendMerge:
						canvas->data->rotateBlitRegion<SoftMergeBlender>(sx, sy, sx2, sy2, dx, dy, angle, dest->data, SoftMergeBlender());
						break;
					case BlendPreserve:
						canvas->data->rotateBlitRegion<SoftPreserveBlender>(sx, sy, sx2, sy2, dx, dy, angle, dest->data, SoftPreserveBlender());
						break;
					case BlendAdd:
						canvas->data->rotateBlitRegion<SoftAddBlender>(sx, sy, sx2, sy2, dx, dy, angle, dest->data, SoftAddBlender());
						break;
					case BlendSubtract:
						canvas->data->rotateBlitRegion<SoftSubtractBlender>(sx, sy, sx2, sy2, dx, dy, angle, dest->data, SoftSubtractBlender());
						break;
				}
				return 0;
			}

			int rotateScaleBlitRegion(lua_State* L)
			{
				Wrapper<Canvas>* canvas = PLUM_CHECK_DATA(L, 1, Canvas);
				int sx = luaL_checkint(L, 2);
				int sy = luaL_checkint(L, 3);
				int sx2 = luaL_checkint(L, 4);
				int sy2 = luaL_checkint(L, 5);
				int dx = luaL_checkint(L, 6);
				int dy = luaL_checkint(L, 7);
				double angle = luaL_checkint(L, 8);
				double scale = luaL_checkint(L, 9);
				Wrapper<Canvas>* dest = PLUM_CHECK_DATA(L, 10, Canvas);
				BlendMode mode = (BlendMode) luaL_optint(L, 11, getBlendMode());
				
				switch(mode)
				{
					case BlendOpaque:
						canvas->data->rotateScaleBlitRegion<SoftOpaqueBlender>(sx, sy, sx2, sy2, dx, dy, angle, scale, dest->data, SoftOpaqueBlender());
						break;
					case BlendMerge:
						canvas->data->rotateScaleBlitRegion<SoftMergeBlender>(sx, sy, sx2, sy2, dx, dy, angle, scale, dest->data, SoftMergeBlender());
						break;
					case BlendPreserve:
						canvas->data->rotateScaleBlitRegion<SoftPreserveBlender>(sx, sy, sx2, sy2, dx, dy, angle, scale, dest->data, SoftPreserveBlender());
						break;
					case BlendAdd:
						canvas->data->rotateScaleBlitRegion<SoftAddBlender>(sx, sy, sx2, sy2, dx, dy, angle, scale, dest->data, SoftAddBlender());
						break;
					case BlendSubtract:
						canvas->data->rotateScaleBlitRegion<SoftSubtractBlender>(sx, sy, sx2, sy2, dx, dy, angle, scale, dest->data, SoftSubtractBlender());
						break;
				}
				return 0;
			}

			int gettrueWidth(lua_State* L)
			{
				Wrapper<Canvas>* canvas = PLUM_CHECK_DATA(L, 1, Canvas);
				lua_pushnumber(L, canvas->data->width);

				return 1;
			}

			int gettrueHeight(lua_State* L)
			{
				Wrapper<Canvas>* canvas = PLUM_CHECK_DATA(L, 1, Canvas);
				lua_pushnumber(L, canvas->data->height);

				return 1;
			}

			int getwidth(lua_State* L)
			{
				Wrapper<Canvas>* canvas = PLUM_CHECK_DATA(L, 1, Canvas);
				lua_pushnumber(L, canvas->data->occupiedWidth);

				return 1;
			}

			int getheight(lua_State* L)
			{
				Wrapper<Canvas>* canvas = PLUM_CHECK_DATA(L, 1, Canvas);
				lua_pushnumber(L, canvas->data->occupiedHeight);

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
				PLUM_BIND_META(index)
				PLUM_BIND_META(newindex)
				PLUM_BIND_META(tostring)
				PLUM_BIND_FUNC(restoreClipRegion)
				PLUM_BIND_FUNC(setClipRegion)
				PLUM_BIND_FUNC(getPixel)
				PLUM_BIND_FUNC(setPixel)
				PLUM_BIND_FUNC(clear)
				PLUM_BIND_FUNC(flip)
				PLUM_BIND_FUNC(replaceColor)
				PLUM_BIND_FUNC(line)
				PLUM_BIND_FUNC(rect)
				PLUM_BIND_FUNC(solidRect)
				PLUM_BIND_FUNC(circle)
				PLUM_BIND_FUNC(solidCircle)
				PLUM_BIND_FUNC(blit)
				PLUM_BIND_FUNC(scaleBlit)
				PLUM_BIND_FUNC(rotateBlit)
				PLUM_BIND_FUNC(rotateScaleBlit)
				PLUM_BIND_FUNC(blitRegion)
				PLUM_BIND_FUNC(scaleBlitRegion)
				PLUM_BIND_FUNC(rotateBlitRegion)
				PLUM_BIND_FUNC(rotateScaleBlitRegion)
				PLUM_BIND_FUNC(gettrueWidth)
				PLUM_BIND_FUNC(gettrueHeight)
				PLUM_BIND_FUNC(getwidth)
				PLUM_BIND_FUNC(getheight)
				PLUM_BIND_FUNC_END_NULL()

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
}