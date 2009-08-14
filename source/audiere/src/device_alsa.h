#ifndef DEVICE_ALSA_H
#define DEVICE_ALSA_H

#include "audiere.h"
#include "device_mixer.h"

#include <alsa/asoundlib.h>


namespace audiere {

  class ALSAAudioDevice : public MixerDevice {
  public:
    static ALSAAudioDevice* create(const ParameterList& parameters);

  private:
    ALSAAudioDevice(snd_pcm_t* pcm_handle,
                    int rate,
                    int buffer_size);
    ~ALSAAudioDevice();

  public:
    void ADR_CALL update();
    const char* ADR_CALL getName();

  private:
    snd_pcm_t* m_pcm_handle;
    int m_buffer_size;
    char* m_buffer;
  };

}


#endif
