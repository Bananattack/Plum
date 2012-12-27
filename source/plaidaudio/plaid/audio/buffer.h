#ifndef PLAIDGADGET_AUDIO_BUFFER_H
#define PLAIDGADGET_AUDIO_BUFFER_H

#include "stream.h"

namespace plaidgadget
{
	/*
		AudioBuffer:  Captured or loaded audio that can be played from memory by
			an arbitrary number of player streams.

		Uses:
			non-streaming (preloaded) sound effects
			recording microphone input or rendered audio
	*/
	class AudioBuffer
	{
	private:
		class Data
		{
		public:
			Audio &audio;
			AudioFormat format;
			std::vector<Uint8> data;

			bool _lock;
		};

	public:
		AudioBuffer(Audio &audio);

		//Access the buffered data.


		//Get a playable stream -- this locks the buffer.
		//Sound get();

		bool locked();

	private:
		Ref<Data> data;
	};
}

#endif // PLAIDGADGET_AUDIO_BUFFER_H
