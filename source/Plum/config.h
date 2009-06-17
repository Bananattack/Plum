#ifndef PLUM_CONFIG_H
#define PLUM_CONFIG_H

namespace Plum
{
	class Config
	{
		private:
			lua_State* lua;
			std::string filename;
		public:
			Config::Config()
				: lua(0), filename("") {}
			Config(std::string name);

			void checkInitialized();
			bool hasValue(std::string key);
			bool getBoolValue(std::string key);
			int getIntValue(std::string key);
			std::string getStringValue(std::string key);
	};
}

#endif