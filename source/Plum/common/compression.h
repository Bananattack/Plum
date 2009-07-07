#pragma once

namespace Plum
{
	class Compression
	{
	public:
		// This stuff was shamelessly adapted from ika.
		static int compressData(const u8* src, int srclen, u8* dest, int destlen);
		static void decompressData(const u8* src, int srclen, u8* dest, int destlen);
	};
}