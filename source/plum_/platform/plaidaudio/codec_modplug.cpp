#include <plaid/audio/implementation.h>

#include <modplug.h>
#include <cstdint>
#include <memory>
#include <vector>

#include "../../core/file.h"

namespace
{
    class ModplugStream : public plaidgadget::AudioStream
    {
        public:
            ModplugStream(plaidgadget::String fn, bool loop)
                : loop(loop), finished(true)
            {
                {
                    std::unique_ptr<plum::File> file(new plum::File(plaidgadget::ToStdString(fn), plum::FileRead));
                    // Get current position.
                    unsigned int pos = file->tell();
                    // Length to read = position of end - current.
                    unsigned int length = 0;
                    file->seek(pos, plum::SeekEnd);
                    length = file->tell() - pos;
                    file->seek(pos, plum::SeekStart);
                    // Read fully.
                    std::vector<uint8_t> data(length);
                    file->readRaw(data.data(), data.size());
                    // Load the mod.
                    mod = ModPlug_Load(data.data(), data.size());
                }
                if(mod)
                {
                    // Info
                    ModPlug_Settings settings;
                    ModPlug_GetSettings(&settings);
                    output.channels = settings.mChannels;
                    output.rate = settings.mFrequency;

                    finished = false;
                }
            }

            virtual ~ModplugStream()
            {
                if(mod)
                {
                    ModPlug_Unload(mod);
                }
            }

            bool success()
            {
                return mod != nullptr;
            }

            virtual plaidgadget::AudioFormat format()
            {
                return output;
            }

            virtual bool exhausted()
            {
                return finished;
            }

            virtual void tick(plaidgadget::Uint64 frame)
            {
            }

            virtual void pull(plaidgadget::AudioChunk& chunk)
            {
                if(!chunk.length())
                {
                    return;
                }

                if(!mod || finished)
                {
                    chunk.silence();
                    return;
                }

                int have = 0;
                int need = chunk.length();

                // Expand the intermediate buffer.
                if(need * chunk.format().channels > buffer.size())
                {
                    buffer.resize(need * chunk.format().channels);
                }

                // Create pointers to 16-bit data
                int16_t* d16[PG_MAX_CHANNELS];
                for(plaidgadget::Uint32 i = 0; i < chunk.format().channels; ++i)
                {
                    d16[i] = (int16_t*) chunk.start(i);
                }

                while(true)
                {
                    int samples = ModPlug_Read(mod, buffer.data(),
                        (need - have) * chunk.format().channels * sizeof(int16_t)
                    ) / chunk.format().channels / sizeof(int16_t);

                    if(samples < 0)
                    {
                        finished = true;
                        break;
                    }
                    if(samples == 0)
                    {
                        // File's end
                        if(loop)
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

                    for(plaidgadget::Uint32 i = 0; i < chunk.format().channels; ++i)
                    {
                        for(int j = 0; j < samples; ++j)
                        {
                            d16[i][j] = buffer[j * chunk.format().channels + i];
                        }
                        d16[i] += samples;
                    }

                    have += samples;
                    if(have >= need)
                    {
                        break;
                    }
                }

                // Cutoff marker if necessary
                if(have < need)
                {
                    chunk.cutoff(have);
                }
                // Upsample data to 24-bit Sint32s
                for(plaidgadget::Uint32 i = 0; i < chunk.format().channels; ++i)
                {
                    plaidgadget::Sint32* start = chunk.start(i);
                    plaidgadget::Sint32* op = start + have;
                    int16_t* ip = d16[i];
                    while(op != start)
                    {
                        *--op = plaidgadget::Sint32(*--ip) << 8;
                    }
                }
            }

        private:
            bool loop, finished;
            plaidgadget::AudioFormat output;
            std::vector<int16_t> buffer;
            ModPlugFile* mod;
    };

    class Codec : public plaidgadget::AudioCodec
    {
        public:
            Codec()
                : AudioCodec(L"669,abc,amf,ams,dbm,dmf,dsm,far,it,j2b,mdl,med,mod,mt2,mtm,okt,pat,psm,ptm,s3m,stm,ult,umx,xm")
            {
            }

            virtual ~Codec()
            {
            }

            virtual plaidgadget::Sound stream(const plaidgadget::String& file, bool loop)
            {
                ModplugStream* mod = new ModplugStream(file, loop);
                if (!mod->success())
                {
                    delete mod;
                    return plaidgadget::Sound();
                }
                return plaidgadget::Sound(mod);
            }
    } codec;
}
