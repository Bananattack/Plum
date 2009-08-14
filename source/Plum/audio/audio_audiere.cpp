#include "../plum.h"

namespace Plum
{
	void Audio::startup(bool disableSound)
	{
		this->cleanupSteps = 0;
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
			if(++cleanupSteps == STEPS_TO_CLEANUP)
			{
				// Cleanup all inactive channels.
				// Reverse iteration so we can delete while we loop over things.
				for(int i = activeChannels.size() - 1; i >= 0; i--)
				{
					if(activeChannels[i]->shouldDispose)
					{
						delete activeChannels[i];
						activeChannels.pop_back();
					}
				}
				cleanupSteps = 0;
			}

			// Update the audio device.
			audioDevice->update();
		}
	}

	void Audio::setMasterPitch(double pitch)
	{
		masterPitch = pitch;
		for(u32 i = 0; i < activeChannels.size(); i++)
		{
			if(!activeChannels[i]->shouldDispose)
			{
				activeChannels[i]->setPitch(pitch);
			}
		}
		for(u32 i = 0; i < soundStreamPool.size(); i++)
		{
			if (soundStreamPool[i] && soundStreamPool[i]->isPlaying())
			{
				soundStreamPool[i]->setPitchShift((float) pitch);
			}
		}
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

	// Attempt to allocate a new stream for a sound effect,
	// reusing an existing array slot, where possible
	// Returns the handle index for the added stream, or -1 on failure (no sound, bad stream).
	// Handle may become invalidated if the stream becomes null, or the sound stream finishes playing.
	int Audio::addSoundStream(audiere::OutputStreamPtr stream)
	{
		if(disableSound)
		{
			return -1;
		}
		if(!stream)
		{
			return -1;
		}

		int found = -1;
		// Reuse streams where possible
		// Cleanup along the way.
		for (u32 i = 0; i < soundStreamPool.size(); ++i)
		{
			if (soundStreamPool[i] && !soundStreamPool[i]->isPlaying())
			{
				soundStreamPool[i] = NULL;
			}
			if(!soundStreamPool[i])
			{
				if(found == -1)
				{
					found = i;
				}
			}
		}

		if(found != -1)
		{
			found;
			soundStreamPool[found] = stream;
			return found;
		}
		else
		{
			soundStreamPool.push_back(stream);
			return soundStreamPool.size() - 1;
		}
	}

	int Audio::playSound(Sound* sound, double volume)
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
		int handle = addSoundStream(stream);
		stream->play();

		return handle;
	}

	Channel* Audio::createChannel(int handle)
	{
		if(handle >= 0 && handle < soundStreamPool.size())
		{
			return createChannel(soundStreamPool[handle]);
		}
	}

	Channel* Audio::createChannel(audiere::OutputStreamPtr stream)
	{
		if(disableSound)
		{
			return NULL;
		}
		if(!stream)
		{
			return NULL;
		}


		Channel* chan = new Channel(stream);
		activeChannels.push_back(chan);
		return chan;
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

		song->channel = createChannel(stream);
		stream->setRepeat(true);
		stream->setVolume((float) song->getVolume());
		stream->play();
	}
}