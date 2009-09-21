#pragma once
namespace Plum
{
	class Texture
	{
		private:
			inline static int nearestPowerOfTwo(int num)
			{
				if (num <= 0)
				{
					return 0;
				}
				else
				{
					num--;
					num |= num >> 1;
					num |= num >> 2;
					num |= num >> 4;
					num |= num >> 8;
					num |= num >> 16;
					num++;

					return num;
				}
			}

			// A backend software image that this raw texture copies.
			// Useful if the textures need to be reloaded later.
			Image* textureImage;
			// The GL target type
			int target; 
			// The GL texture ID
			GLuint textureID;
			// The width and height of the image
			int imageWidth, imageHeight;
			// The width and height of the texture (usually is a power of two)
			int textureWidth, textureHeight;
			// The ratio of image{Width|Height} / texture{Width|Height}
			double widthRatio, heightRatio;

			void setImageWidth(int width);
			void setImageHeight(int height);
			void setTextureWidth(int width);
			void setTextureHeight(int height);
		public:
			Texture(const std::string& filename);
			Texture(const char* filename);
			Texture(Image* image);

			~Texture();

			void init(const char* filename);
			void init(Image* image);

			int getImageWidth();
			int getImageHeight();
			int getTextureWidth();
			int getTextureHeight();
			double getWidthRatio();
			double getHeightRatio();
			Image* getImage();

			void updateRatios();
			void refresh();

			void bind();

			void blit(int x, int y, BlendMode mode = BlendUnspecified, Color tint = Color::White);
			void scaleBlit(int x, int y, int width, int height, BlendMode mode = BlendUnspecified, Color tint = Color::White);
			void rotateBlit(int x, int y, double angle, BlendMode mode = BlendUnspecified, Color tint = Color::White);
			void rotateScaleBlit(int x, int y, double angle, double scale, BlendMode mode = BlendUnspecified, Color tint = Color::White);

			void blitRegion(int sourceX, int sourceY, int sourceX2, int sourceY2,
					int destX, int destY, BlendMode mode = BlendUnspecified, Color tint = Color::White);
			void scaleBlitRegion(int sourceX, int sourceY, int sourceX2, int sourceY2,
					int destX, int destY, int scaledWidth, int scaledHeight, BlendMode mode = BlendUnspecified, Color tint = Color::White);
			void rotateBlitRegion(int sourceX, int sourceY, int sourceX2, int sourceY2,
					int destX, int destY, double angle, BlendMode mode = BlendUnspecified, Color tint = Color::White);
			void rotateScaleBlitRegion(int sourceX, int sourceY, int sourceX2, int sourceY2,
					int destX, int destY, double angle, double scale, BlendMode mode = BlendUnspecified, Color tint = Color::White);
			void rawBlitRegion(int sourceX, int sourceY, int sourceX2, int sourceY2,
					int destX, int destY, double angle, double scale);
	};
}
