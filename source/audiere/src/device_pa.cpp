#include "device_pa.h"
  #include "portaudio.h"
 
 
  namespace audiere {
  
    static const int RATE = 44100;
    static const int FRAMES_PER_BUFFER = 64;
  
  
    PAAudioDevice* PAAudioDevice::create(const ParameterList& parameters) {
  
      PaStreamParameters outputParameters;
      PaStream *stream;
      PaError err;
      err = Pa_Initialize();
  
      if( err != paNoError ) 
        printf(  "PortAudio init error: %s\n", Pa_GetErrorText( err ) );
  
      outputParameters.device = Pa_GetDefaultOutputDevice(); // default output device 
      outputParameters.channelCount = 2;
      outputParameters.sampleFormat = paInt16;
      outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultHighOutputLatency;
      outputParameters.hostApiSpecificStreamInfo = NULL;
  
  
      err = Pa_OpenStream(
  			&stream,
  			NULL, // no input
  			&outputParameters,
  			RATE,
  			FRAMES_PER_BUFFER,
  			paClipOff,      // we won't output out of range samples so don't bother clipping them 
  			NULL,
  			NULL );
      if( err != paNoError ) 
        printf(  "PortAudio open stream error: %s\n", Pa_GetErrorText( err ) );
  
      err = Pa_StartStream( stream );
      if( err != paNoError )
        printf(  "PortAudio start stream error: %s\n", Pa_GetErrorText( err ) );
      
      printf("portaudio device created\n");
      return new PAAudioDevice(stream);
    }
  
    PAAudioDevice::PAAudioDevice(PaStream *stream) : 
      MixerDevice(RATE)
    {
      stream_ = stream;
    }
    
    PAAudioDevice::~PAAudioDevice()
    {
      PaError err;
  
      
      err = Pa_StopStream( stream_ );
      if( err != paNoError ) 
        printf(  "PortAudio stop error: %s\n", Pa_GetErrorText( err ) );
      
      //-- don't forget to cleanup! -- 
      err = Pa_CloseStream( stream_ );
      if( err != paNoError ) 
        	  printf("PortAudio close error: %s\n", Pa_GetErrorText( err ) );
      
      printf("portaudio stopped\n");
      Pa_Terminate();
    }
    
    void ADR_CALL PAAudioDevice::update()
    {
      PaError err = 0;
  
      static const int BUFFER_SIZE = 1024; // nombre de frame a lire
      //short buffer[BUFFER_SIZE * 2]; // 2 channel
      memset(buffer, 0, BUFFER_SIZE * 2 * sizeof(short));
      
      read(BUFFER_SIZE, buffer);

      //buffer[0] = (s16)32767;
      //buffer[1] = (s16)32767;
      err = Pa_WriteStream( stream_, buffer, BUFFER_SIZE);
  //    if( err )
  //      printf("PortAudio update error: %s\n", Pa_GetErrorText( err ) );

      
      /*     FILE *f;
      f = fopen("device.au", "a");
      fwrite(buffer, BUFFER_SIZE * 4, 1, f);
      fclose(f);*/
      
    }
  
    const char*
    PAAudioDevice::getName() {
      return "portAudio";
    }
  
  
  }
