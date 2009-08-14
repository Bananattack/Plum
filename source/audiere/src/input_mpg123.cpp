/*
  Heavily modified code originally by Matt Campbell <mattcampbell@pobox.com> (was based on libavcodec from ffmpeg (http://ffmpeg.sourceforge.net/))
  This code relies on libmpg123 (http://www.mpg123.de/api/ - lGPL 2.1), modifications by Jason A. Petrasko.
*/

#include <string.h>
#include "input_mp3.h"
#include "utility.h"
#include "debug.h"

#ifdef NO_MPAUDEC

namespace audiere {


    bool MP3InputStream::mpg123_initialized = false;

  MP3InputStream::MP3InputStream()
  {
    m_eof = false;

    m_channel_count = 2;
    m_sample_rate = 44100;
    m_sample_format = SF_S16;

    m_input_position = 0;
    m_input_length = 0;
    m_decode_buffer = 0;
    m_first_frame = true;

    m_seekable = false;
    m_length = 0;
    m_position = 0;

    m_decoder_text = "mp3:mpg123";

    if (!mpg123_initialized)
    {
        mpg123_init();
        mpg123_initialized = true;
    }
    mh = NULL;
  }


  MP3InputStream::~MP3InputStream()
  {
    delete[] m_decode_buffer;
    if (mh)
    {
        mpg123_close(mh);
        mpg123_delete(mh);
    }
  }


  bool MP3InputStream::initialize(FilePtr file)
  {
    m_file = file;
    m_seekable = m_file->seek(0, File::END);
    m_file->seek(0, File::BEGIN);
    m_eof = false;

    mh = mpg123_new(NULL,NULL);
    if (mpg123_open_feed(mh) != MPG123_OK)
    {
        return false;
    }

    m_input_position = 0;
    m_input_length = 0;
    m_decode_buffer = new u8[4608]; // as I have no reason to set a specific value, I used the one from MPaudec
    if (!m_decode_buffer) return false;
    m_first_frame = true;

    if (m_seekable)
    {
      // Scan the file to determine the length.
      mpg123_scan(mh);
    }

    // this should fill in the audio format if it isn't set already
    return decodeFrame();
  }

  bool MP3InputStream::isSeekable() { return m_seekable; }
  int MP3InputStream::getPosition() { return m_position; }

  void MP3InputStream::setPosition(int position)
  {
    if (!m_seekable || position > m_length) return;
    int scan_position = 0;
    int target_frame = 0;
    int frame_count = m_frame_sizes.size();
    while (target_frame < frame_count) {
      int frame_size = m_frame_sizes[target_frame];
      if (position <= scan_position + frame_size)
        break;
      else {
        scan_position += frame_size;
        target_frame++;
      }
    }
    // TODO
  }

  int MP3InputStream::getLength() { return m_length; }

  void MP3InputStream::getFormat(int& channel_count, int& sample_rate, SampleFormat& sample_format)
  {
    channel_count = m_channel_count;
    sample_rate = m_sample_rate;
    sample_format = m_sample_format;
  }


    // totally replaced the wasteful vesion here
  int MP3InputStream::doRead(int frame_count, void* samples)
  {
      ADR_GUARD("MP3InputStream::doRead");
      return decodeFrames(frame_count,samples);
  }


  void MP3InputStream::reset()
  {
    ADR_GUARD("MP3InputStream::reset");

    m_file->seek(0, File::BEGIN);
    m_eof = false;

    m_buffer.clear();

    if (m_seekable) mpg123_seek(mh, 0, SEEK_SET);
     else
     {
         mpg123_close(mh);
         mpg123_open_feed(mh);
     }


    m_input_position = 0;
    m_input_length = 0;
    m_position = 0;
  }

    int MP3InputStream::decodeFrames(int frames, void* samples)
    {
        int ret, rret;
        unsigned int len = frames * GetFrameSize(this);
        unsigned char *out = (unsigned char*)samples;
        size_t rb = 0, rv = 0;

        if (!mh) return 0;

        while (len > 0)
        {
            ret = mpg123_read(mh, out, len, &rv);
            rb += rv;
            out += rv;
            len -= rv;
            if (ret == MPG123_NEED_MORE)
            {
                // read the data
                m_input_length = m_file->read(m_input_buffer, INPUT_BUFFER_SIZE);
                if (m_input_length == 0) { m_eof = true; return true; }
                // feed it to mpg123
                rret = mpg123_feed(mh,m_input_buffer,m_input_length);
                if (rret != MPG123_OK) return false;    // something went wrong!
            }
        }

        return rb / GetFrameSize(this);
    }

    bool MP3InputStream::decodeFrame()
    {
        const int frame_size = GetFrameSize(this);
        int ret, rret;
        size_t rv = 0;

        if (!mh) return false;

        while (rv < (size_t)frame_size)
        {
            ret = mpg123_read(mh, m_decode_buffer, frame_size, &rv);
            if (ret == MPG123_NEED_MORE)
            {
                // read the data
                m_input_length = m_file->read(m_input_buffer, INPUT_BUFFER_SIZE);
                if (m_input_length == 0) { m_eof = true; return true; }
                // feed it to mpg123
                rret = mpg123_feed(mh,m_input_buffer,m_input_length);
                if (rret != MPG123_OK) return false;    // something went wrong!
            }
        }


        if (m_first_frame)
        {
            int enc = 0;
            long int li;
            mpg123_getformat(mh, &li, &m_channel_count, &enc);
            m_sample_rate = (int)li;
            switch (enc)
            {
                case MPG123_ENC_SIGNED_16: m_sample_format = SF_S16; break;
                case MPG123_ENC_UNSIGNED_8: m_sample_format = SF_U8; break;
                default: return false;
            }
            m_first_frame = false;
        }

        m_buffer.write(m_decode_buffer, rv);

        return true;
    }

    const char* getGenre(u8 code) {
    const char* genres[] = {
      // From Appendix A.3 at http://www.id3.org/id3v2-00.txt and

      "Blues", "Classic Rock", "Country", "Dance", "Disco", "Funk",
      "Grunge", "Hip-Hop", "Jazz", "Metal", "New Age", "Oldies", "Other",
      "Pop", "R&B", "Rap", "Reggae", "Rock", "Techno", "Industrial",
      "Alternative", "Ska", "Death Metal", "Pranks", "Soundtrack",
      "Euro-Techno", "Ambient", "Trip-Hop", "Vocal", "Jazz+Funk",
      "Fusion", "Trance", "Classical", "Instrumental", "Acid", "House",
      "Game", "Sound Clip", "Gospel", "Noise", "AlternRock", "Bass",
      "Soul", "Punk", "Space", "Meditative", "Instrumental Pop",
      "Instrumental Rock", "Ethnic", "Gothic", "Darkwave",
      "Techno-Industrial", "Electronic", "Pop-Folk", "Eurodance",
      "Dream", "Southern Rock", "Comedy", "Cult", "Gangsta", "Top 40",
      "Christian Rap", "Pop/Funk", "Jungle", "Native American",
      "Cabaret", "New Wave", "Psychadelic", "Rave", "Showtunes",
      "Trailer", "Lo-Fi", "Tribal", "Acid Punk", "Acid Jazz", "Polka",
      "Retro", "Musical", "Rock & Roll", "Hard Rock", "Folk", "Folk-Rock",
      "National Folk", "Swing", "Fast Fusion", "Bebob", "Latin", "Revival",
      "Celtic", "Bluegrass", "Avantgarde", "Gothic Rock",
      "Progressive Rock", "Psychedelic Rock", "Symphonic Rock",
      "Slow Rock", "Big Band", "Chorus", "Easy Listening", "Acoustic",
      "Humour", "Speech", "Chanson", "Opera", "Chamber Music", "Sonata",
      "Symphony", "Booty Bass", "Primus", "Porn Groove", "Satire",
      "Slow Jam", "Club", "Tango", "Samba", "Folklore", "Ballad",
      "Power Ballad", "Rhythmic Soul", "Freestyle", "Duet", "Punk Rock",
      "Drum Solo", "Acapella", "Euro-House", "Dance Hall",

      // http://lame.sourceforge.net/doc/html/id3.html

      "Goa", "Drum & Bass", "Club-House", "Hardcore", "Terror", "Indie",
      "BritPop", "Negerpunk", "Polsk Punk", "Beat", "Christian Gangsta",
      "Heavy Metal", "Black Metal", "Crossover", "Contemporary C",
      "Christian Rock", "Merengue", "Salsa", "Thrash Metal", "Anime",
      "JPop", "SynthPop",
    };
    const int genre_count = sizeof(genres) / sizeof(*genres);

    return (code < genre_count ? genres[code] : "");
  }

    void MP3InputStream::GetMpg123String(mpg123_string *s, std::string &dest)
    {
        dest = "";
        unsigned int cnt = 0;
        if (s != NULL)
        {
            while (cnt < s->size) dest.push_back(s->p[cnt++]);
        }
    }

    void MP3InputStream::GetID3()
    {
        mpg123_id3v1 *v1;
        mpg123_id3v2 *v2;
        std::string k, v, t;
        char b[32];

        if (mpg123_id3(mh,&v1,&v2) == MPG123_OK)
        {
            if (v1)
            {
                t = "ID3v1";
                k = "title"; memcpy(b,v1->title,30); b[30] = 0; v = b; addTag(k,v,t);
                k = "artist"; memcpy(b,v1->artist,30); b[30] = 0; v = b; addTag(k,v,t);
                k = "album"; memcpy(b,v1->album,30); b[30] = 0; v = b; addTag(k,v,t);
                k = "comment"; memcpy(b,v1->comment,30); b[30] = 0; v = b; addTag(k,v,t);
                k = "year"; memcpy(b,v1->year,4); b[4] = 0; v = b; addTag(k,v,t);
                k = "genre"; v = getGenre(v1->genre); addTag(k,v,t);
            }

            if (v2)
            {
                t = "ID3v2";
                k = "title"; GetMpg123String(v2->title,v); addTag(k,v,t);
                k = "artist"; GetMpg123String(v2->artist,v); addTag(k,v,t);
                k = "album"; GetMpg123String(v2->album,v); addTag(k,v,t);
                k = "year"; GetMpg123String(v2->year,v); addTag(k,v,t);
                k = "genre"; GetMpg123String(v2->genre,v); addTag(k,v,t);
                k = "comment"; GetMpg123String(v2->comment,v); addTag(k,v,t);
            }
        }
    }

}

#endif


