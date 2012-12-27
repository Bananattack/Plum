#ifndef PLAIDGADGET_AUDIOSTREAM_H
#define PLAIDGADGET_AUDIOSTREAM_H

#include <cstring>
#include <set>

#include "../core.h"


/**
    The basic classes used to represent streams of audio in Plaidgadget.

	None of these classes actually produce audio; the base classes for such
		functionality and common utilities are represented.

	Programmers simply USING the audio API rather than implementing their own
		effects will not generally need to deal with these interfaces.
*/

namespace plaidgadget
{
	class Audio;

	/*
		The maximum number of audio channels supported by this build.
		Affects array size in AudioChunk.

		Standard channel counts:
		1 - Mono
		2 - Stereo
		3 - Stereo w/center
		4 - Quadruphonic
		5 - Quadruphonic w/center
		6 - Dolby 5.1
		8 - Dolby 7.1
		12 - 10.2 theatre surround
		24 - 22.2 theatre surround
	*/
	#define PG_MAX_CHANNELS 8


	/*
		Represents an audio format.  48000 hz stereo is typical.

		As of July 24, 2012, DSP sample format was locked to 24-bit.
			All AudioStreams use 24-bit samples in 32-bit aligned integers.
			Values outside the 24-bit range will be clamped at output.
			The extra 8 bits allow for mastering features and simpler code.

		Sampling rate can still vary, and AudioStreams are generally expected
			to conform to the one dictated at runtime by the Audio module.
	*/
	class AudioFormat
	{
	public:
		enum CHANNELMODE {MONO=1, STEREO=2};
		static const Sint8  INT8_CLIP  = 127;
		static const Sint16 INT16_CLIP = 32767;
		static const Sint32 INT24_CLIP = 16777215;
		//All floating-point types clip at +/- 1.0

	public:
		AudioFormat() :
			channels(0), rate(0) {}
		AudioFormat(Uint32 _channels, Uint32 _rate) :
			channels(_channels), rate(_rate) {}

		//Comparison
		bool operator==(const AudioFormat &other) const
			{return channels == other.channels && rate == other.rate;}
		bool operator!=(const AudioFormat &other) const
			{return channels != other.channels || rate != other.rate;}

	public:
		Uint32 channels;
		Uint32 rate;     // Expressed in hertz (samples per second per channel)
	};


	/*
		A piece of audio, corresponding to all or part of a frame.

		'a' and 'b' indicate the progress through the frame represented
			by the start and end of this chunk.  Most commonly they'll be
			0 and 1, but split frames are to be expected of all systems.
			Split frames are usually split into two parts, but could easily be
			broken into three or more depending on the underlying system.

		Sometimes both 'start' and 'end' will be 0 and extra() will return true.
			This happens when, due to scheduling, the next frame's settings have
			not yet been prepared and a non-interpolated render must occur.

		Note that constructs are also planned which will facilitate timelines
			separate from engine framerate, such as musical tempos.  These can
			have arbitrarily large or small frame lengths and request sizes!
	*/
	class AudioChunk
	{
	public:
		/*
			Construct an AudioChunk.

			Pass data=NULL to allocate temp space from the scratch pool.
				(usually what you want for mixers/effects)

				*** NOTE ***
				The scratch pool can't be resized while it's in use.
				Thus, it's possible for an allocation to be denied.
				This will produce a NULL-data, 0-length chunk.
				Account for this in your code!
		*/
		AudioChunk(Audio &audio,
			const AudioFormat &format,       //Formatting
			Sint32 **data, Uint32 length,    //Buffer
			Uint64 frame, float a, float b); //Timing parameters

		~AudioChunk();


		//Check validity of chunk (false for unsuccessful scratch-allocs)
		bool ok() const    {return _data[0];}


		//Get data, iterator-style, for reading and/or writing
		Sint32 *start(Uint32 i)    {Sint32 *p=(i<PG_MAX_CHANNELS)?_data[i]:0;
						            if (!p) reportWarning(
										"NULL start on AudioChunk"); return p;}
		Sint32 *end  (Uint32 i)    {Sint32 *p=start(i); return p?p+_length:0;}

		Uint32 channels()          {return _format.channels;}

		//Length in audio frames (each channel gets one sample per frame)
		Uint32 length()            {return _length;}

		//Length in seconds
		float seconds()            {return float(_length)/float(_format.rate);}

		//Format
		const AudioFormat &format() {return _format;}

		//Frame ID
		Uint64 frame()              {return _frame;}


		/*
			Get the 'A' and 'B' values for this chunk.  They range from 0 to 1
				and represent the segment of the current frame the current chunk
				represents, with 0 being beginning and 1 being end.
				Generally used for interpolating effects.
		*/
		float a()         {return _a;}
		float b()         {return _b;}


		//Check if this is an "extra" render resulting from underflow
		bool extra()      {return _a == 1.0f;}

		//Check if this is a "whole" render, corresponding to an entire frame.
		bool whole()      {return (_a==0.0f) & (_b==1.0f);}
		float partial()   {return (_a!=0.0f) | (_b!=1.0f);}

		//Check if this is the first or last chunk of a frame.  Wholes are both.
		bool first()      {return _a==0.0f;}
		bool last()       {return _b==1.0f;}


		/*
			When a Stream is exhausted, set this value to how many samples of
				audio data (per channel) were produced before stream exhaustion.
				Automatically zeroes the rest of the Chunk.

			This facilitates, for example, the Splicer class.
		*/
		Uint32 cutoff()              {return _cutoff;}
		void cutoff(Uint32 sample)   {_cutoff = sample; silence(sample);}


		//Clear data to zero  (offsets are in bytes)
		void silence(Sint32 start=0, Sint32 end=-1)

			{Sint32 span = 4*( ((end<0)?_length:end) - start );
			for (Uint32 i = 0; i < _format.channels; ++i)
				std::memset((void*)(_data[i]+start), 0, span);}


		//Get the channel buffer array directly
		Sint32 *const* data() const {return _data;}


	public:
		// Easy access to the audio module
		Audio &audio;

	private:
		//no copying
		void operator=(const AudioChunk &) const;
		AudioChunk(const AudioChunk &);

	private:
		//Metadata
		AudioFormat _format;
		Uint64 _frame;
		float _a, _b;
		bool _scratch;
		Uint32 _cutoff;

		//Data
		Sint32 *_data[PG_MAX_CHANNELS];
		Sint32 _length;
	};

	/*
		The basis of all audio sources, be they synthesizers, decoders, buffers,
			mixers, effects or otherwise.

		Represents a stream of audio which can be "pulled" from as necessary.

		Note: AudioStream is for implementing sound objects.  The primary
			interface for using them is Sound; see pg_sound.h.

		Note: When implementing AudioStreams, the structures provided in
			pg_lockfree.h should be used for passing data.
	*/
	class AudioStream : public RefCounted
	{
	public:
		virtual ~AudioStream() {}

	protected:
		friend class Signal;


		//  ---  Functions below should be called from only one thread  ---

		AudioStream() :  reserved(false) {}

		//Attach to a sound before using its output.  Only one attachment is
		//  allowed at a time; if attach() returns false, don't use it!
		bool attach() {bool r = !reserved; reserved = true; return r;}
		void detach() {reserved = false;}

		//Query output formatting
		virtual AudioFormat format() = 0;

		//This is called every time the audio frame advances.
		//  Queue any changed settings and propagate the call to child streams.
		virtual void tick(Uint64 frame) = 0;

		//  ----------------------------------------------------------------


		//  ---  Functions below should only be called in render thread  ---

		//Returns whether this audio source has "run out" and can be dropped.
		//  Exhausted streams should always output silence after completion.
		virtual bool exhausted() = 0;

		//Call this function to pull sample data.
		//  Override it with rendering logic.
		virtual void pull(AudioChunk &chunk) = 0;

		//  ----------------------------------------------------------------

	private:
		//Disallow copying
		AudioStream(const AudioStream &other)
			{reportError("AudioStream does not allow copying...");}
		void operator=(const AudioStream &other)
			{reportError("AudioStream does not allow copying...");}

	private:
		template<typename T> friend class Stream;
		bool reserved;
	};


	/*
		The simplest way to refer to a sound object is with this typedef.
	*/
	typedef Ref<AudioStream> Sound;


	class Transcoder;

	/*
		A Signal is an attachment to an audio stream; it reserves the stream's
			output and allows that output to be captured.

		Most generally, Signals are used by audio classes that take inputs.
	*/
	class Signal
	{
	public:
		//Attach, regardless of source format
		Signal(const Sound &sound);
		Signal(AudioStream *stream);

		/*
			Attach, requesting a specific format.
			If the third parameter is true, attaching to a Stream with a
				different format will autotranscode; if false it will fail.

			format.channels = 0 specifies no preference regarding channels.
			format.rate = 0 is likewise for sampling rate.
		*/
		Signal(const Sound &sound, AudioFormat format, bool transcode = true);
		Signal(AudioStream *stream, AudioFormat format, bool transcode = true);

		// use "if (signal)" to check for attachment success

		//Null attachments
		Signal() {}
		static Signal Null()   {return Signal();}
		bool null() const      {return !data;}
		operator bool() const  {return data;}

		//Cast to Sound
		operator Sound() {if (data) return data->stream; return Sound();}

		//A signal which leaves an Audiochunk unmodified when pulled.
		//  Useful for a few specific back-end things.  (note: never exhausts)
		static Signal Dummy(const AudioFormat &format);

		//Patch-throughs to the bound stream.
		//  Unbound signals are treated as silence, and exhaust immediately.
		bool exhausted() const;
		AudioFormat format() const;
		void tick(Uint64 frame) const;
		void pull(AudioChunk &chunk) const;

		//Comparisons  (mostly so these can be used as set/map keys)
		bool operator< (const Signal &other) const {return data <  other.data;}
		bool operator<=(const Signal &other) const {return data <= other.data;}
		bool operator> (const Signal &other) const {return data >  other.data;}
		bool operator>=(const Signal &other) const {return data >= other.data;}
		bool operator==(const Signal &other) const {return data == other.data;}
		bool operator!=(const Signal &other) const {return data != other.data;}

	private:
		class Data
		{
		public:
			~Data();

			Sound stream;
			Signal *trans;
		};
		void bind(Sound stream, AudioFormat form, bool tc);

	private:
		Ref<Data> data;
	};
}

#endif // PLAIDGADGET_AUDIOSTREAM_H
