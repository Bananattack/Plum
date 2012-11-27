#include "../plum.h"

namespace plum
{
    // This stuff was shamelessly adapted from ika.

    int Compression::compressData(const uint8_t* src, int srclen, uint8_t* dest, int destlen)
    {
        z_stream stream;

        stream.next_in=(Bytef*)src;
        stream.avail_in = srclen;
        stream.next_out=(Bytef*)dest;
        stream.avail_out = destlen;
        stream.data_type = Z_BINARY;

        stream.zalloc = NULL;
        stream.zfree = NULL;

        deflateInit(&stream, Z_DEFAULT_COMPRESSION);
        deflate(&stream, Z_SYNC_FLUSH);
        deflateEnd(&stream);

        return stream.total_out;
        // TODO: error returns
    }

    void Compression::decompressData(const uint8_t* src, int srclen, uint8_t* dest, int destlen)
    {
        z_stream stream;

        stream.next_in=(Bytef*)src;
        stream.avail_in = srclen;
        stream.next_out=(Bytef*)dest;
        stream.avail_out = destlen;
        stream.data_type = Z_BINARY;

        stream.zalloc = NULL;
        stream.zfree = NULL;

        inflateInit(&stream);
        inflate(&stream, Z_SYNC_FLUSH);
        inflateEnd(&stream);

        // TODO: error returns
    }
}