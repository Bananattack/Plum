#include "plum.h"

void RunGame(Plum::Engine& engine)
{
	unsigned int i;
	Plum::Sound* sound = engine.audio.loadSound("resources/sounds/shot.wav");
	Plum::Font* fnt = new Plum::Font("resources/fonts/ccfont.png");
	Plum::Song* song = NULL;

	double x = 0;
	double y = 0;
	Plum::Sprite* sprite = new Plum::Sprite(x, y, "resources/sprites/heartsprite.sprite");
	sprite->scale = 3.0;


	fnt->enableVariableWidth();
	sprite->setAnimation("throb", "left");

	//printf("SONG = %s\n", engine.config.getStringValue("song").c_str());
	song = engine.audio.loadSong(engine.config.getStringValue("song"));

	engine.audio.playSong(song);

	while(!engine.key[Plum::KEY_ESCAPE].isPressed())
	{
		for(i = 0; i < 9; i++)
		{
			sprite->x = x + i * 16;
			sprite->y = y;
			sprite->blit();
		}
		
		fnt->print(5, 5, "FPS: " + Plum::integerToString(engine.timer.fps));

		engine.refresh();
		engine.video.clear(Plum::rgb(0, 102, 255));
		engine.video.solidRect(0, 0, engine.video.getScreenWidth(), engine.video.getScreenHeight(), Plum::rgb(0x33, 0x66, 0xcc));

		for(i = 0; i < engine.timer.gap; i++)
		{
			sprite->angle += 0.2;
			if(engine.key[Plum::KEY_LEFT].isPressed())
			{
				x -= 0.5;
			}
			if(engine.key[Plum::KEY_RIGHT].isPressed())
			{
				x += 0.5;
			}
			if(engine.key[Plum::KEY_UP].isPressed())
			{
				y -= 0.5;
			}
			if(engine.key[Plum::KEY_DOWN].isPressed())
			{
				y += 0.5;
			}
			if(engine.key[Plum::KEY_ENTER].isPressed())
			{
				engine.audio.playSound(sound);
				engine.key[Plum::KEY_ENTER].setPressed(false);
			}
			sprite->update();
		}
	}
	delete sprite;
}

int main(int argc, char** argv)
{
	Plum::Engine engine;

	freopen("stdout.log", "w", stdout);
	freopen("stderr.log", "w", stderr);
	try
	{
		engine.startup();
	}
	catch(Plum::Audio::Exception& e)
	{
		engine.quit("Failure during audio system startup: " + std::string(e.what()));
	}
	catch(Plum::Video::Exception& e)
	{
		engine.quit("Failure during video system startup: " + std::string(e.what()));
	}
	catch(Plum::Engine::Exception& e)
	{
		engine.quit("Failure during internal startup: " + std::string(e.what()));
	}
	catch(std::exception& e)
	{
		engine.quit("Very unexpected error during startup: " + std::string(e.what()));
	}

	try
	{
		//RunGame(engine);
		Plum::Script::runScript("system.lua");
	}
	catch(std::exception& e)
	{
		engine.quit("Error " + std::string(e.what()));
	}
	engine.quit();

	return 0;
}
