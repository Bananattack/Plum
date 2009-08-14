#ifndef DEVICE_PA_H
#define DEVICE_PA_H
  
  
#include "audiere.h"
  #include "device_mixer.h"
  
  #include "portaudio.h"
  
  namespace audiere {
    
    class PAAudioDevice : public MixerDevice
      {
      public:
        static PAAudioDevice* create(const ParameterList& parameters);
        void ADR_CALL update();
        const char* ADR_CALL getName();
  
      private:
	short buffer[2048]; // 2 channel

        PaStream *stream_;
        PAAudioDevice(PaStream *stream_);
        ~PAAudioDevice();
      };
  }
  
#endif
