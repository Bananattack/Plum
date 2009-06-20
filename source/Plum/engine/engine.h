#ifndef PLUM_ENGINE_H
#define PLUM_ENGINE_H

namespace Plum
{
	class Engine
	{
		public:
			class Exception : public std::exception
			{
				public:
					Exception(const std::string& message)
						: msg(message)
					{
					}

					virtual const char* what() const throw ()
					{
						return msg.c_str();
					}

					virtual ~Exception() throw ()
					{
					}

				private:
					std::string msg;
			};

		private:
			std::string titlePrefix;

			int xres, yres;
			int windowWidth, windowHeight;
			int desktopWidth, desktopHeight;
			bool windowed;
			bool initialized;
			bool destroyed;

			SDL_Surface* frontSurface;
			SDL_Cursor* mouseCursor;
		public:
			Config config;
			Video video;
			Audio audio;
			Timer timer;
			KeyInput key[KEY_MAX_COUNT];
		public:
			Engine();

			void startup();
			void shutdown();
			void quit(std::string message = "");

			void poll();
			void refresh();

			int getScreenWidth()
			{
				return xres;
			}

			int getScreenHeight()
			{
				return yres;
			}

			void setTitle(std::string title);
			void setResolution(int width, int height, bool windowed);

			FMOD::System* getSoundSystem();
	};
}

#endif
