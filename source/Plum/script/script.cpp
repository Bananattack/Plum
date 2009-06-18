#include "../plum.h"

namespace Plum
{
	void Script::startup()
	{
		lua = luaL_newstate();
		luaL_openlibs(lua);
	}

	void Script::shutdown()
	{
		lua_close(lua);
	}

}