#include <iostream>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <cstdio>

#include <plaid/audio/implementation.h>

#include <modplug.h>
#include <cstdint>

#include "../../core/file.h"

namespace plaidgadget
{
	namespace codec_modplug
	{
		class ModplugStream : public AudioStream
		{
		public:
			ModplugStream(String fn, bool _loop)
			{
				loop = _loop;
				finished = true;
				valid = false;

                std::cout << "MODPLUGGGGGG" << std::endl;
				int error = 0;
                std::unique_ptr<plum::File> file(new plum::File(ToStdString(fn), plum::FileRead));
                // Get current position.
                unsigned int pos = file->tell();
                // Length to read = position of end - current.
                unsigned int length = 0;
                file->seek(pos, plum::SeekEnd);
                length = file->tell() - pos;
                file->seek(pos, plum::SeekStart);
                // Read fully.
                data.resize(length);
                file->readRaw(data.data(), data.size());

                mod = ModPlug_Load(&data[0], data.size());
                std::cout << " LIBMODPLUG?" << std::endl;
				if(!mod)
                {
                    std::cout << " LIBMODPLUG FAIL" << std::endl;
                    return;
                }

                
				//Info
                ModPlug_Settings settings;
                ModPlug_GetSettings(&settings);
				output.channels = settings.mChannels;
				output.rate = settings.mFrequency;

				//Woot!
                std::cout << settings.mBits << "bits" << std::endl;
                std::cout << output.channels << " chan @ " << output.rate << "hz" << std::endl;
				valid = true;
				finished = false;
			}

			virtual ~ModplugStream()
			{
				if(valid)
                {
                    ModPlug_Unload(mod);
                }
			}

			bool success()
			{
				return valid;
			}

			virtual AudioFormat format()
			{
				return output;
			}

			virtual bool exhausted()
			{
				return finished;
			}

			virtual void tick(Uint64 frame)
			{
			}

			virtual void pull(AudioChunk &chunk)
			{
				if (!chunk.length())   return;
				if (!valid || finished) {chunk.silence(); return;}

				int samples, have = 0, need = chunk.length();
                std::cout << "FUCK" << std::endl;

				// Resize the modplug intermediate buffer.
                if(need * chunk.format().channels > buffer.size())
                {
                    buffer.resize(need * chunk.format().channels);
                }

                //Create pointers to 16-bit data
				short *d16[PG_MAX_CHANNELS];
				for (Uint32 i = 0; i < chunk.format().channels; ++i)
                {
					d16[i] = (short*) chunk.start(i);
                }

				while (true)
				{
                    std::cout << "READ SOME BUFSIZE " << mod << " " << (need - have) * chunk.format().channels * 2 << std::endl;
					/*samples = stb_vorbis_get_samples_short(ogg,
						chunk.format().channels, d16, (need-have));*/
                    samples = ModPlug_Read(mod, &buffer[0], (need - have) * chunk.format().channels * 2) / chunk.format().channels / 2;
                    std::cout << "JUST READ " << samples << std::endl;

					if (samples < 0)
					{
						finished = true;
						std::cout << " MODPLUG ERROR" << std::endl;
						break;
					}
					if (samples == 0)
					{
						//File's end
						if (loop)
						{
							ModPlug_Seek(mod, 0);
							continue;
						}
						else
						{
							finished = true;
							break;
						}
					}

					for (Uint32 i=0; i < chunk.format().channels; ++i)
                    {
                        for(size_t j = 0; j < samples; ++j)
                        {
                            d16[i][j] = buffer[j * chunk.format().channels + i];
                        }
						d16[i] += samples;
                    }
					have += samples;
					if (have > need) std::cout << "MODPLUG OVERDRAW" << std::endl;

					std::cout << "MOD pull: " << have << "/" << need << std::endl;

					if (have >= need) break;
				}

				//Cutoff marker if necessary
				if (have < need) chunk.cutoff(have);

				//Upsample data to 24-bit Sint32s
				for (Uint32 i=0; i < chunk.format().channels; ++i)
				{
					Sint32 *start = chunk.start(i), *op = start + have;
					short *ip = d16[i];
					while (op!=start) {*(--op) = 256 * Sint32(*(--ip));}
				}
			}

		private:
			bool loop, finished;
			bool valid;
			AudioFormat output;
            std::vector<uint16_t> buffer;

            std::vector<uint8_t> data;
			ModPlugFile* mod;
		};


		class Codec : public AudioCodec
		{
		public:
			Codec() : AudioCodec(L"it,s3m,mod,xm") { std::cout << "MODPLUGGGGGG REGISTERERRRE MFM< " << std::endl; }
			virtual ~Codec() {}

			virtual Sound stream(const String &file, bool loop)
			{
				ModplugStream *mod = new ModplugStream(file, loop);
				if (!mod->success()) {delete mod; return Sound();}
				return Sound(mod);
			}
		};

		//Register the codec by instantiating it in static time
		static Codec codec;
	}
}
