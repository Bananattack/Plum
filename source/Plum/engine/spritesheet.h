#pragma once
namespace Plum
{
	class Spritesheet
	{
		private:
			Texture* texture;
		public:
			int frameWidth, frameHeight;
			int padding;
			int columns;

			Spritesheet(Canvas* canvas, int w, int h)
			{
				init(canvas, w, h);
			}

			Spritesheet(Texture* tex, int w, int h)
			{
				init(tex->getCanvas(), w, h);
			}
			
			~Spritesheet()
			{
				delete texture;
			}

		private:
			void init(Canvas* canvas, int w, int h)
			{
				texture = new Texture(canvas);
				frameWidth = w;
				frameHeight = h;
				padding = 0;
				columns = 1;
			}

		public:
			void bind()
			{
				texture->bind();
			}

			void blitFrame(int x, int y, int f, BlendMode mode = BlendUnspecified, Color tint = Color::White)
			{
				// Ensure there's always at least one column before looking up a frame
				if(columns == 0) columns = 1;

				int fx = (f % columns) * (frameWidth + padding) + padding;
				int fy = (f / columns) * (frameHeight + padding) + padding;
				texture->blitRegion(fx, fy, fx + frameWidth - 1, fy + frameHeight - 1,
						x, y, mode, tint);
			}

			void rawBlitFrame(int x, int y, int f, double angle, double scale)
			{
				// Ensure there's always at least one column before looking up a frame
				if(columns == 0) columns = 1;

				int fx = (f % columns) * (frameWidth + padding) + padding;
				int fy = (f / columns) * (frameHeight + padding) + padding;
				texture->rawBlitRegion(fx, fy, fx + frameWidth - 1, fy + frameHeight - 1,
						x, y, 0, 1);
			}
	};
}