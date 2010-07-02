#include "plum.h"

using namespace Plum;

int main(int argc, char** argv)
{
	Engine engine;

	clearLog();
//	freopen("stdout.log", "w", stdout);
//	freopen("stderr.log", "w", stderr);
	try
	{
		engine.startup();
	}
	catch(Audio::Exception& e)
	{
		engine.quit("Failure during audio system startup: " + std::string(e.what()));
	}
	catch(Video::Exception& e)
	{
		engine.quit("Failure during video system startup: " + std::string(e.what()));
	}
	catch(Engine::Exception& e)
	{
		engine.quit("Failure during internal startup: " + std::string(e.what()));
	}
	catch(std::exception& e)
	{
		engine.quit("Very unexpected error during startup: " + std::string(e.what()));
	}

	try
	{
		engine.script.runScript("system.lua");
	}
	catch(std::exception& e)
	{
		engine.quit(e.what());
	}
	engine.quit();

	return 0;
}
