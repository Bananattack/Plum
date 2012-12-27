#ifndef PLAIDGADGET_AUDIO_UTIL_H
#define PLAIDGADGET_AUDIO_UTIL_H


#include "../thread/lockfree.h"
#include <map>
#include <vector>

#include "audio.h"


/*
	Additional AudioStream classes which perform useful functions.

	Mixer: combines sound streams and provides an easy API to control them.
	Transcoder: changes the format of sound streams.

	AudioBuffer: holds sound data which can be played at any time.
	AudioCapture: intercepts audio data from the stream; useful for recording.
*/
namespace plaidgadget
{
	/*
		A mixer adds other audio streams together while controlling volume.

		Sounds:
			Can be volume-controlled, muted or paused.
			Can be paused individually.
			Are automatically dropped when they exhaust.  (When they finish)
	*/
	class Mixer : public AudioStream
	{
	public:
		Mixer(AudioFormat format);
		virtual ~Mixer();

		//Master controls
		void play();
		void pause();
		void volume(float volume);

		//Add sounds (starting off paused) - returns whether added
		bool add(Sound sound);

		//Drop sounds (if a sound "ends" it's dropped automatically)
		void stop(Sound sound)    {drop(sound);}
		void drop(Sound sound);
		//  'roll' command forthcoming.  :)

		//Sound settings (non-present sounds will be autoadded if possible)
		void play(Sound sound);
		void pause(Sound sound);
		void volume(Sound sound, float volume);

		/*
			Query sound settings.
			Note that dropped sounds will often not appear as such until after
				a little time has passed...
		*/
		bool has(Sound sound);
		bool playing(Sound sound);
		float volume(Sound sound);

		//Check if mixer is clipping
		//bool clipping(bool reset = false);

	protected:
		//All that audio goodness
		virtual AudioFormat format();
		virtual void pull(AudioChunk &chunk);
		virtual bool exhausted();
		virtual void tick(Uint64 frame);

	private:
		Signal *findOrAdd(Sound sound);

	private:
		class Channel
		{
		public:
			Channel() : play(false), drop(false), volume(1.0f) {}
			bool play, drop; float volume;
		};

		typedef std::map<Sound, Signal> Signals;
		typedef std::pair<Sound, Signal> SignalsEntry;

		typedef std::map<Signal*, Channel> Channels;
		typedef std::pair<Signal*, Channel> ChannelsEntry;

		enum ACTIONS {NONE=0,
			GPLAY=1, GVOLUME=2, ADD=3, DROP=4, PLAY=5, VOLUME=6};
		struct Action
		{
			Uint32 code;
			Signal *signal;
			float value;

			Action(Uint32 _c=0, Signal *_s=NULL, float _v=0.0f) :
				code(_c), signal(_s), value(_v) {}
		};

	private:
		//Static settings
		AudioFormat output;

		//Program-side data
		Signals signals;
		Uint64 extFrame;
		float extPlay; float extVolume;
		Channels external;
		TimedEventQueue<Action> actions;
		//bool clipped;

		//Mixer-side data
		bool intPlay; float intVolume;
		Channels internal;
		LockFreeQueue<Signal*> drops;
	};

	/*
		A Splicer plays other AudioStreams in sequence, such that as
			each one exhausts the next one begins.
			If nothing is queued, the Splicer itself is exhausted.

		A common use case is music which has "intro" and "loop" parts.
	*/
	class Splicer : public AudioStream
	{
	public:
		//Create an empty Splicer with the given format.
		Splicer(AudioFormat format);

		//Create a Splicer from the given Sounds, using their format.
		Splicer(Sound first);
		Splicer(Sound first, Sound second);

		//Add more sounds to the sequence.
		void add(Sound source);

		virtual ~Splicer();

	protected:
		//All that audio goodness
		virtual AudioFormat format();
		virtual void pull(AudioChunk &chunk);
		virtual bool exhausted();
		virtual void tick(Uint64 frame);

	private:
		AudioFormat output;

		typedef std::list<Signal> Signals;
		Signals signals;
		LockFreeQueue<Signal*> play, stop;
		Signal *current;
	};

	/*
		This class allows the output of one AudioStream to act as an input to
			many others, assuming all are pulling at the same rate.
			(If rates differ, EG from a Pitch effect, glitches will occur...)

		Each audio frame, the first pull populates the Splitter's buffer with
			the output of its source stream, and all further pulls output this.
	*/
	class Splitter : public RefCounted
	{
	public:
		//Create a splitter with the given sample capacity per channel.
		Splitter(Signal source, Uint32 capacity = 12000);

		//Make copies of the Splitter -- one for each output desired.
		Splitter(Splitter *other);
		Splitter *copy();

	protected:
		virtual AudioFormat format();
		virtual void pull(AudioChunk &chunk);
		virtual bool exhausted();
		virtual void tick(Uint64 frame);

	private:
		struct Common
		{
			Common(Signal source, Uint32 size);
			~Common();

			Signal source;
			Uint64 frame;
			float a;
			Sint32 *data[PG_MAX_CHANNELS];
			Uint32 capacity, length;
		};
		Ref<Common> common;
	};


	/*
		A transcoder.  Used to change audio from one sample format to another.

		The 'Pitch' class should be used to change the sample rate, if that is
			also necessary.
			The Transcoder "spoofs" the sample rate in any case, however.

		This class is useful enough that it (and Pitch) are automatically added
			by some constructors in the Signal class.
	*/
	class Transcoder : public AudioStream
	{
	public:
		class Function;

	public:
		//Transcode from the format of the source to the destination format.
		Transcoder(Signal source, AudioFormat dest);
		virtual ~Transcoder();

	protected:
		//Notably formats can't be requested.  For that, get a new transcoder.
		virtual AudioFormat format();
		virtual void pull(AudioChunk &chunk);
		virtual bool exhausted();
		virtual void tick(Uint64 frame);

	private:
		Signal source;
		AudioFormat dest;
		Function *function;
		float ratio, offset;
	};


	/*
		AudioBuffer:  A buffer of audio data.  (Not actually an AudioStream)

		It can be defined either by an AudioCapture object (streamside) or,
			more commonly, from an exhaustible stream such as a sound file.

		After being defined, it can be LOCKED and subsequently played.
			A locked AudioBuffer can no longer be safely modified.
	*/
	class AudioBuffer
	{
	public:
		/*
			Define the buffer by rendering the given number of frames from the
				provided signal.  This will occur immediately and not in the
				audio callback; for long sequences a hitch may be incurred.

			TODO gradual load?
		*/
		AudioBuffer(Signal source, Uint32 frames);

		//Null buffers
		AudioBuffer() : data(NULL) {}
		static AudioBuffer Null() {return AudioBuffer();}
		bool null() const {return !data;}
		operator bool() const {return data;}

	private:
		bool locked;
		class Data;
		Ref<RefCounted> rc;
		Data *data;
	};
}


#endif // PLAIDGADGET_AUDIO_UTIL_H
