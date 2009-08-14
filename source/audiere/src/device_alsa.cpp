#include <algorithm>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <alsa/asoundlib.h>
#include "device_alsa.h"
#include "debug.h"


namespace audiere {

  ALSAAudioDevice*
  ALSAAudioDevice::create(const ParameterList& parameters) {
    std::string devices[] = {
      "plughw:0,0", "hw:0,0", ""
    };
    std::string default_device = parameters.getValue("device", "default");

    int status = 0;
    snd_pcm_t* pcm_handle = 0;

    status = snd_pcm_open(&pcm_handle, default_device.c_str(),
			  SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK);
    if (status < 0 && default_device == "default") {
      for (int i = 0; devices[i].length() > 0; i++) {
	status = snd_pcm_open(&pcm_handle, devices[i].c_str(),
			      SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK);
	if (status >= 0)
	  break;
      }
    }
    if (status < 0) {
      ADR_LOG("Coudn't open hardware.");
      return 0;
    }

    int rate = 48000;
    status = snd_pcm_set_params(pcm_handle,
                                SND_PCM_FORMAT_S16_LE,
                                SND_PCM_ACCESS_RW_INTERLEAVED,
                                2, rate, 1, 0);
    if (status < 0) {
      rate = 441000;
      status = snd_pcm_set_params(pcm_handle,
                                  SND_PCM_FORMAT_S16_LE,
                                  SND_PCM_ACCESS_RW_INTERLEAVED,
                                  2, rate, 1, 0);
      if (status < 0) {
        ADR_LOG("Couldn't set audio device parameters.");
        snd_pcm_close(pcm_handle);
        return 0;
      }
    }

    snd_pcm_uframes_t buffer_size;
    snd_pcm_uframes_t period_size;
    status = snd_pcm_get_params(pcm_handle, &buffer_size, &period_size);
    if (status) {
      ADR_LOG("Couldn't get audio device parameters.");
      snd_pcm_close(pcm_handle);
      return 0;
    }

    return new ALSAAudioDevice(pcm_handle, rate, 4096);
  }


  ALSAAudioDevice::ALSAAudioDevice(snd_pcm_t* pcm_handle,
                                   int rate,
                                   int buffer_size)
    : MixerDevice(rate)
  {
    m_pcm_handle = pcm_handle;
    m_buffer_size = buffer_size;
    m_buffer = new char [buffer_size];
  }


  ALSAAudioDevice::~ALSAAudioDevice() {
    ADR_GUARD("ALSAAudioDevice::~ALSAAudioDevice");
    snd_pcm_drain(m_pcm_handle);
    snd_pcm_close(m_pcm_handle);
    delete [] m_buffer;
  }


  void ADR_CALL
  ALSAAudioDevice::update() {
    int           ret;
    int           sample_len;
    int           sample_left;
    char*         sample_buf;

    sample_buf = m_buffer;
    sample_len = m_buffer_size / 4;

    sample_left = read(sample_len, sample_buf);
    while (sample_left > 0) {
      ret = snd_pcm_writei(m_pcm_handle, sample_buf, sample_left);
      if (ret == -EAGAIN || (ret > 0 && ret < sample_left)) {
        snd_pcm_wait(m_pcm_handle, 10);
      } else if (ret == -ESTRPIPE) {
        do {
          snd_pcm_wait(m_pcm_handle, 10);
          ret = snd_pcm_resume(m_pcm_handle);
        } while (ret == -EAGAIN);
        snd_pcm_prepare(m_pcm_handle);
      } else if (ret == -EPIPE) {
        snd_pcm_prepare(m_pcm_handle);
      }
      if (ret > 0) {
        sample_buf += ret * 4;
        sample_left -= ret;
      }
    }
  }


  const char* ADR_CALL
  ALSAAudioDevice::getName() {
    return "alsa";
  }

}
