#ifndef PLUM_VIDEO_H
#define PLUM_VIDEO_H

namespace Plum
{
	class Video
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
			bool windowed;

			int windowWidth, windowHeight;
			int xres, yres;
			int desktopWidth, desktopHeight;

			SDL_Surface* frontSurface;
		public:
			Video();

			void startup();
			void shutdown();

			void setResolution(int width, int height, bool win);

			int getScreenWidth()
			{
				return xres;
			}

			int getScreenHeight()
			{
				return yres;
			}

			int getWindowWidth()
			{
				return windowWidth;
			}

			int getWindowHeight()
			{
				return windowHeight;
			}

			void clear(Color color);
			void setPixel(int x, int y, Color color, BlendMode mode = BlendUnspecified);
			void line(int x, int y, int x2, int y2, Color color, BlendMode mode = BlendUnspecified);
			void rect(int x, int y, int x2, int y2, Color color, BlendMode mode = BlendUnspecified);
			void solidRect(int x, int y, int x2, int y2, Color color, BlendMode mode = BlendUnspecified);
			void horizontalGradientRect(int x, int y, int x2, int y2, Color color, Color color2, BlendMode mode = BlendUnspecified);
			void verticalGradientRect(int x, int y, int x2, int y2, Color color, Color color2, BlendMode mode = BlendUnspecified);
			void circle(int x, int y, int horizontalRadius, int verticalRadius, Color color, BlendMode mode = BlendUnspecified);
			void solidCircle(int x, int y, int horizontalRadius, int verticalRadius, Color color, BlendMode mode = BlendUnspecified);
	};
}

#endif
