#ifndef DEVICE_COREAUDIO_H
#define DEVICE_COREAUDIO_H


#include "audiere.h"
#include "device_mixer.h"
#include <AudioUnit/AudioUnit.h>
#ifdef AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER
#include <AudioUnit/AUNTComponent.h>
#endif


namespace audiere {

  class CAAudioDevice : public MixerDevice {
  public:
    static CAAudioDevice* create(const ParameterList& parameters);

  private:
    CAAudioDevice(ComponentInstance output_audio_unit);
    ~CAAudioDevice();

  public:
    void ADR_CALL update();
    const char* ADR_CALL getName();

    static OSStatus fillInput(void                        *inRefCon,
			      AudioUnitRenderActionFlags  inActionFlags,
			      const AudioTimeStamp        *inTimeStamp,
			      UInt32                      inBusNumber,
			      AudioBuffer                 *ioData);

  private:
      ComponentInstance m_output_audio_unit;
  };

}


#endif
