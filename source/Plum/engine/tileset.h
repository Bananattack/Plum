#pragma once
namespace Plum
{
	class Tileset
	{
		public:
			std::string externalTileFile, externalTileHash;
			std::string externalObsFile, externalObsHash;

			int tileSize;
			Image* tiles;
			Image* obs;
			Config config;

			// For importing purposes.
			Tileset(int tileSize, Canvas* tiles, Canvas* obs);
			// For normal stuff.
			Tileset(const std::string& filename, lua_State* state = 0);
			~Tileset();

			void save(const std::string& filename);
	};
}