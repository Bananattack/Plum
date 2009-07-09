#include "plum.h"

using namespace Plum;

#ifdef PLUM_WIN32
	static FARPROC WINAPI failHook(unsigned /* dliNotify */, PDelayLoadInfo pdli)
	{
		throw Plum::Engine::Exception("Error encountered in Plum.\r\n"
				"Seems like '" + std::string(pdli->szDll) + "' might be missing or corrupted.\r\n"
				+ "Please get a working version, and try again.\r\n");
		return 0;
	}
#endif

int main(int argc, char** argv)
{
#ifdef PLUM_WIN32
	__pfnDliFailureHook2 = failHook;
#endif

	ScriptInstanceMap map;
	Engine engine;

	clearLog();
	freopen("stdout.log", "w", stdout);
	freopen("stderr.log", "w", stderr);
	try
	{
		initScriptInstances(&map);

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

	Image* image = new Image("tileset.png");
	Image* obs = new Image("obstile.png");
	Tileset* tileset = new Tileset(16, image, obs);
	tileset->save("test.tileset");

	delete tileset;
	delete image;
	delete obs;

	tileset = new Plum::Tileset("test.tileset");

	try
	{
		while(!engine.key[KEY_ENTER].isPressed())
		{
			engine.video.clear(Color::Black);
			tileset->tiles->blit(0, 0);
			engine.refresh();
		}
		engine.script.runScript("system.lua");
	}
	catch(std::exception& e)
	{
		engine.quit("Error " + std::string(e.what()));
	}
	engine.quit();

	return 0;
}
