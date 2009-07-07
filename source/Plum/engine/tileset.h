#pragma once
namespace Plum
{
	class Tileset
	{
		public:
			int tileSize;
			Texture* tiles;
			Texture* obs;
			Config config;

			// For importing purposes.
			Tileset(int tileSize, Image* tiles, Image* obs);
			// For normal stuff.
			Tileset(const std::string& filename);
			~Tileset();

			void save(const std::string& filename);
	};
}