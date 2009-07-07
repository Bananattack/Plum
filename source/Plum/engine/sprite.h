#pragma once

namespace Plum
{
	const int SPRITE_VERSION = 0x00000101;
	class Sprite
	{
		private:
			Texture* texture;
			AnimationParser* parser;
			std::map<std::string, AnimationInfo*> animationInfo;
		public:
			Config cfg;

			int frameWidth, frameHeight;
			int frameColumns;
			int framePadding;
			int hotspotX, hotspotY;
			int hotspotWidth, hotspotHeight;

			int renderPriority;
			double x, y;
			double angle;
			double scale;
			std::string direction;
			bool visible;
			int frameIndex;
			int alpha;
			BlendMode blendMode;
			Color tint;

			Sprite(double x, double y, const char* filename, lua_State* state = 0);
			Sprite(double x, double y, const std::string& filename, lua_State* state = 0);
			~Sprite();
		private:
			void init(double x, double y, const std::string& filename, lua_State* state = 0);
		public:
			Texture* getTexture()
			{
				return texture;
			}

			void blit();
			void setAnimation(std::string strand, std::string direction);
			void update();

			void saveAs(std::string filename);
			void saveAs(const char* filename);
	};
}