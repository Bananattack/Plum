#include "portaudio.h"

#include <iostream>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <vector>

//#include <SDL/SDL.h>

#include <plaid/audio.h>
#include <plaid/audio/implementation.h>

#if PLAIDGADGET
#include <plaid/graphics.h>
#include <plaid/console.h>
#endif

#include <plaid/thread/lockfree.h>


#define PCM_OUT 0


using namespace std;


static int MixAudio(const void *input, void *output,
	unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags, void *userData);


namespace plaidgadget
{
	class AudioImp_PortAudio : public AudioImp
	{
	public:
		AudioImp_PortAudio(Audio &audio, AudioScheduler &scheduler) :
			AudioImp(audio, scheduler), inDev(0), outDev(0)
		{
			//Initialize PortAudio
			PaError err = Pa_Initialize();
			if (err)
			{
				cout << "Error initializing PortAudio: "
					<< Pa_GetErrorText(err) << endl;
			}

			struct
			{
				void operator()(PaHostApiIndex ind)
				{
					const PaHostApiInfo *api = Pa_GetHostApiInfo(ind);
					cout << "  API " << ind << " '" << api->name << "'";
					if (ind == Pa_GetDefaultHostApi()) cout << " (*)";
					cout << " with " << api->deviceCount << " devices:" << endl;
					for (int i = 0, e = api->deviceCount; i != e; ++i)
					{
						PaDeviceIndex did =
							Pa_HostApiDeviceIndexToDeviceIndex(ind, i);
						const PaDeviceInfo *dev = Pa_GetDeviceInfo(did);
						cout << "  |- " << dev->defaultSampleRate << ' ';
						if (dev->maxOutputChannels)
							cout << ((did==api->defaultOutputDevice)?'+':' ')
								<< dev->maxOutputChannels << "o ";
						else cout << "    ";
						if (dev->maxInputChannels)
							cout << ((did==api->defaultInputDevice)?'+':' ')
								<< dev->maxInputChannels << "i ";
						else cout << "    ";
						cout << '`' << dev->name << '\'' << endl;
					}
				}
			} describeAPI;

			cout << "AVAILABLE AUDIO APIs: " << Pa_GetHostApiCount() << endl;
			for (PaHostApiIndex i = 0, e = Pa_GetHostApiCount(); i != e; ++i)
			{
				describeAPI(i);
			}
			cout << "-------------------------" << endl;

			//Get audio device
			PaHostApiIndex api = -1;

#if 0 //defined(_WIN32) || defined(WIN32)
			//WASAPI
			api = Pa_HostApiTypeIdToHostApiIndex(paASIO);
			if (api >= 0)
			{
				cout << "Using preferred API, ASIO..." << endl;

				const PaHostApiInfo *info = Pa_GetHostApiInfo(api);
				inDev  = info->defaultInputDevice;
				outDev = info->defaultOutputDevice;

				if (!inDev || !outDev)
				{
					cout << "But no suitable pairing existed..." << endl;
					api = -1;
				}
				else
				{
					output.rate = Pa_GetDeviceInfo(outDev)->defaultSampleRate;
					output.channels = 2;
				}

				output.rate = 48000;
				output.channels = 2;
			}

#else
			if (api < 0)
			{
				inDev  = Pa_GetDefaultInputDevice();
				outDev = Pa_GetDefaultOutputDevice();

				//Output format
				output.rate = 48000;
				output.channels = 2;
			}
#endif

		/*#ifdef _WIN32
			{
				int bestAPI = -1;
				for (PaHostApiIndex i = 0, e = Pa_GetHostApiCount(); i!=e; ++i)
				{
					const PaHostApiInfo *info = Pa_GetHostApiInfo(i);
				}
			}
		#else*/

			//Open stream
			PaStreamParameters inParam = {inDev, 1, paInt16, inDev ? Pa_GetDeviceInfo(inDev)->defaultLowInputLatency : 0, nullptr};
            PaStreamParameters outParam = {outDev, 2, paInt16|paNonInterleaved, outDev ? Pa_GetDeviceInfo(outDev)->defaultLowOutputLatency : 0, nullptr};
			err = Pa_OpenStream(&stream, inDev ? &inParam : nullptr, outDev ? &outParam : nullptr, output.rate,
				paFramesPerBufferUnspecified, paNoFlag, &MixAudio, (void*)this);
			if (err)
			{
				cout << "Error initializing PortAudio: "
					<< Pa_GetErrorText(err) << endl;
			}
		//#endif
		}
		~AudioImp_PortAudio()
		{
			//Kill output stream
			Pa_StopStream(stream);
			Pa_CloseStream(stream);

			//Close PortAudio
			PaError err = Pa_Terminate();
			if (err)
			{
				cout << "Error closing PortAudio: "
					<< Pa_GetErrorText(err) << endl;
			}
		}

		virtual void startStream()
		{
			//Start audio pull thread
			PaError err = Pa_StartStream(stream);
			if (err)
			{
				cout << "Error starting audio stream: "
					<< Pa_GetErrorText(err) << endl;
			}
		}

		virtual double time()
		{
			return Pa_GetStreamTime(stream);
		}

		virtual float load()
		{
			return Pa_GetStreamCpuLoad(stream);
		}

		virtual AudioFormat format()
		{
			return output;
		}

		virtual void update()
		{
			//Nothin'
		}

        PaDeviceIndex inDev, outDev;
		AudioFormat output;
		PaStream *stream;

		void mix(const void *_mike, void *_speaker,
			unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo,
			PaStreamCallbackFlags statusFlags)
		{
			//Buffers
			static Sint32 mbuff[24000],
				obuff[48000], *ochan[4] = {obuff, obuff+24000, nullptr, nullptr};
			Sint16 **speaker = (Sint16**) _speaker;

            if(inDev != 0)
            {
			    //Mike data 32->24 conversion
			    Sint32 *pos = mbuff;
			    for (Sint16 *i=(Sint16*)_mike, *e=i+frameCount; i!=e; ++i)
				    *(pos++) = Sint32(*i << 8);
            }

			//Render audio
			//std::cout << "Demand from scheduler: frames x " << frameCount << std::endl;
			scheduler.render(ochan, mbuff, frameCount,
				timeInfo->currentTime, timeInfo->outputBufferDacTime);
			//std::cout << "Completed scheduler" << std::endl;

			//Speaker data 24->16 conversion
            if(outDev != 0)
            {
			    Sint32 samp;
			    for (Uint32 c = 0; c < output.channels; ++c)
				    for (Sint16 *i = speaker[c], *e=i+frameCount; i!=e; ++i)
			    {
				    samp = (ochan[c][(i-speaker[c])]) >> 8;
				    if (samp > +32767) samp = +32767;
				    if (samp < -32767) samp = -32767;
				    *i = samp; // (intptr_t(i)&4)?30000:-30000;
			    }
            }

			/*if (Pa_GetStreamTime(stream) > criticalTime)
				mixReport << " (LATE FOR CAPTURE!)";
			if (obFill != obSize) mixReport << " (INCOMPLETE RENDER)";*/
		}
	};

	AudioImp *Implementation_Audio(Audio &audio, AudioScheduler &scheduler)
	{
		return new AudioImp_PortAudio(audio, scheduler);
	}
}

int MixAudio(const void *input, void *output,
	unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags, void *module)
{
	((plaidgadget::AudioImp_PortAudio*) module)->mix(
		input, output, frameCount, timeInfo, statusFlags);
	return 0;
}
