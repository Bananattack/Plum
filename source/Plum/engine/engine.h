#pragma once

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

			bool initialized;
			bool destroyed;

			SDL_Cursor* mouseCursor;
		public:
			Script script;
			Config config;
			Video video;
			Audio audio;
			Timer timer;
			Mouse mouse;
			KeyInput key[KEY_MAX_COUNT];
		public:
			Engine();

			void startup();
			void shutdown();
			void quit(std::string message = "");

			void handleMouseButtonEvent(SDL_MouseButtonEvent e);
			void poll();
			void refresh();

			void setTitle(std::string title);
			void setResolution(int width, int height, bool windowed);
	};
}
