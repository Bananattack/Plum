#ifndef PLUM_SCRIPT_H
#define PLUM_SCRIPT_H

namespace Plum
{
	namespace Script
	{
		extern lua_State* lua;
		extern Engine* engine;

		void startup(Engine* eng);
		void runScript();
		void shutdown();
	}
}

#endif