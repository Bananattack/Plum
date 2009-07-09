#include "../plum.h"

namespace Plum
{
	// For importing purposes.
	Tileset::Tileset(int tileSize, Image* tiles, Image* obs)
	{
		this->tileSize = tileSize;
		this->tiles = new Texture(tiles);
		this->obs = new Texture(obs);
	}

	Tileset::Tileset(const std::string& filename)
	{
		// NOTE: Needs to take lua_State* at some point so it can spawn a thread for config instead of full-on state.
		config.init(filename, "tileset");

		tileSize = config.getIntValue("tileSize");
		
		lua_pushstring(config.lua, "tiles");
		lua_rawget(config.lua, -2);
		if(lua_istable(config.lua, -1))
		{
			// Texture Width
			lua_pushstring(config.lua, "width");
			lua_rawget(config.lua, -2);
			if(!lua_isnumber(config.lua, -1))
			{
				throw Engine::Exception("Error while loading " + filename + ":\r\nTileset's 'tiles' block has no 'width' attribute.");
			}
			int width = lua_tointeger(config.lua, -1);
			lua_pop(config.lua, 1);

			// Texture Height
			lua_pushstring(config.lua, "height");
			lua_rawget(config.lua, -2);
			if(!lua_isnumber(config.lua, -1))
			{
				throw Engine::Exception("Error while loading " + filename + ":\r\nTileset's 'tiles' block has no 'height' attribute.");
			}
			int height = lua_tointeger(config.lua, -1);
			lua_pop(config.lua, 1);			


			// Occupied Width
			lua_pushstring(config.lua, "occupiedWidth");
			lua_rawget(config.lua, -2);
			if(!lua_isnumber(config.lua, -1))
			{
				throw Engine::Exception("Error while loading " + filename + ":\r\nTileset's 'tiles' block has no 'occupiedWidth' attribute.");
			}
			int occupiedWidth = lua_tointeger(config.lua, -1);
			lua_pop(config.lua, 1);

			// Occupied Height
			lua_pushstring(config.lua, "occupiedHeight");
			lua_rawget(config.lua, -2);
			if(!lua_isnumber(config.lua, -1))
			{
				throw Engine::Exception("Error while loading " + filename + ":\r\nTileset's 'tiles' block has no 'occupiedHeight' attribute.");
			}
			int occupiedHeight = lua_tointeger(config.lua, -1);
			lua_pop(config.lua, 1);


			// Data. The funky stuff.
			lua_pushstring(config.lua, "data");
			lua_rawget(config.lua, -2);
			if(!lua_isstring(config.lua, -1))
			{
				throw Engine::Exception("Error while loading " + filename + ":\r\nTileset's 'tiles' block has no 'data' attribute.");
			}
			// Decode.
			std::string blob = Base64::decode(lua_tostring(config.lua, -1));
			// Pop.
			lua_pop(config.lua, 1);
			// Allocate temporary image.
			Image* img = new Image(width, height);
			img->occupiedWidth = occupiedWidth;
			img->occupiedHeight = occupiedHeight;
			// Decompress.
			Compression::decompressData((u8*) blob.data(), blob.length(), (u8*)(img->data), width * height * sizeof(Color));
			// Make texture.
			tiles = new Texture(img);
			// Destroy temporary image.
			delete img;
		}
		else
		{
			throw Engine::Exception("Error while loading " + filename + ":\r\nTileset's doesn't have a valid 'tiles' block.");
		}
		lua_pop(config.lua, 1);

		lua_pushstring(config.lua, "obs");
		lua_rawget(config.lua, -2);
		if(lua_istable(config.lua, -1))
		{
			// Texture Width
			lua_pushstring(config.lua, "width");
			lua_rawget(config.lua, -2);
			if(!lua_isnumber(config.lua, -1))
			{
				throw Engine::Exception("Error while loading " + filename + ":\r\nTileset's 'obs' block has no 'width' attribute.");
			}
			int width = lua_tointeger(config.lua, -1);
			lua_pop(config.lua, 1);

			// Texture Height
			lua_pushstring(config.lua, "height");
			lua_rawget(config.lua, -2);
			if(!lua_isnumber(config.lua, -1))
			{
				throw Engine::Exception("Error while loading " + filename + ":\r\nTileset's 'obs' block has no 'height' attribute.");
			}
			int height = lua_tointeger(config.lua, -1);
			lua_pop(config.lua, 1);			


			// Occupied Width
			lua_pushstring(config.lua, "occupiedWidth");
			lua_rawget(config.lua, -2);
			if(!lua_isnumber(config.lua, -1))
			{
				throw Engine::Exception("Error while loading " + filename + ":\r\nTileset's 'obs' block has no 'occupiedWidth' attribute.");
			}
			int occupiedWidth = lua_tointeger(config.lua, -1);
			lua_pop(config.lua, 1);

			// Occupied Height
			lua_pushstring(config.lua, "occupiedHeight");
			lua_rawget(config.lua, -2);
			if(!lua_isnumber(config.lua, -1))
			{
				throw Engine::Exception("Error while loading " + filename + ":\r\nTileset's 'obs' block has no 'occupiedHeight' attribute.");
			}
			int occupiedHeight = lua_tointeger(config.lua, -1);
			lua_pop(config.lua, 1);


			// Data. The funky stuff.
			lua_pushstring(config.lua, "data");
			lua_rawget(config.lua, -2);
			if(!lua_isstring(config.lua, -1))
			{
				throw Engine::Exception("Error while loading " + filename + ":\r\nTileset's 'obs' block has no 'data' attribute.");
			}
			// Decode.
			std::string blob = Base64::decode(lua_tostring(config.lua, -1));
			// Pop.
			lua_pop(config.lua, 1);
			// Allocate temporary image.
			Image* img = new Image(width, height);
			img->occupiedWidth = occupiedWidth;
			img->occupiedHeight = occupiedHeight;
			// Decompress.
			Compression::decompressData((u8*) blob.data(), blob.length(), (u8*)(img->data), width * height * sizeof(Color));
			// Make texture.
			obs = new Texture(img);
			// Destroy temporary image.
			delete img;
		}
		else
		{
			throw Engine::Exception("Error while loading " + filename + ":\r\nTileset's doesn't have a valid 'obs' block.");
		}
		lua_pop(config.lua, 1);
	}

	Tileset::~Tileset()
	{
		delete tiles;
		delete obs;
	}

	void Tileset::save(const std::string& filename)
	{
		// Alright! Let's rock!
		FILE* f = fopen(filename.c_str(), "wb");

		// ...Maybe not?
		if(!f)
		{
			return;	
		}

		// Header.
		fprintf(f, "tileset {\n");
		// (Required) Version number. Engine might attempt upwards compatibility with old versions.
		fprintf(f, "    version = 0.01;\n");
		// (Required) Size of each tile in pixels. Assumes tiles have square dimensions.
		fprintf(f, "    tileSize = %d;\n", tileSize);

		// (Required) Tile image data, which are the things that the player sees when exploring a map.
		//
		// It contains a zlib-compressed array of pixels, which is base64 encoded so it can be stored in plain-text.
		// 
		// Treats as an image which is might be comprised of many tileSize*tileSize regions.
		// It assumes the image has the maximum amount of tiles that can be fit within a similar image of that size,
		// so that no space is wasted.
		//
		// The image MUST have dimensions that are multiples of the tileSize or terrible things will happen.
		//
		// Since a tileset holds a texture, it may save unused pixels that are were necessary to obey power-of-two texture rules.
		{
			fprintf(f, "    tiles = {\n");

			Image* img = tiles->getImage();
			// Physical width of image being stored, so it knows how big to inflate when loading later.
			// This is the texture size.
			fprintf(f, "        width = %d;\n", img->width);

			// Physical height of image being stored, so it knows how big to inflate when loading later.
			// This is the texture size.
			fprintf(f, "        height = %d;\n", img->height);

			// Actually occupied width of image being stored, so it knows how much of it was actually used later.
			fprintf(f, "        occupiedWidth = %d;\n", img->occupiedWidth);

			// Actually occupied width of image being stored, so it knows how much of it was actually used later.
			fprintf(f, "        occupiedHeight = %d;\n", img->occupiedHeight);

			// Allocate buffer.
			u8* buffer = new u8[img->width * img->height * sizeof(Color)];
			// Compress.
			int compressedSize = Compression::compressData((u8*)(img->data),
				img->width * img->height * sizeof(Color),
				buffer,
				img->width * img->height * sizeof(Color)
			);
			// Encode.
			std::string blob = std::string(buffer, buffer + compressedSize);
			std::string plainText = Base64::encode(blob);
			// Write.
			fprintf(f, "        data = '%s';\n", plainText.c_str());
			// Destroy buffer.
			delete buffer;

			fprintf(f, "    };\n");
		}
		// (Required) Obstruction data, which denotes pixel patterns which are invisible to the player but mask areas that can't be walked on.
		//
		// It contains a zlib-compressed array of pixels, which is base64 encoded so it can be stored in plain-text.
		// 
		// Treats as an image which is might be comprised of many tileSize*tileSize regions.
		// It assumes the image has the maximum amount of tiles that can be fit within a similar image of that size,
		// so that no space is wasted.
		//
		// The image MUST have dimensions that are multiples of the tileSize or terrible things will happen.
		//
		// Since a tileset holds a texture, it may save unused pixels that are were necessary to obey power-of-two texture rules.
		{
			fprintf(f, "    obs = {\n");

			Image* img = obs->getImage();
			// Width of image being stored, so it knows how big to inflate when loading later.
			fprintf(f, "        width = %d;\n", img->width);

			// Height of image being stored, so it knows how big to inflate when loading later.
			fprintf(f, "        height = %d;\n", img->height);

			// Actually occupied width of image being stored, so it knows how much of it was actually used later.
			fprintf(f, "        occupiedWidth = %d;\n", img->occupiedWidth);

			// Actually occupied width of image being stored, so it knows how much of it was actually used later.
			fprintf(f, "        occupiedHeight = %d;\n", img->occupiedHeight);

			// Allocate buffer.
			u8* buffer = new u8[img->width * img->height * sizeof(Color)];
			// Compress.
			int compressedSize = Compression::compressData((u8*)(img->data), img->width * img->height, buffer, img->width * img->height);
			// Encode.
			std::string blob = std::string(buffer, buffer + compressedSize);
			std::string plainText = Base64::encode(blob);
			// Write.
			fprintf(f, "        data = '%s';\n", plainText.c_str());
			// Destroy buffer.
			delete buffer;

			fprintf(f, "    };\n");
		}
		// Done!
		fprintf(f, "};\n");
		fclose(f);
	}
}