#include <cassert>
#include <ctime>
#include <cstring>

#include "audio.h"
#include "implementation.h"
#include "../thread/lockfree.h"

#include "util.h"
#include "effects.h"

#if PLAIDGADGET
	#include "../console/console.h"
	#include "../graphics/graphics.h"
	#include "../geometry/2d.h"
	#include "../player/player.h"
	#include "../storage/storage.h"

	#include "../script/bindings.h"
#else //PLAIDGADGET
	#define PI 3.14159f
#endif


using namespace plaidgadget;

using namespace std;


#if PLAIDGADGET
static void SoundConD(void *p)                     {new (p) Sound();}
static void SoundCopy(void *p, const Sound &o)     {new (p) Sound(o);}
static void SoundDest(void *p)                     {((Sound*)p)->~Sound();}
static bool SoundEquals(Sound *s, void *other, int typeID)
{
	if (typeID == 0) return s->null();
	if (typeID == asGetActiveContext()->GetEngine()->
		GetObjectTypeByName("Sound")->GetTypeId())
	{
		return (*s == *((Sound*)other));
	}
	else
	{
		//TODO comparison with audiostream types
		return false;
	}
}


static int audioBindings(Scripts &scripts, asIScriptEngine *engine)
{
	//Sound type
	AS_CLASS("Sound", sizeof(Sound),
		asOBJ_VALUE | asOBJ_APP_CLASS_CDAK | asOBJ_ASHANDLE);
	{
		//Construct/destroy
		AS_CONSTRUCT("Sound", "void f()",                asFUNCTION(SoundConD));
		AS_CONSTRUCT("Sound", "void f(const Sound &in)", asFUNCTION(SoundCopy));
		AS_DESTRUCT("Sound",                             asFUNCTION(SoundDest));

		//Handle compare
		AS_METHOD("Sound", "bool opEquals(?&in) const",
			asFUNCTION(SoundEquals), PGC_METHOD_OBJFIRST);

		//Assign/compare
		AS_METH("Sound", "bool opEquals(const Sound &in)",
			asMETHODPR(Sound, operator==, (const Sound&) const, bool));
		AS_METH("Sound", "Sound &opAssign(const Sound &in)",
			asMETHODPR(Sound, operator=, (const Sound&), Sound&));
	}


	//Audio type
	AS_CLASS("AUDIO", 0, asOBJ_REF|asOBJ_NOHANDLE);
	{
		//Format
		//AS_METH("AUDIO", "AudioFormat format()", asMETHOD(Audio, format));

		//Mixer controls
		AS_METH("AUDIO", "float get_volume()",
			asMETHODPR(Audio, volume, (), float));
		AS_METH("AUDIO", "void set_volume(float)",
			asMETHODPR(Audio, volume, (float), void));

		AS_METH("AUDIO", "void play(Sound)",
			asMETHODPR(Audio, play, (Sound), void));
		AS_METH("AUDIO", "void play(Sound, float)",
			asMETHODPR(Audio, play, (Sound, float), void));
		AS_METH("AUDIO", "float volume(Sound)",
			asMETHODPR(Audio, volume, (Sound), float));
		AS_METH("AUDIO", "void volume(Sound, float)",
			asMETHODPR(Audio, volume, (Sound, float), void));
		AS_METH("AUDIO", "void pause(Sound)",   asMETHOD(Audio, pause));
		AS_METH("AUDIO", "void stop(Sound)",    asMETHOD(Audio, stop));
		AS_METH("AUDIO", "bool playing(Sound)", asMETHOD(Audio, playing));
		AS_METH("AUDIO", "bool has(Sound)",     asMETHOD(Audio, has));

		//Stream files
		AS_METH("AUDIO", "Sound stream(String, bool loop=false)",
			asMETHODPR(Audio, stream, (String, bool), Sound));

		//Stream microphone
		AS_METH("AUDIO", "Sound microphone()", asMETHOD(Audio, microphone));
	}

	//Singleton-style global
	AS_GLOBAL("AUDIO Audio", scripts.audio);

	return 0;
}
static Bindings bindings(L"Audio", audioBindings);

#endif // PLAIDGADGET


namespace plaidgadget
{
	class AudioImp_Dummy : public AudioImp
	{
	public:
		AudioImp_Dummy(Audio &audio, AudioScheduler &scheduler) :
			AudioImp(audio, scheduler), output(2, 120)
		{
			started = false;
			t = 0.0f;
		}
		virtual ~AudioImp_Dummy() {}

		virtual void startStream() {started = true;}

		virtual AudioFormat format() {return output;}
		virtual double time() {return t;}
		virtual float load() {return 0.0f;}

		virtual void update()
		{
			if (!started) return;

#if PLAIDGADGET
			float tn = t + audio.clock.delta();
#else
			float tn = t + 1.0f;
#endif

			//Pull a trivial chunk of audio
			Sint32 b[4], *out[2] = {b,b+2}, in[2];
			scheduler.render(out, in, 2, t, tn);

			t = tn;
		}

	private:
		AudioFormat output;
		bool started;
		float t;
	};
}

class ExampleGen : public AudioStream
{
public:
	ExampleGen()
	{
		output.channels = 1;
		output.rate = 48000;
		fm = 0.0f;
		wave = 0.0f;
		circ = 2.0f*PI;
		settings.waveFreq = waveBase = 440.0f;
		modRange = 110.0f;
		settings.modFreq = modBase = 110.0f;
		settings.loud = false;
	}

	virtual AudioFormat format()
	{
		return output;
	}

	virtual void tick(Uint64 frame)
	{
		queue.push(settings);
	}

	virtual bool exhausted()
	{
		return false;
	}

	virtual void pull(AudioChunk &chunk)
	{
		if (chunk.first()) queue.pull(cur);

		Sint32 val;
		Sint32 *pos = chunk.start(0), *end = chunk.end(0);

		float step = circ / output.rate;
		float amp = cur.loud ? 5000000.0f : 0.0f;
		while (pos < end)
		{
			//Frequency oscillator
			fm += step * cur.modFreq;
			if (fm > circ) fm -= circ;

			//Output oscillator
			wave += step * (cur.waveFreq + sin(fm) * modRange);
			if (wave > circ) wave -= circ;

			//Output
			val = Sint32(amp * sin(wave));
			*(pos++) = val;
		}
	}

	void control(float mx, float my, bool _loud)
	{
		settings.waveFreq = waveBase * (1.0f + my);
		settings.modFreq = modBase * (1.0f + mx);
		settings.loud = _loud;
	}

private:
	AudioFormat output;

	//Static settings
	float circ;
	float modBase, waveBase;
	float modRange;

	//Dynamic settings
	struct Settings
	{
		bool loud;
		float modFreq, waveFreq;
	};
	Settings settings;
	LockFreeQueue<Settings> queue;

	//Mixerside state
	Settings cur;
	float fm, wave;
};


//Defined below
class Audio::Scratch
{
public:
	Scratch();
	~Scratch();

	void alloc(Sint32 **ptr, Uint32 length, Uint32 channels);
	void release(Sint32 **ptr);

	typedef std::vector<Sint32> Buffer;
	typedef std::map<Sint32*, Uint32> Allocs;
	typedef Allocs::value_type Alloc;

	struct Back
	{
		Buffer buffer;
		Allocs allocs;
		Uint32 resize;
	};
	Back *back;
};


#if PLAIDGADGET
	Audio::Audio(const ModuleSet &modules, bool headless) :
		Module(modules)
#else
	Audio::Audio(bool headless)
#endif //PLAIDGADGET
{
	scheduler = new AudioScheduler(*this);

	//headless = true;

	if (headless)
	{
		imp = new AudioImp_Dummy(*this, *scheduler);
	}
	else
	{
		imp = Implementation_Audio(*this, *scheduler);
	}

	//Set up the scheduler
	scheduler->setupFront(imp->format(), imp->time());
	master = scheduler->master;
	vol = 1.0f;
	master->volume(1.0f);

	//Create scratch buffer
	scratch = new Scratch;

	//Start the audio stream
	imp->startStream();
}

void Audio::update()
{
	/*static ExampleGen *eGen = NULL;
	if (!eGen) {eGen = new ExampleGen(); play(Sound(eGen));}
	eGen->control(mouse().x, mouse().y, mouse.left);*/

#if PLAIDGADGET
	console.monitor() << L"audio: " << int(imp->load()*100.0f+.9f) << std::endl;

	if (keyboard.f6.pressed())
	{
		if (vol < .01f) console(L"audio volume .1");
		else if (vol < .2f) console(L"audio volume 1");
		else console(L"audio volume 0");
	}
#endif

	scheduler->tick(imp->time());
	imp->update();
}

Audio::~Audio()
{
	delete imp;
	delete scheduler;
	delete scratch;
}

float Audio::load()
{
	return imp->load();
}

#if PLAIDGADGET
void Audio::handle(Command &command)
{
	if (command.is(L"hush"))
	{
		if (command.help())
		{
			command.acknowledge();
			console << L"\n'hush' or 'audio hush' silences all audio.\n\n";
		}
		else
		{
			volume(0.0f);
			command.respond(L"Mum's the word!");
		}
	}
	else if (command.is(L"volume"))
	{
		if (command.help())
		{
			command.acknowledge();
			console << L"\n'volume <vol>' sets the volume.\n"
				L"    0.0 is silence, 1.0 is normal.  2.0 is double!\n\n";
		}
		else if (command.parameters() != 2)
		{
			command.respond(L"ERROR: usage is 'volume <vol>'");
		}
		else
		{
			float v;
			if (!Parse(command[1], v))
			{
				command.respond(L"ERROR: 'volume' expects a decimal number.");
			}
			else
			{
				volume(v);
				command.acknowledge();
				console << L"Master volume changed to " << v << L"\n";
			}
		}
	}
}
#endif PLAIDGADGET

AudioFormat Audio::format()
{
	return imp->format();
}

float Audio::volume()
{
	return vol;
}

void Audio::volume(float v)
{
	vol = v;
	master->volume(v);
}

#if PLAIDGADGET
Sound Audio::stream(const File &file, bool loop)
{
	return stream(file.name(), loop);
}
#endif

Sound Audio::stream(String fname, bool loop)
{
	AudioCodec *codec = AudioCodec::Find(fname);
	if (!codec)
	{
		std::wcout << L"Could not find codec for: " << fname << std::endl;
		return Sound::Null();
	}

	return codec->stream(fname, loop);
}

Sound Audio::microphone()
{
	return scheduler->microphone();
}

void Audio::play(Sound stream)
{
	master->play(stream);
}
void Audio::play(Sound stream, float volume)
{
	master->play(stream);
	master->volume(stream, volume);
}
void Audio::pause(Sound stream)
{
	master->pause(stream);
}
void Audio::stop(Sound stream)
{
	master->drop(stream);
}
void Audio::volume(Sound stream, float volume)
{
	master->volume(stream, volume);
}

bool Audio::playing(Sound stream)
{
	return master->playing(stream);
}
bool Audio::has(Sound stream)
{
	return master->has(stream);
}
float Audio::volume(Sound stream)
{
	return master->volume(stream);
}


//=============================================================================
//========    AND THE REST OF THE STUFF   =====================================
//=============================================================================

static const float dbLogMult = 10.0f / 2.30258509f;
static const float multSemitone = 1.0594630943f;
static const float semiLogMult = 1.0f / 0.0577622650f;

float plaidgadget::Decibels(float db)
{
	return std::pow(10.0f, db/10.0f);
}
float plaidgadget::ToDecibels(float mult)
{
	return std::log(mult) * dbLogMult;
}
float plaidgadget::Semitones(float steps)
{
	return std::pow(multSemitone, steps);
}
float plaidgadget::ToSemitones(float mult)
{
	return std::log(mult) * semiLogMult;
}


//------------------------------------------------------------------------

/*
	A very simple nonfragmenting allocator designed for stack-type allocations.

	Used to get scratch space for audio processors.
*/
Audio::Scratch::Scratch()
{
	back = NULL;
}

Audio::Scratch::~Scratch()
{
	if (back) delete back;
}

void Audio::Scratch::alloc(Sint32 **ptr, Uint32 length, Uint32 channels)
{
	Uint32 size = length*channels;
	if (!size) return;

	//Possibly initialize
	if (!back)
	{
		back = new Back();
		back->buffer.resize(1024*128); //1 MB, about 2.7 seconds at stereo 48K
		back->resize = 0;
	}

	//Find starting position, after most recent alloc
	Uint32 start = 0, end;
	if (back->allocs.size()) start = back->allocs.rbegin()->second;
	end = start+size;

	//Is there enough space?
	if (end > back->buffer.size())
	{
		std::cout << back->allocs.size()
			<< " stacked allocs -- Expanding audio-scratch to "
			<< ((end+1023)/1024) << " KB";

		if (!start)
		{
			//Buffer is free; we can resize now
			std::cout << " (immediate)" << std::endl;
			back->buffer.resize(end);
		}
		else
		{
			//Unsafe to resize with buffer in use
			std::cout << " (delayed)" << std::endl;
			back->resize = std::max(back->resize, end);
			return;
		}
	}

	//Allocate
	for (Uint32 i = 0; i < channels; ++i)
		ptr[i] = &back->buffer[start + i*length];
	back->allocs.insert(Alloc(ptr[0], end));
}
void Audio::Scratch::release(Sint32 **ptr)
{
	Allocs::iterator i = back->allocs.find(ptr[0]);
	if (i == back->allocs.end())
	{
		std::cout << "WARNING: NO SUCH ALLOC ON AUDIO SCRATCH" << std::endl;
		return;
	}
	back->allocs.erase(i);
	if (back->allocs.size() == 0 && back->resize)
	{
		back->buffer.resize(back->resize);
		back->resize = 0;
	}
}

void Audio::alloc  (Sint32 **p, Uint32 s, Uint32 c)  {scratch->alloc(p,s,c);}
void Audio::release(Sint32 **p)                      {scratch->release(p);}

AudioChunk::AudioChunk(Audio &_audio, const AudioFormat &format,
	Sint32 **data, Uint32 length, Uint64 frame, float a, float b) :
	audio(_audio), _format(format), _frame(frame),
	_a(a), _b(b), _length(length), _scratch(!data), _cutoff(0)
{
	//TODO move this check to AudioFormat
	if (!_format.channels || _format.channels > PG_MAX_CHANNELS)
		reportError(L"Invalid channel count in AudioChunk!");

	for (Uint32 i = 0; i < PG_MAX_CHANNELS; ++i) _data[i] = NULL;
	if (_scratch)
	{
		audio.alloc(_data, _length, _format.channels);
		if (!_data[0])
		{
			_scratch = false;
			_length = 0;
		}
	}
	else
	{
		for (Uint32 i = 0; i < format.channels; ++i)
		{
			_data[i] = data[i];
		}
	}
}

AudioChunk::~AudioChunk()
{
	if (_scratch) audio.release(_data);
}

//------------------------------------------------------------------------



//Codec registry
typedef std::map<String, AudioCodec*> Codecs;
typedef Codecs::value_type Codec;
static Codecs &codecRegistry() {static Codecs c; return c;}
static bool &lockCodecs() {static bool b = false; return b;}

AudioCodec::AudioCodec(String ext)
{
	if (lockCodecs()) return;
	Codecs &codecs = codecRegistry();

	Uint32 i = 0; String tmp; Char c;
	const Char *arr = ext.c_str();
	while (true)
	{
		c = arr[i]; ++i;
		if (!c || c == ',')
		{
			codecs.insert(Codec(tmp, this));
			tmp = L"";
			if (!c) break;
			continue;
		}
		if (c >= 'A' && c <= 'Z') c += ('a'-'A');
		tmp.push_back(c);
	}
}

void AudioCodec::LockRegistry()
{
	lockCodecs() = true;
}


AudioCodec *AudioCodec::Find(const String &file)
{
	size_t pos = file.find_last_of(L'.');
	if (pos != String::npos) ++pos;
	String ext = file.substr(pos);

	//Case insensitivity -- convert to lowercase
	for (unsigned i = 0; i < ext.size(); ++i)
		if (ext[i] <= 'Z' && ext[i] >= 'A') ext[i] += ('a'-'A');

	Codecs &codecs = codecRegistry();
	Codecs::const_iterator i = codecs.find(ext);
	if (i == codecs.end()) return NULL;
	return i->second;
}
