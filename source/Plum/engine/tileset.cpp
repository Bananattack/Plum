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
			// Width of image being stored, so it knows how big to inflate when loading later.
			fprintf(f, "        width = %d;\n", img->width);

			// Height of image being stored, so it knows how big to inflate when loading later.
			fprintf(f, "        height = %d;\n", img->height);

			// Allocate buffer.
			u8* buffer = new u8[img->width * img->height * sizeof(Color)];
			// Compress.
			logFormat("Compressing from %d bytes...\n", img->width * img->height * sizeof(Color));
			int compressedSize = Compression::compressData((u8*)(img->data), img->width * img->height, buffer, img->width * img->height);
			logFormat("Compressed to %d bytes!\n", compressedSize);
			// Encode.
			logFormat("Coercing...\n");
			std::string blob = std::string(buffer, buffer + compressedSize);
			logFormat("Encoding...\n");
			std::string plainText = Base64::encode(blob);
			logFormat("Encoded!\n");
			// Write.
			fprintf(f, "    data = '%s';\n", plainText.c_str());
			// Destroy buffer.
			//delete buffer;

			fprintf(f, "    };\n");
		}
/*
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

			// Allocate buffer.
			u8* buffer = new u8[img->width * img->height * sizeof(Color)];
			// Compress.
			int compressedSize = Compression::compressData(reinterpret_cast<const u8*>(img->data), img->width * img->height, buffer, img->width * img->height);
			// Encode.
			std::string plainText = Base64::encode(std::string(buffer, buffer + compressedSize));
			// Write.
			fprintf(f, "    data = '%s';\n", plainText.c_str());
			// Destroy buffer.
			delete buffer;

			fprintf(f, "    };\n");
		}
*/
		// Done!
		fprintf(f, "};\n");
		fclose(f);
	}
}