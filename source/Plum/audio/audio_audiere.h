#pragma once

namespace Plum
{

	struct Sound
	{
		audiere::SampleBufferPtr buffer;
		Sound() : buffer(0)
		{
		}

		~Sound()
		{
		}
	};

	struct Channel
	{
		audiere::OutputStreamPtr stream;
		Channel(ptrdiff_t channel)
		{
			this->stream = (audiere::OutputStream*) channel;
		}

		Channel(audiere::OutputStreamPtr channel)
		{
			this->stream = channel;
		}
		~Channel() {}

		void resume()
		{
			stream->play();
		}

		void pause()
		{
			stream->stop();
		}
		
		bool isPlaying()
		{
			return stream->isPlaying();
		}

		void stop()
		{
			stream->stop();
			stream->reset();
		}

		double getVolume()
		{
			return stream->getVolume();
		}

		void setVolume(double volume = 1.0)
		{
			stream->setVolume((float) volume);
		}
	};


	struct Song
	{
		audiere::SampleSourcePtr sample;
		Channel* channel;
		double volume;

		Song() : sample(0), channel(0), volume(1.0)
		{
		}

		~Song()
		{
			stop();
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
				delete channel;
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

	private:
		bool disableSound;
		audiere::AudioDevicePtr audioDevice;
		std::vector<audiere::OutputStreamPtr> streamPool;

		void addStream(audiere::OutputStreamPtr stream);
	public:
		Audio()
			: audioDevice(0)
		{
		}

		void startup(bool disableSound = false);
		void shutdown();
		void update();
		void setMasterPitch(double pitch);

		Sound* loadSound(std::string filename);
		Sound* loadSound(const char* filename);
		ptrdiff_t playSound(Sound* sound, double volume = 1.0);

		Song* loadSong(std::string filename);
		Song* loadSong(const char* filename);
		void playSong(Song* song);
	};
}