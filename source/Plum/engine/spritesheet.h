#pragma once
namespace Plum
{
	class Spritesheet
	{
		private:
			Image* image;
		public:
			int frameWidth, frameHeight;
			int padding;
			int columns;

			Spritesheet(Canvas* canvas, int w, int h)
			{
				init(canvas, w, h);
			}

			Spritesheet(Image* img, int w, int h)
			{
				init(img->getCanvas(), w, h);
			}
			
			~Spritesheet()
			{
				delete image;
			}

		private:
			void init(Canvas* canvas, int w, int h)
			{
				image = new Image(canvas);
				frameWidth = w;
				frameHeight = h;
				padding = 0;
				columns = 1;
			}

		public:
			void bind()
			{
				image->bind();
			}

			void blitFrame(int x, int y, int f, BlendMode mode = BlendUnspecified, Color tint = Color::White)
			{
				// Ensure there's always at least one column before looking up a frame
				if(columns == 0) columns = 1;

				int fx = (f % columns) * (frameWidth + padding) + padding;
				int fy = (f / columns) * (frameHeight + padding) + padding;
				image->blitRegion(fx, fy, fx + frameWidth - 1, fy + frameHeight - 1,
						x, y, mode, tint);
			}

			void rawBlitFrame(int x, int y, int f, double angle, double scale)
			{
				// Ensure there's always at least one column before looking up a frame
				if(columns == 0) columns = 1;

				int fx = (f % columns) * (frameWidth + padding) + padding;
				int fy = (f / columns) * (frameHeight + padding) + padding;
				image->rawBlitRegion(fx, fy, fx + frameWidth - 1, fy + frameHeight - 1,
						x, y, 0, 1);
			}
	};
}