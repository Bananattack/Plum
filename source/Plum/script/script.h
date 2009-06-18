#ifndef PLUM_SCRIPT_H
#define PLUM_SCRIPT_H

namespace Plum
{
	class Script
	{
		private:
			lua_State* lua;
		public:
			void startup();
			void shutdown();
	};
}

#endif