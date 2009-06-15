#ifndef PLUM_AUDIO_H
#define PLUM_AUDIO_H

namespace Plum
{
	typedef FMOD::Sound Sound;

	struct Channel
	{
		FMOD::Channel* channel;
		Channel(unsigned int channel)
		{
			this->channel = (FMOD::Channel*) channel;
		}

		Channel(FMOD::Channel* channel)
		{
			this->channel = channel;
		}
		~Channel() {}

		void resume()
		{
			channel->setPaused(false);
		}

		void pause()
		{
			channel->setPaused(true);
		}
		
		bool isPlaying()
		{
			bool playing;
			channel->isPlaying(&playing);
			return playing;
		}

		void stop()
		{
			channel->stop();
		}

		double getVolume()
		{
			float volume;
			channel->getVolume(&volume);
			return volume;
		}

		void setVolume(double volume = 1.0)
		{
			channel->setVolume((float) volume);
		}
	};


	struct Song
	{
		FMOD::Sound* sample;
		Channel* channel;
		double volume;

		Song() : sample(0), channel(0), volume(1.0)
		{
		}

		void resume()
		{
			if(channel)
			{
				channel->resume();
			}
		}

		void pause()
		{
			if(channel)
			{
				channel->pause();
			}
		}
		
		bool isPlaying()
		{
			return channel && channel->isPlaying();
		}

		void stop()
		{
			if(channel)
			{
				channel->stop();
				channel = NULL;
			}
		}

		double getVolume()
		{
			return this->volume;
		}

		void setVolume(double volume = 1.0)
		{
			this->volume = volume;
			if(channel)
			{
				channel->setVolume(volume);
			}
		}
	};

	struct Audio
	{
		class Exception : public std::exception
		{
			public:
				Exception(const std::string& message)
					: msg(message)
				{
				}

				virtual const char* what() const throw ()
				{
					return msg.c_str();
				}

				virtual ~Exception() throw ()
				{
				}

			private:
				std::string msg;
		};

		FMOD::System* soundSystem;

		Audio()
			: soundSystem(0)
		{
		}

		void startup();
		void shutdown();
		void update();
		void setMasterPitch(double pitch);

		Sound* loadSound(std::string filename);
		Sound* loadSound(const char* filename);
		unsigned int playSound(Sound* sound, double volume = 1.0);

		Song* loadSong(std::string filename);
		Song* loadSong(const char* filename);
		void playSong(Song* song);
	};
}

#endif