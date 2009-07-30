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

		logFormat(">> Initializing...\r\n");

		logFormat("    Loading config settings...");
		config.init("plum.cfg", "config");

		int xres = config.hasValue("xres") ? config.getIntValue("xres") : 320;
		int yres = config.hasValue("yres") ? config.getIntValue("yres") : 240;
		bool windowed = config.hasValue("windowed") ? config.getBoolValue("windowed") : true;

		logFormat(" OK!\r\n");
		logFormat("    (Settings: %dx%d resolution, %s mode)\r\n\r\n", xres, yres, windowed ? "windowed" : "fullscreen");

		logFormat("    Initializing SDL...");
		if(SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_NOPARACHUTE) != 0)
		{
			throw Engine::Exception("Couldn't initialize SDL.\r\n");
		}

#ifdef PLUM_WIN32
        {
            SDL_SysWMinfo info;
            SDL_VERSION(&info.version);
            HWND hWnd = SDL_GetWMInfo(&info) ? info.window : 0;
            if (hWnd)
			{
                DWORD successful = SetClassLong(hWnd, GCL_HICON, (long)LoadIcon(GetModuleHandle(0), "APPICON"));
			}
        }
#endif

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
		logFormat(" OK!\r\n");

		logFormat("    Initializing video engine...");
		setTitle("Plum");
		video.startup();
		video.setResolution(xres, yres, windowed);
		logFormat(" OK!\r\n");

		logFormat("    Initializing sound engine...");
		audio.startup();
		logFormat(" OK!\r\n");

		logFormat("    Initializing scripting engine...");
		script.startup(this);
		logFormat(" OK!\r\n");

		destroyed = false;
		initialized = true;
		logFormat(">> Initialization complete!\r\n\r\n");

	}

	void Engine::shutdown()
	{
		if(!initialized)
		{
			logFormat("\r\n>> Shutdown before program was fully initialized, probably means fatal errors. Uh oh!\r\n");
			return;
		}
		logFormat("\r\n>> Destroying...\r\n");

		logFormat("    Destroying scripting engine...");
		script.shutdown();
		logFormat(" OK!\r\n");

		logFormat("    Destroying sound engine...");
		audio.shutdown();
		logFormat(" OK!\r\n");

		logFormat("    Destroying video engine...");
		video.shutdown();
		logFormat(" OK!\r\n");

		logFormat("    Destroying SDL...");
		SDL_FreeCursor(mouseCursor);
		SDL_Quit();
		logFormat(" OK!\r\n");

		logFormat(">> Destroyed!\r\n");
		initialized = false;
	}

	void Engine::quit(std::string message) 
	{
		logFormat("\r\n>> Shutdown requested");
		if(message.length())
		{
			// If we're initialized enough, we can draw the error on-screen!
			if(initialized)
			{
			}

			logFormat(", with quit message:\r\n%s", message.c_str());
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
		logFormat(".\r\n");
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
					break;
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


		script.stepGarbageCollector();
		//std::string cap = titlePrefix + " - FPS: " + integerToString(timer.fps);
		//SDL_WM_SetCaption(cap.c_str(), cap.c_str());

		// Update the input hooks, one by one.
		// The input hook wrapper stuff makes me sick inside somewhat.
		for(u32 i = 0; i < script.inputHooks.size(); i++)
		{
			script.processInputHook(script.inputHooks[i]);
		}

		if((key[KEY_LALT].isPressed() || key[KEY_RALT].isPressed()) && (key[KEY_F4].isPressed() || key[KEY_X].isPressed()))
		{
			quit();
		}
		if(timer.gap == 0)
		{
			SDL_Delay(10);
		}
	}

	void Engine::setTitle(std::string title)
	{
		titlePrefix = title;
		SDL_WM_SetCaption(title.c_str(), title.c_str());
	}
}
