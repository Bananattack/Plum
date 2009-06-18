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
			{ "solidRect", video_solidRect },
			{ "getscreenWidth", video_getScreenWidth },
			{ "getscreenHeight", video_getScreenHeight },
			{ "getblendMode", video_getBlendMode },
			{ "setblendMode", video_setBlendMode },
			{ "getOpacity", video_getOpacity },
			{ "setOpacity", video_setOpacity },

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

			// Create video namespace
			lua_newtable(L);
			lua_pushvalue(L, -1);
			lua_setfield(L, -3, "video");

			luaL_getmetatable(L, "plum_video");
			lua_setmetatable(L, -2);

			// Pop timer namespace.
			lua_pop(L, -1);


			// Pop plum namespace.
			lua_pop(L, -1);
		}
	}
}