#pragma once

namespace plum
{
    class Compression
    {
    public:
        // This stuff was shamelessly adapted from ika.
        static int compressData(const uint8_t* src, int srclen, uint8_t* dest, int destlen);
        static void decompressData(const uint8_t* src, int srclen, uint8_t* dest, int destlen);
    };
}