#include "plum.h"

void RunGame(Plum::Engine& engine)
{
	unsigned int i;
	Plum::Texture* screen = new Plum::Texture(new Plum::Image(320, 240));
	Plum::Texture* tex = new Plum::Texture("scrribblytest.png");
	Plum::Sound* sound = engine.audio.loadSound("shot.wav");
	Plum::Font* fnt = new Plum::Font("ccfont.png");
	Plum::Song* song = NULL;
	Plum::Sprite* sprite = new Plum::Sprite(0.0, 0.0, "heartsprite.sprite");
	sprite->scale = 3.0;
	double x = 0;
	double y = 0;

	fnt->enableVariableWidth();
	sprite->setAnimation("throb", "left");

	printf("SONG = %s\n", engine.config.getStringValue("song").c_str());
	song = engine.audio.loadSong(engine.config.getStringValue("song"));

	engine.audio.playSong(song);

	while(!engine.key[Plum::KEY_ESCAPE].isPressed())
	{
		//tex->getImage()->blitRegion(16, 0, 16 + 15, 15, x, y, screen->getImage());
		//tex->getImage()->scaleBlitRegion(16, 0, 16 + 15, 15, x, y, 64, 64, screen->getImage());
		//tex->getImage()->scaleBlit(x, y, 128, 128, screen->getImage());
		//tex->rotateScaleBlitRegion(16, 0, 16 + 15, 15, x, y, counter, 1);

		for(i = 0; i < 9; i++)
		{
			sprite->x = x + i * 16;
			sprite->y = y;
			sprite->blit();
		}
		
		fnt->print(5, 5, "FPS: " + Plum::integerToString(engine.timer.fps));
		//screen->refresh();
		//screen->blit(0, 0);
		//tex->blit(x, y);
		engine.refresh();
		//screen->getImage()->clear(Plum::rgb(255, 0, 255));
		engine.video.clear(Plum::rgb(0, 102, 255));
		engine.video.solidRect(0, 0, engine.getScreenWidth(), engine.getScreenHeight(), Plum::rgb(0x33, 0x66, 0xcc));
		//engine->setTitle("Test (FPS: " + IntegerToString(engine->timer->fps) + ", GAP: " + IntegerToString(engine->timer->gap) + ")");

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
				//song->play();
				engine.key[Plum::KEY_ENTER].setPressed(false);
			}
			if(engine.key[Plum::KEY_Q].isPressed())
			{
				engine.quit("Wooo.");
			}
			sprite->update();
		}
	}
	delete tex;
}

int main(int argc, char** argv)
{
	Plum::Engine engine;

//	freopen("stdout.log", "w", stdout);
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
		RunGame(engine);
	}
	catch(std::exception& e)
	{
		engine.quit(e.what());
	}
	engine.quit();

	return 0;
}
