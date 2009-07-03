#ifndef PLUM_SPRITE_H
#define PLUM_SPRITE_H

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

			Sprite(double x, double y, const char* filename);
			Sprite(double x, double y, std::string filename);
			~Sprite();
		private:
			void init(double x, double y, std::string filename, lua_State* state = 0);
		public:
			Texture* getTexture();

			void blit();
			void setAnimation(std::string strand, std::string direction);
			void update();

			void saveAs(std::string filename);
			void saveAs(const char* filename);
	};
}

#endif