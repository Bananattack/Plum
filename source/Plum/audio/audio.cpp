#include "../plum.h"

namespace Plum
{
	void Audio::startup()
	{
		FMOD_RESULT result;
		FMOD_SPEAKERMODE speakermode;
		FMOD_CAPS caps;
		char name[256];

		FMOD::System_Create(&soundSystem);

		#ifdef __linux__
			soundSystem->setOutput(FMOD_OUTPUTTYPE_ALSA);
		#endif

		soundSystem->getDriverCaps(0, &caps, 0, 0, &speakermode);

		if(caps & FMOD_CAPS_HARDWARE_EMULATED)
		{
			printf("\n    Your sound hardware settings are crap, resorting to emulated hardware.\n");
			soundSystem->setDSPBufferSize(1024, 10);
		}
		soundSystem->getDriverInfo(0, name, 256, 0);
		// Sigmatel sound devices crackle for some reason if the format is PCM 16bit.  PCM floating point output seems to solve it.
		if(strstr(name, "SigmaTel"))
		{
			soundSystem->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0, 0, FMOD_DSP_RESAMPLER_LINEAR);
		}

		result = soundSystem->init(32, FMOD_INIT_NORMAL, 0);
		// Ok, the speaker mode selected isn't supported by this soundcard.  Switch it back to stereo...
		if(result == FMOD_ERR_OUTPUT_CREATEBUFFER)
		{
			soundSystem->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
			result = soundSystem->init(32, FMOD_INIT_NORMAL, 0);
		}
		if(result != FMOD_OK)
		{
			printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
			throw Audio::Exception("Couldn't initialize sound engine.\n");
		}
		printf(" OK!\n");
	}

	void Audio::shutdown()
	{
		soundSystem->release();
	}

	void Audio::update()
	{
		soundSystem->update();
	}

	void Audio::setMasterPitch(double pitch)
	{
		FMOD::ChannelGroup* grp;
		soundSystem->getMasterChannelGroup(&grp);
		grp->setPitch((float) pitch);
	}

	Sound* Audio::loadSound(std::string filename)
	{
		return loadSound(filename.c_str());
	}


	FMOD_RESULT F_CALLBACK Audio::openSample(const char* name, int unicode, unsigned int* filesize, void** handle, void** userdata)
	{
		// Unicode support is TODO.
		if(unicode)
		{
			return FMOD_ERR_FILE_NOTFOUND;
		}

		ZZIP_FILE* f = zzip_fopen_plum(name, "rb");
		if(!f)
		{
			return FMOD_ERR_FILE_NOTFOUND;
		}

		zzip_seek(f, 0, SEEK_END);
		*filesize = zzip_tell(f);
		zzip_seek(f, 0, SEEK_SET);

		*handle = f;

		return FMOD_OK;
	}

	FMOD_RESULT F_CALLBACK Audio::closeSample(void* handle, void* userdata)
	{
		zzip_fclose((ZZIP_FILE*) handle);
		return FMOD_OK;
	}

	FMOD_RESULT F_CALLBACK Audio::readSample(void* handle, void* buffer, unsigned int sizebytes, unsigned int* bytesread, void* userdata)
	{
		ZZIP_FILE* f = (ZZIP_FILE*) handle;

		*bytesread = zzip_fread(buffer, 1, sizebytes, f);
		return FMOD_OK;
	}

	FMOD_RESULT F_CALLBACK Audio::seekSample(void* handle, unsigned int pos, void* userdata)
	{
		ZZIP_FILE* f = (ZZIP_FILE*) handle;
		
		zzip_seek(f, pos, SEEK_SET);
		return FMOD_OK;
	}


	Sound* Audio::loadSound(const char* filename)
	{
		FMOD::Sound* sample = NULL;

		FMOD_CREATESOUNDEXINFO info;
		memset(&info, 0, sizeof(FMOD_CREATESOUNDEXINFO));
		info.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
		info.useropen = Audio::openSample;
		info.userclose = Audio::closeSample;
		info.userread = Audio::readSample;
		info.userseek = Audio::seekSample;

		soundSystem->createSound(filename, FMOD_SOFTWARE | FMOD_LOOP_OFF | FMOD_2D, &info, &sample);
		return sample;
	}

	unsigned int Audio::playSound(Sound* sound, double volume)
	{
		FMOD::Channel* channel = NULL;
		soundSystem->playSound(FMOD_CHANNEL_FREE, sound, true, &channel);
		channel->setVolume((float) volume);
		channel->setPaused(false);
		return (unsigned int) channel;
	}

	Song* Audio::loadSong(std::string filename)
	{
		return loadSong(filename.c_str());
	}

	Song* Audio::loadSong(const char* filename)
	{
		FMOD::Sound* sample = NULL;
		
		FMOD_CREATESOUNDEXINFO info;
		memset(&info, 0, sizeof(FMOD_CREATESOUNDEXINFO));
		info.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
		info.useropen = Audio::openSample;
		info.userclose = Audio::closeSample;
		info.userread = Audio::readSample;
		info.userseek = Audio::seekSample;

		soundSystem->createSound(filename, FMOD_SOFTWARE | FMOD_LOOP_NORMAL | FMOD_2D, &info, &sample);

		Song* song = new Song();
		song->sample = sample;
		song->volume = 1.0;
		song->channel = NULL;
		return song;
	}

	void Audio::playSong(Song* song)
	{
		song->stop();

		FMOD::Channel* channel = NULL;
		soundSystem->playSound(FMOD_CHANNEL_FREE, song->sample, true, &channel);
		channel->setVolume((float) song->volume);
		channel->setPaused(false);

		song->channel = new Channel(channel);
	}
}