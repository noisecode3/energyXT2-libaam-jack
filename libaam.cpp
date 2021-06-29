// energyXT - JACK audio interface
// author: Jorgen Aase, Ralf Beck, falkTX and Martin Bångens
// compile: g++ -shared -m32 -Wall -ljack -lQt5Core -I/usr/include/qt libaam.cpp -o /_path_to_energyXT2_/libaam.so

#include "libaam.h"

#include <cstdio>
#include <iostream>

#define HAVE_JACK2 false

bool debug = false;
CProcess* xt_process = 0;

int libaam(int index, int value1, int value2)
{
  int ret = -1;

  switch (index)
  {
  case STREAM_INIT:
      if (debug) std::cout << "STREAM_INIT" << " -> " << value1 << " : " << value2 << std::endl;
      stream_init();
      xt_process = (CProcess*)value1;
      break;
  case STREAM_OPEN:
      if (debug) std::cout << "STREAM_OPEN" << " -> " << value1 << " : " << value2 << std::endl;
      if (value1 >= 0) {
          jack_driver.enabled = true;
          ret = 1;
      }
      break;
  case STREAM_CLOSE:
      if (debug) std::cout << "STREAM_CLOSE" << " -> " << value1 << " : " << value2 << std::endl;
      jack_driver.enabled = false;
      ret = 1;
      break;
  case STREAM_PLAY:
      if (debug) std::cout << "STREAM_PLAY" << " -> " << value1 << " : " << value2 << std::endl;
      break;
  case STREAM_STOP:
      if (debug) std::cout << "STREAM_STOP" << " -> " << value1 << " : " << value2 << std::endl;
      break;
  case STREAM_EXIT:
      if (debug) std::cout << "STREAM_EXIT" << " -> " << value1 << " : " << value2 << std::endl;
      stream_exit();
      break;

  case STREAM_ABOUT:
      if (debug) std::cout << "STREAM_ABOUT" << " -> " << value1 << " : " << value2 << std::endl;
      break;
  case STREAM_AUDIO_COUNT:
      if (debug) std::cout << "STREAM_AUDIO_COUNT" << " -> " << value1 << " : " << value2 << std::endl;
      ret = 1;
      break;
  case STREAM_AUDIO_NAME:
      if (debug) std::cout << "STREAM_AUDIO_NAME" << " -> " << value1 << " : " << value2 << std::endl;
      strcpy((char*)value2, get_audio_name());
      break;

  case STREAM_FRAMES:
      if (debug) std::cout << "STREAM_FRAMES" << " -> " << value1 << " : " << value2 << std::endl;
      if (value1 > 0)
          xt_frames = value1;
      else if (value1 == 0) {
          xt_frames = jack_get_buffer_size(jack_client);
//          *(float*)value2 = xt_frames;
      }
      ret = xt_frames;
      break;
  case STREAM_RATE:
      if (debug) std::cout << "STREAM_RATE" << " -> " << value1 << " : " << value2 << std::endl;
      if (value1 == 0) {
          xt_rate = jack_get_sample_rate(jack_client);
          *(float*)value2 = xt_rate; //jack_nframes_t
      } else if (value1 > 0)
          xt_rate = value1;
      ret = xt_rate;
      break;

  case STREAM_LATENCY:
      if (debug) std::cout << "STREAM_LATENCY" << " -> " << value1 << " : " << value2 << std::endl;
      break;
  case STREAM_RELOAD:
//      if (debug) std::cout << "STREAM_RELOAD" << " -> " << value1 << " : " << value2 << std::endl;
      if (audio_reload) {
          audio_reload = false;
          ret = 1;
      }
      break;

  case MIDI_IN_COUNT:
      if (debug) std::cout << "MIDI_IN_COUNT" << " -> " << value1 << " : " << value2 << std::endl;
      ret = jack_driver.midi_ins.count();
      break;
  case MIDI_IN_NAME:
      if (debug) std::cout << "MIDI_IN_NAME" << " -> " << value1 << " : " << value2 << std::endl;
      strcpy((char*)value2, get_midi_in_name(value1));
      break;
  case MIDI_IN_ENABLE:
      if (debug) std::cout << "MIDI_IN_ENABLE" << " -> " << value1 << " : " << value2 << std::endl;
      if (value1 >= 0 && value1 < jack_driver.midi_ins.count())
          jack_driver.midi_ins[value1].enabled = (bool)value2;
      break;
  case MIDI_IN_ENABLED:
      if (debug) std::cout << "MIDI_IN_ENABLED" << " -> " << value1 << " : " << value2 << std::endl;
      if (value1 >= 0 && value1 < jack_driver.midi_ins.count())
          ret = jack_driver.midi_ins[value1].enabled;
      break;

  case MIDI_OUT_COUNT:
      if (debug) std::cout << "MIDI_OUT_COUNT" << " -> " << value1 << " : " << value2 << std::endl;
      ret = jack_driver.midi_outs.count();
      break;
  case MIDI_OUT_NAME:
      if (debug) std::cout << "MIDI_OUT_NAME" << " -> " << value1 << " : " << value2 << std::endl;
      strcpy((char*)value2, get_midi_out_name(value1));
      break;
  case MIDI_OUT_ENABLE:
      if (debug) std::cout << "MIDI_OUT_ENABLE" << " -> " << value1 << " : " << value2 << std::endl;
      if (value1 >= 0 && value1 < jack_driver.midi_outs.count())
          jack_driver.midi_outs[value1].enabled = (bool)value2;
      break;
  case MIDI_OUT_ENABLED:
      if (debug) std::cout << "MIDI_OUT_ENABLED" << " -> " << value1 << " : " << value2 << std::endl;
      if (value1 >= 0 && value1 < jack_driver.midi_outs.count())
          ret = jack_driver.midi_outs[value1].enabled;
      break;

  case AUDIO_IN_COUNT:
      if (debug) std::cout << "AUDIO_IN_COUNT" << " -> " << value1 << " : " << value2 << std::endl;
      ret = jack_driver.audio_ins.count();
      break;
  case AUDIO_IN_NAME:
      if (debug) std::cout << "AUDIO_IN_NAME" << " -> " << value1 << " : " << value2 << std::endl;
      strcpy((char*)value2, get_audio_in_name(value1));
      break;
  case AUDIO_IN_ENABLE:
      if (debug) std::cout << "AUDIO_IN_ENABLE" << " -> " << value1 << " : " << value2 << std::endl;
      if (value1 >= 0 && value1 < jack_driver.audio_ins.count())
          jack_driver.audio_ins[value1].enabled = (bool)value2;
      break;
  case AUDIO_IN_ENABLED:
      if (debug) std::cout << "AUDIO_IN_ENABLED" << " -> " << value1 << " : " << value2 << std::endl;
      if (value1 >= 0 && value1 < jack_driver.audio_ins.count())
          ret = jack_driver.audio_ins[value1].enabled;
      break;

  case AUDIO_OUT_COUNT:
      if (debug) std::cout << "AUDIO_OUT_COUNT" << " -> " << value1 << " : " << value2 << std::endl;
      ret = jack_driver.audio_outs.count();
      break;
  case AUDIO_OUT_NAME:
      if (debug) std::cout << "AUDIO_OUT_NAME" << " -> " << value1 << " : " << value2 << std::endl;
      strcpy((char*)value2, get_audio_out_name(value1));
      break;
  case AUDIO_OUT_ENABLE:
      if (debug) std::cout << "AUDIO_OUT_ENABLE" << " -> " << value1 << " : " << value2 << std::endl;
      if (value1 >= 0 && value1 < jack_driver.audio_outs.count())
          jack_driver.audio_outs[value1].enabled = (bool)value2;
      break;
  case AUDIO_OUT_ENABLED:
      if (debug) std::cout << "AUDIO_OUT_ENABLED" << " -> " << value1 << " : " << value2 << std::endl;
      if (value1 >= 0 && value1 < jack_driver.audio_outs.count())
          ret = jack_driver.audio_outs[value1].enabled;
      break;
  default:
      if (debug) std::cout << "UNKNOWN (" << index << ")" << " -> " << value1 << " : " << value2 << std::endl;
      break;

  }
  return ret;
}

void stream_init()
{
    audio_reload = false;
    jack_driver.enabled = false;

    jack_client = jack_client_open("energyXT2", JackNullOption, 0);

    if (jack_client)
    {
        jack_driver.active = true;

        int audio_ins = settings.value("Audio-Inputs", 2).toInt();
        int audio_outs = settings.value("Audio-Outputs", 2).toInt();
        int midi_ins = settings.value("MIDI-Inputs", 1).toInt();
        int midi_outs = settings.value("MIDI-Outputs", 1).toInt();

        bool audio_ins_connect = settings.value("Audio-Inputs-Connect", true).toBool();
        bool audio_outs_connect = settings.value("Audio-Outputs-Connect", true).toBool();
        bool midi_ins_connect = settings.value("MIDI-Inputs-Connect", false).toBool();
        bool midi_outs_connect = settings.value("MIDI-Outputs-Connect", false).toBool();

        for (i=0;i<audio_ins && i<MAX_AUDIO_INS;i++) {
            sprintf(name, "audio_in_%2i", i+1);
            struct jack_port port;
            port.port = jack_port_register(jack_client, name, JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
            port.buffer = 0;
            port.name = name;
            port.enabled = false;
            jack_driver.audio_ins.append(port);
        }

        for (i=0;i<audio_outs && i<MAX_AUDIO_OUTS;i++) {
            sprintf(name, "audio_out_%2i", i+1);
            struct jack_port port;
            port.port = jack_port_register(jack_client, name, JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
            port.buffer = 0;
            port.name = name;
            port.enabled = false;
            jack_driver.audio_outs.append(port);
        }

        for (i=0;i<midi_ins && i<MAX_MIDI_INS;i++) {
            sprintf(name, "midi_in_%i", i+1);
            struct jack_port port;
            port.port = jack_port_register(jack_client, name, JACK_DEFAULT_MIDI_TYPE, JackPortIsInput, 0);
            port.buffer = 0;
            port.name = name;
            port.enabled = false;
            jack_driver.midi_ins.append(port);
        }

        for (i=0;i<midi_outs && i<MAX_MIDI_OUTS;i++) {
            sprintf(name, "midi_out_%i", i+1);
            struct jack_port port;
            port.port = jack_port_register(jack_client, name, JACK_DEFAULT_MIDI_TYPE, JackPortIsOutput, 0);
            port.buffer = 0;
            port.name = name;
            port.enabled = false;
            jack_driver.midi_outs.append(port);
        }

        jack_set_process_callback(jack_client, process_callback, 0);
        jack_set_buffer_size_callback(jack_client, buffer_size_callback, 0);
        jack_on_shutdown(jack_client, shutdown_callback, 0);

        if (jack_activate(jack_client)) {
            std::cerr << "failed to activate jack client" << std::endl;
            stream_exit();
        } else {

            if (audio_ins_connect || audio_outs_connect || midi_ins_connect || midi_outs_connect)
            {
                int ains, aouts, mins, mouts;
                ains = 0;
                aouts = 0;
                mins = 0;
                mouts = 0;
                k = -1;

                const char** ports = jack_get_ports(jack_client, 0, 0, JackPortIsPhysical);

                if (ports)
                {
                    while (ports[++k])
                    {
                        jack_port_t* port = jack_port_by_name(jack_client, ports[k]);

                        int port_flags = jack_port_flags(port);

                        if (!port_flags & JackPortIsPhysical)
                            continue;

#if HAVE_JACK2
                        if (audio_ins_connect && ains<jack_driver.audio_ins.count() && port_flags & JackPortIsOutput && jack_port_type_id(port) == JACK_AUDIO_PORT) {
#else
                        if (audio_ins_connect && ains<jack_driver.audio_ins.count() && port_flags & JackPortIsOutput && strstr(jack_port_type(port), "audio")) {
#endif
                            const char* my_port = jack_port_name(jack_driver.audio_ins[ains].port);
                            jack_connect(jack_client, ports[k], my_port);
                            ains++;
                        }

#if HAVE_JACK2
                        if (audio_outs_connect && aouts<jack_driver.audio_outs.count() && port_flags & JackPortIsInput && jack_port_type_id(port) == JACK_AUDIO_PORT) {
#else
                        if (audio_outs_connect && aouts<jack_driver.audio_outs.count() && port_flags & JackPortIsInput && strstr(jack_port_type(port), "audio")) {
#endif
                            const char* my_port = jack_port_name(jack_driver.audio_outs[aouts].port);
                            jack_connect(jack_client, my_port, ports[k]);
                            aouts++;
                        }

#if HAVE_JACK2
                        if (midi_ins_connect && mins<jack_driver.midi_ins.count() && port_flags & JackPortIsOutput && jack_port_type_id(port) == JACK_MIDI_PORT) {
#else
                        if (midi_ins_connect && mins<jack_driver.midi_ins.count() && port_flags & JackPortIsOutput && strstr(jack_port_type(port), "midi")) {
#endif
                            const char* my_port = jack_port_name(jack_driver.midi_ins[mins].port);
                            jack_connect(jack_client, ports[k], my_port);
                            mins++;
                        }

#if HAVE_JACK2
                        if (midi_outs_connect && mouts<jack_driver.midi_outs.count() && port_flags & JackPortIsInput && jack_port_type_id(port) == JACK_MIDI_PORT) {
#else
                        if (midi_outs_connect && mouts<jack_driver.midi_outs.count() && port_flags & JackPortIsInput && strstr(jack_port_type(port), "midi")) {
#endif
                            const char* my_port = jack_port_name(jack_driver.midi_outs[mouts].port);
                            jack_connect(jack_client, my_port, ports[k]);
                            mouts++;
                        }
                    }
                }

                jack_free(ports);
            }


        }


    } else {
        std::cerr << "failed to open jack client" << std::endl;
        jack_client = 0;
        jack_driver.active = false;
    }
}

void stream_exit()
{
    if (jack_client)
    {
        jack_deactivate(jack_client);
        jack_client_close(jack_client);
    }

    jack_client = 0;
    jack_driver.active = false;
}

const char* get_audio_name()
{
#if HAVE_JACK2
    sprintf(name, "JACK Audio v%s", jack_get_version_string());
#else
    sprintf(name, "JACK Audio");
#endif
    return name;
}

const char* get_midi_in_name(int x)
{
    sprintf(name, "midi_in_%i", x+1);
    return name;
}

const char* get_midi_out_name(int x)
{
    sprintf(name, "midi_out_%i", x+1);
    return name;
}

const char* get_audio_in_name(int x)
{
    sprintf(name, "audio_in_%2i", x+1);
    return name;
}

const char* get_audio_out_name(int x)
{
    sprintf(name, "audio_out_%2i", x+1);
    return name;
}

unsigned char* xt_midi_buffer;
unsigned char* jack_midi_buffer;
jack_default_audio_sample_t* xt_inputs[MAX_AUDIO_INS];
jack_default_audio_sample_t* xt_outputs[MAX_AUDIO_OUTS];

int process_callback(jack_nframes_t nframes, void *arg)
{
    if (jack_driver.enabled && jack_client)
    {
        // Audio
        int xt_audio_ins = 0, xt_audio_outs = 0;

        // Audio In
        j = 0;
        for (i=0; i<jack_driver.audio_ins.count() && j<=xt_audio_ins; i++) {
            if (jack_driver.audio_ins[i].enabled) {
                xt_inputs[j++] = (jack_default_audio_sample_t*)jack_port_get_buffer(jack_driver.audio_ins[i].port, nframes);
                xt_audio_ins++;
            }
        }

        // Audio Out
        j = 0;
        for (i=0; i<jack_driver.audio_outs.count() && j<=xt_audio_outs; i++) {
            if (jack_driver.audio_outs[i].enabled) {
                xt_outputs[j++] = (jack_default_audio_sample_t*)jack_port_get_buffer(jack_driver.audio_outs[i].port, nframes);
                xt_audio_outs++;
            }
        }

        // MIDI
        int mcount = 0;
        CMIDIData mbuf[MIDI_BUFFER_SIZE];

        // TESTING
        for (i=0; i<MIDI_BUFFER_SIZE; i++)
        {
            mbuf[i].buffer = 0;
            mbuf[i].data = 0;
            mbuf[i].port = 0;
        }

        // MIDI In
        for (i=0; i<jack_driver.midi_ins.count(); i++) {
            if (jack_driver.midi_ins[i].enabled) {
                jack_driver.midi_ins[i].buffer = jack_port_get_buffer(jack_driver.midi_ins[i].port, nframes);

                int num_jackevents = jack_midi_get_event_count(jack_driver.midi_ins[i].buffer);
                jack_midi_event_t jackevent;

                if(num_jackevents > MIDI_BUFFER_SIZE-mcount)
                    num_jackevents = MIDI_BUFFER_SIZE-mcount;

                for(j=0; j<num_jackevents; j++) {
                    if(jack_midi_event_get(&jackevent, jack_driver.midi_ins[i].buffer, j) != 0)
                        break;

                    mbuf[j].buffer = 0;
                    mbuf[j].data = jackevent.buffer[0] | (jackevent.buffer[1] << 8) | (jackevent.buffer[2] << 16);
                    mbuf[j].port = i+1;
                    mcount += 1;
                }
            }
        }

        // energyXT
        xt_process(xt_inputs, xt_outputs, xt_audio_ins, xt_audio_outs, nframes, &mbuf, mcount);

        // MIDI Out
        for (i=0; i<jack_driver.midi_outs.count(); i++) {
            if (jack_driver.midi_outs[i].enabled) {
                jack_driver.midi_outs[i].buffer = jack_port_get_buffer(jack_driver.midi_outs[i].port, nframes);
                jack_midi_clear_buffer(jack_driver.midi_outs[i].buffer);
            }
        }

        for (i=mcount; i<MIDI_BUFFER_SIZE; i++) {
            if (!mbuf[i].data)
                continue;

            xt_midi_buffer = (unsigned char*)&mbuf[i].data;

            int mode = xt_midi_buffer[0];
            int note = xt_midi_buffer[1];
            int velo = xt_midi_buffer[2];
            int port = (mode & 0x7f) - 0x0f;
            int xt_port = mbuf[i].port;

            // Fix note-off
            if (0x90 >= mode && mode <= 0x9f && velo == 0)
                mode = 0x80+port-1;

            for (j=0; j<jack_driver.midi_outs.count(); j++) {
                if (jack_driver.midi_outs[j].enabled && (xt_port == j+1 || xt_port == -1)) {
                    jack_midi_buffer = jack_midi_event_reserve(jack_driver.midi_outs[j].buffer, i, 3);
                    jack_midi_buffer[0] = mode;
                    jack_midi_buffer[1] = note;
                    jack_midi_buffer[2] = velo;
                }
            }
        }

    }

    return 0;
}

int buffer_size_callback(jack_nframes_t buffer_size, void* arg)
{
    audio_reload = true;
    return 0;
}

void shutdown_callback(void* arg)
{
    jack_client = 0;
    jack_driver.active = false;
}
