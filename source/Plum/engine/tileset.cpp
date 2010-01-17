#include "../plum.h"

namespace Plum
{
	// For importing purposes.
	Tileset::Tileset(int tileSize, Canvas* tiles, Canvas* obs)
	{
		this->tileSize = tileSize;
		this->tiles = new Image(tiles);
		this->obs = new Image(obs);
	}

	Tileset::Tileset(const std::string& filename, lua_State* state)
	{
		bool modified = false;
		// NOTE: Needs to take lua_State* at some point so it can spawn a thread for config instead of full-on state.
		config.init(filename, "tileset", state);

		tileSize = config.getIntValue("tileSize");
		
		lua_pushstring(config.lua, "tiles");
		lua_rawget(config.lua, -2);

		logFormat("Loading tileset '%s'...\r\n", filename.c_str());
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

			// (Optional) externalFile is an attribute that allows tilesets to automatically be
			// re-imported on engine staturp.
			bool update = false;
			lua_pushstring(config.lua, "externalFile");
			lua_rawget(config.lua, -2);
			if(lua_isstring(config.lua, -1))
			{
				std::string externalFile = lua_tostring(config.lua, -1);
				lua_pop(config.lua, 1);

				// Get the last hash value of the external file, so we can compare changes.
				lua_pushstring(config.lua, "lastHash");
				lua_rawget(config.lua, -2);
				std::string lastHash = lua_isstring(config.lua, -1) ? Base64::decode(lua_tostring(config.lua, -1)) : "";


				externalTileFile = externalFile;
				externalTileHash = lastHash;
				// Try to calculate the SHA1 hash of the external image resource.
				// If the image file doesn't exist, just stop.
				std::string hash;
				if(SHA1::digestFile(externalFile, hash))
				{
					logFormat("Comparing tile hashes...\r\n");
					// Compare the difference with the last stored hash.
					// And if they don't match, then update the images being used.
					if(lastHash.length() != 20 || hash.compare(0, 20, lastHash, 0, 20) != 0)
					{
						logFormat("Out of date. Reloading external file.\r\n");
						this->tiles = new Image(externalFile);
						modified = true;
						update = true;
						externalTileHash = hash;
					}
				}
			}
			lua_pop(config.lua, 1);

			// If we didn't update from an external source,
			// Use the data stored in the file instead.
			if(!update)
			{
				logFormat("Using most recent tile copy available.\r\n");
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
				Canvas* canvas = new Canvas(width, height);
				canvas->occupiedWidth = occupiedWidth;
				canvas->occupiedHeight = occupiedHeight;
				// Decompress.
				Compression::decompressData((u8*) blob.data(), blob.length(), (u8*)(canvas->data), width * height * sizeof(Color));
				// Make texture.
				tiles = new Image(canvas);
				// Destroy temporary image.
				delete canvas;
			}
		}
		else
		{
			throw Engine::Exception("Error while loading " + filename + ":\r\nTileset doesn't have a valid 'tiles' block.");
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


			// (Optional) externalFile is an attribute that allows tilesets to automatically be
			// re-imported on engine staturp.
			bool update = false;
			lua_pushstring(config.lua, "externalFile");
			lua_rawget(config.lua, -2);
			if(lua_isstring(config.lua, -1))
			{
				std::string externalFile = lua_tostring(config.lua, -1);
				lua_pop(config.lua, 1);

				// Get the last hash value of the external file, so we can compare changes.
				lua_pushstring(config.lua, "lastHash");
				lua_rawget(config.lua, -2);
				std::string lastHash = lua_isstring(config.lua, -1) ? Base64::decode(lua_tostring(config.lua, -1)) : "";

				externalObsFile = externalFile;
				externalObsHash = lastHash;
				// Try to calculate the SHA1 hash of the external image resource.
				// If the image file doesn't exist, just stop.
				std::string hash;
				if(SHA1::digestFile(externalFile, hash))
				{
					logFormat("Comparing obs hashes...\r\n");
					// Compare the difference with the last stored hash.
					// And if they don't match, then update the images being used.
					if(lastHash.length() != 20 || hash.compare(0, 20, lastHash, 0, 20) != 0)
					{
						logFormat("Out of date. Reloading external file.\r\n");
						this->obs = new Image(externalFile);
						modified = true;
						update = true;

						externalObsHash = hash;
					}
				}
			}
			lua_pop(config.lua, 1);

			// If we didn't update from an external source,
			// Use the data stored in the file instead.
			if(!update)
			{
				logFormat("Using most recent obs copy available.\r\n");

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
				// Allocate temporary canvas.
				Canvas* canvas = new Canvas(width, height);
				canvas->occupiedWidth = occupiedWidth;
				canvas->occupiedHeight = occupiedHeight;
				// Decompress.
				Compression::decompressData((u8*) blob.data(), blob.length(), (u8*)(canvas->data), width * height * sizeof(Color));
				// Make image.
				obs = new Image(canvas);
				// Destroy temporary canvas.
				delete canvas;
			}
		}
		else
		{
			throw Engine::Exception("Error while loading " + filename + ":\r\nTileset doesn't have a valid 'obs' block.");
		}
		lua_pop(config.lua, 1);

		logFormat("Done loading.\r\n");


		if(modified)
		{
			logFormat("Saving tileset modifications...\r\n");
			save(filename);
			logFormat("Saved.\r\n");
		}
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
		// Since a tileset holds an image (which is a hardware texture), it may save unused pixels that are were necessary to obey power-of-two texture rules.
		{
			fprintf(f, "    tiles = {\n");

			Canvas* canvas = tiles->getCanvas();
			// Physical width of image being stored, so it knows how big to inflate when loading later.
			// This is the texture size.
			fprintf(f, "        width = %d;\n", canvas->width);

			// Physical height of image being stored, so it knows how big to inflate when loading later.
			// This is the texture size.
			fprintf(f, "        height = %d;\n", canvas->height);

			// Actually occupied width of image being stored, so it knows how much of it was actually used later.
			fprintf(f, "        occupiedWidth = %d;\n", canvas->occupiedWidth);

			// Actually occupied width of image being stored, so it knows how much of it was actually used later.
			fprintf(f, "        occupiedHeight = %d;\n", canvas->occupiedHeight);

			// If we have loaded from an external file, we can store that in the tileset, so the engine can autoupdate.
			if(externalTileFile.length() > 0)
			{
				fprintf(f, "        externalFile = '%s';\n", externalTileFile.c_str());
				fprintf(f, "        lastHash = '%s';\n", Base64::encode(externalTileHash).c_str());
			}

			// Allocate buffer.
			u8* buffer = new u8[canvas->width * canvas->height * sizeof(Color)];
			// Compress.
			int compressedSize = Compression::compressData((u8*)(canvas->data),
				canvas->width * canvas->height * sizeof(Color),
				buffer,
				canvas->width * canvas->height * sizeof(Color)
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
		// Since a tileset holds an image (which is a hardware texture), it may save unused pixels that are were necessary to obey power-of-two texture rules.
		{
			fprintf(f, "    obs = {\n");

			Canvas* canvas = obs->getCanvas();
			// Width of image being stored, so it knows how big to inflate when loading later.
			fprintf(f, "        width = %d;\n", canvas->width);

			// Height of image being stored, so it knows how big to inflate when loading later.
			fprintf(f, "        height = %d;\n", canvas->height);

			// Actually occupied width of image being stored, so it knows how much of it was actually used later.
			fprintf(f, "        occupiedWidth = %d;\n", canvas->occupiedWidth);

			// Actually occupied width of image being stored, so it knows how much of it was actually used later.
			fprintf(f, "        occupiedHeight = %d;\n", canvas->occupiedHeight);

			// If we have loaded from an external file, we can store that in the tileset, so the engine can autoupdate.
			if(externalObsFile.length() > 0)
			{
				fprintf(f, "        externalFile = '%s';\n", externalObsFile.c_str());
				fprintf(f, "        lastHash = '%s';\n", Base64::encode(externalObsHash).c_str());
			}

			// Allocate buffer.
			u8* buffer = new u8[canvas->width * canvas->height * sizeof(Color)];
			// Compress.
			int compressedSize = Compression::compressData((u8*)(canvas->data), canvas->width * canvas->height, buffer, canvas->width * canvas->height);
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