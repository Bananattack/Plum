#pragma once

namespace Plum
{
	class Config
	{
		public:
			lua_State* lua;
			lua_State* parentThread;
			int ref;
			std::string filename;

			Config()
				: lua(0), parentThread(0), ref(LUA_NOREF), filename("") {}
			~Config();

			void init(const std::string& name, const std::string& blockName, lua_State* state = 0);
			void checkInitialized();
			bool hasValue(const std::string& key);
			bool getBoolValue(const std::string& key);
			int getIntValue(const std::string& key);
			std::string getStringValue(const std::string& key);
	};
}

