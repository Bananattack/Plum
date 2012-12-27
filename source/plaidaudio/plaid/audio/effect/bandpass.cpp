#if PLAIDGADGET
#include "../../geometry/2d.h"
#else
#define PI 3.141592654f
#endif

#include "../effects.h"


using namespace plaidgadget;
using namespace std;


Bandpass::Bandpass(Signal source, float low, float high) :
	AudioEffect<Bandpass_Node>(source, Bandpass_Node(low, high))
{
	hpD = new float[source.format().channels];
	lpP = new float[source.format().channels];
}
Bandpass::~Bandpass()
{
	delete hpD;
	delete lpP;
}


void Bandpass::bandpass(float lowFreq, float highFreq)
{
	//Adjust frequencies
	settings.low = lowFreq;
	settings.high = highFreq;
}
void Bandpass::lowpass(float freq)   {bandpass(freq, 0.0f);}
void Bandpass::highpass(float freq)  {bandpass(0.0f, freq);}
void Bandpass::off()                 {bandpass(0.0f, 0.0f);}

const float RCCONV = .5f / PI;

void Bandpass::pull(AudioChunk &chunk,
	const Bandpass_Node &a, const Bandpass_Node &b)
{
	//Pull source data
	source.pull(chunk);

	//Calculate RC multipliers
	float
		al = RCCONV / ((a.low<=0.0f)?40000.0f:a.low),
		ah = RCCONV / ((a.high<=0.0f)?10.0f:a.high),
		bl = RCCONV / ((b.low<=0.0f)?40000.0f:b.low),
		bh = RCCONV / ((b.high<=0.0f)?10.0f:b.high);
	float lpRC = al, hpRC = ah,
		lpM = pow(bl/al, 1.0f / float(chunk.length())),
		hpM = pow(bh/ah, 1.0f / float(chunk.length())),
		lpA, hpA, samp,
		dt = 1.0f / float(chunk.format().rate);

	//Apply effect!
	Uint32 chan = source.format().channels;

	for (Uint32 i = 0; i < chan; ++i)
	{
		Sint32 *pos = chunk.start(i), *end = chunk.end(i);
		float &lpPc = lpP[i], &hpDc = hpD[i];
		while (pos < end)
		{
			//Interpolate settings
			lpA = dt   / (lpRC + dt); lpRC *= lpM;
			hpA = hpRC / (hpRC + dt); hpRC *= hpM;

			//Get samples
			samp = float(*pos);

			//Lowpass
			samp = lpPc + lpA * (samp-lpPc);
			lpPc = samp;

			//Highpass (confusing but correct)
			samp = hpA * (samp+hpDc);
			hpDc = samp - lpPc;

			//Set samples
			*pos = Sint32(samp);
			++pos;
		}
	}
}

Bandpass_Node Bandpass::interpolate(
	const Bandpass_Node &a, const Bandpass_Node &b, float mid)
{
	float
		al = ((a.low<=0.0f)?40000.0f:a.low),
		ah = ((a.high<=0.0f)?10.0f:a.high),
		bl = ((b.low<=0.0f)?40000.0f:b.low),
		bh = ((b.high<=0.0f)?10.0f:b.high);
	return Bandpass_Node(al*std::pow(bl/al,mid),ah*std::pow(bh/ah,mid));
}
