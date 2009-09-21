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

			Spritesheet(Image* img, int w, int h)
			{
				init(img, w, h);
			}

			Spritesheet(Texture* tex, int w, int h)
			{
				init(tex->getImage(), w, h);
			}
			
			~Spritesheet()
			{
				delete texture;
			}

		private:
			void init(Image* img, int w, int h)
			{
				texture = new Texture(img);
				frameWidth = w;
				frameHeight = h;
				padding = 0;
				columns = 0;
			}

		public:
			void bind()
			{
				texture->bind();
			}

			void blitFrame(int x, int y, int f, BlendMode mode = BlendUnspecified, Color tint = Color::White)
			{
				int fx = (f % columns) * (frameWidth + padding) + padding;
				int fy = (f / columns) * (frameHeight + padding) + padding;
				texture->blitRegion(fx, fy, fx + frameWidth - 1, fy + frameHeight - 1,
						x, y, mode, tint);
			}

			void rawBlitFrame(int x, int y, int f, double angle, double scale)
			{
				int fx = (f % columns) * (frameWidth + padding) + padding;
				int fy = (f / columns) * (frameHeight + padding) + padding;
				texture->rawBlitRegion(fx, fy, fx + frameWidth - 1, fy + frameHeight - 1,
						x, y, 0, 1);
			}
	};
}