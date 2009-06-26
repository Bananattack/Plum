#include "../plum.h"

namespace Plum
{
	namespace Script
	{
		static int video_getField(lua_State* L)
		{
			std::string fieldName = luaL_checkstring(L, 2);
			if(luaL_getmetafield(L, 1, std::string("get" + fieldName).c_str()))
			{
				lua_pushvalue(L, 1);
				lua_call(L, 1, 1);
				return 1;
			}
			return luaL_getmetafield(L, 1, fieldName.c_str());
		}

		static int video_setField(lua_State* L)
		{
			std::string fieldName = luaL_checkstring(L, 2);
			/* L, 3 is the value to set. */
			if(luaL_getmetafield(L, 1, std::string("set" + fieldName).c_str()))
			{
				lua_pushvalue(L, 1);
				lua_pushvalue(L, 3);
				lua_call(L, 2, 0);
				return 0;
			}
			if(luaL_getmetafield(L, 1, std::string("get" + fieldName).c_str()))
			{
				luaL_error(L, "Attempt to modify readonly field '%s' on plum_video.", fieldName.c_str());
				lua_pop(L, 1);
				return 0;
			}
			luaL_error(L, "Attempt to modify unknown field '%s' on plum_video.", fieldName.c_str());
			return 0;
		}


		static int video_toString(lua_State* L)
		{
			lua_pushstring(L, "(plum.video singleton)");
			return 1;
		}

		static int video_clear(lua_State* L)
		{
			Color color = luaL_checkint(L, 1);

			(*engine).video.clear(color);
			return 0;
		}

		static int video_rect(lua_State* L)
		{
			int x = luaL_checkint(L, 1);
			int y = luaL_checkint(L, 2);
			int x2 = luaL_checkint(L, 3);
			int y2 = luaL_checkint(L, 4);
			Color color = luaL_checkint(L, 5);
			BlendMode mode = (BlendMode) luaL_optint(L, 6, BlendUnspecified);

			(*engine).video.rect(x, y, x2, y2, color, mode);
			return 0;
		}

		static int video_solidRect(lua_State* L)
		{
			int x = luaL_checkint(L, 1);
			int y = luaL_checkint(L, 2);
			int x2 = luaL_checkint(L, 3);
			int y2 = luaL_checkint(L, 4);
			Color color = luaL_checkint(L, 5);
			BlendMode mode = (BlendMode) luaL_optint(L, 6, BlendUnspecified);

			(*engine).video.solidRect(x, y, x2, y2, color, mode);
			return 0;
		}

		static int video_line(lua_State* L)
		{
			int x = luaL_checkint(L, 1);
			int y = luaL_checkint(L, 2);
			int x2 = luaL_checkint(L, 3);
			int y2 = luaL_checkint(L, 4);
			Color color = luaL_checkint(L, 5);
			BlendMode mode = (BlendMode) luaL_optint(L, 6, BlendUnspecified);

			(*engine).video.line(x, y, x2, y2, color, mode);
			return 0;
		}

		static int video_horizontalGradientRect(lua_State* L)
		{
			int x = luaL_checkint(L, 1);
			int y = luaL_checkint(L, 2);
			int x2 = luaL_checkint(L, 3);
			int y2 = luaL_checkint(L, 4);
			Color color = luaL_checkint(L, 5);
			Color color2 = luaL_checkint(L, 6);
			BlendMode mode = (BlendMode) luaL_optint(L, 7, BlendUnspecified);

			(*engine).video.horizontalGradientRect(x, y, x2, y2, color, color2, mode);
			return 0;
		}

		static int video_verticalGradientRect(lua_State* L)
		{
			int x = luaL_checkint(L, 1);
			int y = luaL_checkint(L, 2);
			int x2 = luaL_checkint(L, 3);
			int y2 = luaL_checkint(L, 4);
			Color color = luaL_checkint(L, 5);
			Color color2 = luaL_checkint(L, 6);
			BlendMode mode = (BlendMode) luaL_optint(L, 7, BlendUnspecified);

			(*engine).video.verticalGradientRect(x, y, x2, y2, color, color2, mode);
			return 0;
		}


		static int video_circle(lua_State* L)
		{
			int x = luaL_checkint(L, 1);
			int y = luaL_checkint(L, 2);
			int xrad = luaL_checkint(L, 3);
			int yrad = luaL_checkint(L, 4);
			Color color = luaL_checkint(L, 5);
			BlendMode mode = (BlendMode) luaL_optint(L, 6, BlendUnspecified);

			(*engine).video.circle(x, y, xrad, yrad, color, mode);
			return 0;
		}

		static int video_solidCircle(lua_State* L)
		{
			int x = luaL_checkint(L, 1);
			int y = luaL_checkint(L, 2);
			int xrad = luaL_checkint(L, 3);
			int yrad = luaL_checkint(L, 4);
			Color color = luaL_checkint(L, 5);
			BlendMode mode = (BlendMode) luaL_optint(L, 6, BlendUnspecified);

			(*engine).video.solidCircle(x, y, xrad, yrad, color, mode);
			return 0;
		}

		static int video_getScreenWidth(lua_State* L)
		{
			lua_pushinteger(L, (*engine).video.getScreenWidth());
			return 1;
		}

		static int video_getScreenHeight(lua_State* L)
		{
			lua_pushinteger(L, (*engine).video.getScreenHeight());
			return 1;
		}

		static int video_getBlendMode(lua_State* L)
		{
			lua_pushinteger(L, getBlendMode());
			return 1;
		}

		static int video_setBlendMode(lua_State* L)
		{
			BlendMode mode = (BlendMode) luaL_checkint(L, 2);
			setBlendMode(mode);
			return 0;
		}

		static int video_getOpacity(lua_State* L)
		{
			lua_pushinteger(L, getOpacity());
			return 1;
		}

		static int video_setOpacity(lua_State* L)
		{
			int v = luaL_checkint(L, 2);
			setOpacity(v);
			return 0;
		}

		const luaL_Reg videoMembers[] = {
			{ "__index", video_getField },
			{ "__newindex", video_setField },
			{ "__tostring",	video_toString },

			{ "clear", video_clear },
			{ "rect", video_rect },
			{ "solidRect", video_solidRect },
			{ "circle", video_circle },
			{ "solidCircle", video_solidCircle },
			{ "horizontalGradientRect", video_horizontalGradientRect },
			{ "verticalGradientRect", video_verticalGradientRect },

			{ "getscreenWidth", video_getScreenWidth },
			{ "getscreenHeight", video_getScreenHeight },
			{ "getblendMode", video_getBlendMode },
			{ "setblendMode", video_setBlendMode },
			{ "getopacity", video_getOpacity },
			{ "setopacity", video_setOpacity },

			{ NULL, NULL }
		};

		void initVideoClass(lua_State* L)
		{
			// Load timer metatable
			luaL_newmetatable(L, "plum_video");
			// Duplicate the metatable on the stack.
			lua_pushvalue(L, -1);
			// metatable.__index = metatable
			lua_setfield(L, -2, "__index");
			// Put the members into the metatable.
			luaL_register(L, NULL, videoMembers);
			lua_pop(L, 1);

			// Push plum namespace.
			lua_getglobal(L, "plum");

			// Create video
			lua_newtable(L);
			lua_pushvalue(L, -1);
			lua_setfield(L, -3, "video");

			luaL_getmetatable(L, "plum_video");
			lua_setmetatable(L, -2);

			// Pop video.
			lua_pop(L, 1);


			// Pop plum namespace.
			lua_pop(L, 1);
		}
	}
}