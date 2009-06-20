#include "../plum.h"

namespace Plum
{
	Engine::Engine()
	{
		initialized = false;
	}

	void Engine::startup()
	{
		if(initialized)
		{
			return;
		}

		printf(">> Initializing...\n");

		printf("    Loading config settings...");
		config = Config("plum.cfg", "config");

		int xres = config.hasValue("xres") ? config.getIntValue("xres") : 320;
		int yres = config.hasValue("yres") ? config.getIntValue("yres") : 240;
		bool windowed = config.hasValue("windowed") ? config.getBoolValue("windowed") : true;

		printf(" OK!\n");
		printf("    (Settings: %dx%d resolution, %s mode)\n\n", xres, yres, windowed ? "windowed" : "fullscreen");

		printf("    Initializing SDL...");
		if(SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_NOPARACHUTE) != 0)
		{
			throw Engine::Exception("Couldn't initialize SDL.\n");
		}

		SDL_ShowCursor((config.hasValue("hide_cursor") && config.getBoolValue("hide_cursor")) ? SDL_DISABLE : SDL_ENABLE);

		// Create a nice *white* cursor, by inverting the data of each masked pixel.
		// Technically still not correct, if you actually HAD a black cursor before :/
		// I really wish SDL just let you get the system's "normal" cursor, instead of using this non-native UI shit.
		{
			SDL_Cursor* cursor = SDL_GetCursor();
			int size = cursor->area.w * cursor->area.h / 8;
			Uint8* data = new Uint8[size];
			Uint8* mask = new Uint8[size];

			for(int i = 0; i < size; i++)
			{
				// Copy default cursor!
				mask[i] = cursor->mask[i];
				data[i] = cursor->data[i];
				// Invert things with a mask!
				data[i] ^= (mask[i] & 1) | (mask[i] & 2) | (mask[i] & 4) | (mask[i] & 8)
							| (mask[i] & 16) | (mask[i] & 32) | (mask[i] & 64) | (mask[i] & 128);
			}

			mouseCursor = SDL_CreateCursor(data, mask, cursor->area.w, cursor->area.h, cursor->hot_x, cursor->hot_y);
			SDL_SetCursor(mouseCursor);
		}


		timer = Timer();
		printf(" OK!\n");

		printf("    Initializing video engine...");
		setTitle("Plum");
		video.startup();
		video.setResolution(xres, yres, windowed);
		printf(" OK!\n");

		printf("    Initializing sound engine...");
		audio.startup();
		printf(" OK!\n");

		printf("    Initializing scripting engine...");
		Script::startup(this);
		printf(" OK!\n");

		destroyed = false;
		initialized = true;
		printf(">> Initialization complete!\n\n");

	}

	void Engine::shutdown()
	{
		if(!initialized)
		{
			return;
		}
		printf("\n>> Destroying...\n");

		printf("    Destroying scripting engine...");
		Script::shutdown();
		printf(" OK!\n");

		printf("    Destroying sound engine...");
		audio.shutdown();
		printf(" OK!\n");

		printf("    Destroying video engine...");
		video.shutdown();
		printf(" OK!\n");

		printf("    Destroying SDL...");
		SDL_FreeCursor(mouseCursor);
		SDL_Quit();
		printf(" OK!\n");

		printf(">> Destroyed!\n");
		initialized = false;
	}

	void Engine::quit(std::string message) 
	{
		printf("\n>> Shutdown requested");
		if(message.length())
		{
			// If we're initialized enough, we can draw the error on-screen!
			if(initialized)
			{
			}

			printf(", with quit message:\n%s", message.c_str());
			fprintf(stderr, "%s", message.c_str());
#ifdef PLUM_WIN32
			SDL_SysWMinfo info;
			SDL_VERSION(&info.version);
			HWND hWnd = SDL_GetWMInfo(&info) ? info.window : HWND_DESKTOP;

			MessageBoxA(hWnd, message.c_str(), "Error", 0);
#endif
			shutdown();
			::exit(-1);
		}
		printf(".\n");
		shutdown();
		::exit(0);
	}

	void Engine::handleMouseButtonEvent(SDL_MouseButtonEvent e)
	{
		bool state = (e.type == SDL_MOUSEBUTTONDOWN ? true : false);
		// do something with event.button;
		switch(e.button)
		{
			case SDL_BUTTON_LEFT:
				mouse.button[MOUSE_LEFT].setPressed(state);
				break;
			case SDL_BUTTON_MIDDLE:
				mouse.button[MOUSE_MIDDLE].setPressed(state);
				break;
			case SDL_BUTTON_RIGHT:
				mouse.button[MOUSE_RIGHT].setPressed(state);
				break;
			case SDL_BUTTON_WHEELUP:
				if(state)
				{
					mouse.wheelPosition--;
				}
				break;
			case SDL_BUTTON_WHEELDOWN:
				if(state)
				{
					mouse.wheelPosition++;
				}
				break;
		}
	}

	void Engine::poll()
	{
		SDL_Event event;
		//ControlMap::iterator it;
		while (!destroyed && SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				/*case SDL_VIDEORESIZE:
					// I hate you SDL. Why do you nuke the GL context on resize events. You're a terrible person.
					if(windowed)
					{
						adjustSize(event.resize.w, event.resize.h, windowed);
					}
					break;*/
				case SDL_MOUSEMOTION:
					mouse.x = (double) event.motion.x * (double) video.getScreenWidth() / (double) video.getWindowWidth();
					mouse.y = (double) event.motion.y * (double) video.getScreenHeight() / (double) video.getWindowHeight();
				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEBUTTONUP:
					handleMouseButtonEvent(event.button);
					break;
				case SDL_KEYDOWN:
					key[event.key.keysym.sym].setPressed(true);
					break;
				case SDL_KEYUP:
					// do something with event.key;
					key[event.key.keysym.sym].setPressed(false);
					break;
				case SDL_QUIT:
					// Close the program
					quit();
					break;
				default:
					break;
			}
		}
	}

	void Engine::refresh()
	{
		poll();
		SDL_GL_SwapBuffers();

		timer.processInput(key[KEY_TILDE], key[KEY_LSHIFT]);
		if(key[KEY_TILDE].isPressed())
		{
			audio.setMasterPitch(1.0 * TIMER_FAST_MULTIPLIER);
		}
		else if(key[KEY_LSHIFT].isPressed())
		{
			audio.setMasterPitch(1.0 / TIMER_SLOW_DIVISOR);
		}
		else
		{
			audio.setMasterPitch(1.0);
		}
		audio.update();
		timer.update();

		//std::string cap = titlePrefix + " - FPS: " + integerToString(timer.fps);
		//SDL_WM_SetCaption(cap.c_str(), cap.c_str());

		if((key[KEY_LALT].isPressed() || key[KEY_RALT].isPressed()) && (key[KEY_F4].isPressed() || key[KEY_X].isPressed()))
		{
			quit();
		}
	}

	void Engine::setTitle(std::string title)
	{
		titlePrefix = title;
		SDL_WM_SetCaption(title.c_str(), title.c_str());
	}
}
