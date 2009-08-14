#include "../plum.h"

namespace Plum
{
	void Audio::startup(bool disableSound)
	{
		this->disableSound = disableSound;
		if(!disableSound)
		{
			audioDevice = audiere::OpenDevice();
		}
		// Failure (or sound disabled). Give no sound instead.
        if (!audioDevice)
		{
            audioDevice = audiere::OpenDevice("null", "");
		}
        if (!audioDevice)
		{
			throw Audio::Exception("Audiere must be broken. Failed to initialize, even with no sound.");
		}
	}

	void Audio::shutdown()
	{
		audioDevice = 0;
	}

	void Audio::update()
	{
		if(!disableSound)
		{
			audioDevice->update();
		}
	}

	void Audio::setMasterPitch(double pitch)
	{
		return;
	}

	Sound* Audio::loadSound(std::string filename)
	{
		return loadSound(filename.c_str());
	}

	Sound* Audio::loadSound(const char* filename)
	{
		if(disableSound)
		{
			return NULL;
		}

		audiere::FilePtr file = OpenPitAudiereFile(filename, false);
		if(!file.get())
		{
			return NULL;
		}
		audiere::SampleSourcePtr sample = audiere::OpenSampleSource(file, audiere::FF_AUTODETECT);

		if(!sample)
		{
			return NULL;
		}

		audiere::SampleBufferPtr buffer = audiere::CreateSampleBuffer(sample);
		if(!buffer)
		{
			return NULL;
		}

		Sound* sound = new Sound();
		sound->buffer = buffer;
		return sound;
	}


	void Audio::addStream(audiere::OutputStreamPtr stream)
	{
		if(disableSound)
		{
			return;
		}
		if(!stream)
		{
			return;
		}

		u32 found = 0;
		// Reuse streams where possible
		// Cleanup along the way.
		for (u32 i = 0; i < streamPool.size(); ++i)
		{
			if (streamPool[i] && !streamPool[i]->isPlaying())
			{
				streamPool[i] = NULL;
			}
			if(!streamPool[i])
			{
				if(!found)
				{
					found = i + 1;
				}
			}
		}

		if(found)
		{
			found--;
			streamPool[found] = stream;
		}
		else
		{
			streamPool.push_back(stream);
		}
	}

	ptrdiff_t Audio::playSound(Sound* sound, double volume)
	{
		if(disableSound)
		{
			return 0;
		}
		if(!sound)
		{
			return 0;
		}

		audiere::OutputStreamPtr stream = audioDevice->openStream(sound->buffer->openStream());
		if(!stream)
		{
			return 0;
		}
		addStream(stream);
		stream->play();

		return (ptrdiff_t) stream.get();
	}

	Song* Audio::loadSong(std::string filename)
	{
		return loadSong(filename.c_str());
	}

	Song* Audio::loadSong(const char* filename)
	{
		if(disableSound)
		{
			return NULL;
		}

		audiere::FilePtr file = OpenPitAudiereFile(filename, false);
		if(!file)
		{
			return NULL;
		}

		audiere::SampleSourcePtr sample = audiere::OpenSampleSource(file, audiere::FF_AUTODETECT);

		if(!sample)
		{
			return NULL;
		}

		Song* song = new Song();
		song->sample = sample;
		song->volume = 1.0;
		song->channel = NULL;
		return song;
	}

	void Audio::playSong(Song* song)
	{
		if(disableSound)
		{
			return;
		}
		if(!song)
		{
			return;
		}
		song->stop();

		audiere::OutputStreamPtr stream = audioDevice->openStream(song->sample.get());

		if(!stream)
		{
			return;
		}

		song->channel = new Channel(stream);
		stream->setRepeat(true);
		stream->setVolume((float) song->getVolume());
		stream->play();
	}
}