#ifndef PLUM_CONFIG_H
#define PLUM_CONFIG_H

namespace Plum
{
	class Config
	{
		public:
			lua_State* lua;
			lua_State* parentThread;
			int ref;
			std::string filename;

			Config::Config()
				: lua(0), parentThread(0), filename(""), ref(LUA_NOREF) {}
			~Config();

			void init(std::string name, std::string blockName, lua_State* state = 0);
			void checkInitialized();
			bool hasValue(std::string key);
			bool getBoolValue(std::string key);
			int getIntValue(std::string key);
			std::string getStringValue(std::string key);
	};
}

#endif