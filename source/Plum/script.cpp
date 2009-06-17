#include "plum.h"

namespace Plum
{
	void Script::startup()
	{
		lua = lua_open();
		luaL_openlibs(lua);
	}

	void Script::shutdown()
	{
		lua_close(lua);
	}

}