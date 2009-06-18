#include "../plum.h"

#include "../plum.h"

namespace Plum
{
	namespace Script
	{
		static int video_clear(lua_State* L)
		{
			Color color = luaL_checkint(L, 1);

			engine->video.clear(color);
			return 0;
		}

		static int video_solidRect(lua_State* L)
		{
			int x = luaL_checkint(L, 1);
			int y = luaL_checkint(L, 1);
			int x2 = luaL_checkint(L, 1);
			int y2 = luaL_checkint(L, 1);
			Color color = luaL_checkint(L, 5);
			BlendMode mode = (BlendMode) luaL_optint(L, 5, BlendUnspecified);

			engine->video.solidRect(x, y, x2, y2, color, mode);
			return 0;
		}

		void initVideoClass(lua_State* L)
		{
			// Push plum namespace.
			lua_getglobal(L, "plum");

			// Create video namespace
			lua_newtable(lua);
			lua_pushvalue(lua, -1);
			lua_setfield(lua, -3, "video");

			lua_pushcfunction(lua, video_clear);
			lua_setfield(lua, -2, "clear");
			lua_pushcfunction(lua, video_solidRect);
			lua_setfield(lua, -2, "solidRect");

			// Pop plum namespace.
			lua_pop(L, -1);
		}
	}
}