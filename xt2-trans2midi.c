/* xt-trans2midi.c - output jack transport as midi messages for energyXT */

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <jack/jack.h>
#include <jack/midiport.h>

#define MIDI_BUFFER_SIZE 512

#define CC_PLAY 0x01
#define CC_STOP 0x02
#define CC_REW  0x04
#define CC_BPM  0x05

jack_client_t* client;
jack_port_t* port;

volatile int last_state = -1;
volatile jack_nframes_t last_frame = -1;
volatile double last_bpm = 0.0;

int process_callback(jack_nframes_t nframes, void *arg)
{
  int status[] = { -1, -1, -1 }; // PLAY|STOP; POS; BPM
  jack_position_t cur_pos;

  jack_transport_state_t cur_state = jack_transport_query(client, &cur_pos);

  if (cur_state != last_state) {
    if (cur_state == JackTransportStopped)
      status[0] = CC_STOP;
    else if (cur_state == JackTransportRolling || cur_state == JackTransportStarting)
      status[0] = CC_PLAY;
  }

  if (cur_pos.frame != last_frame && cur_pos.frame == 0) {
    status[1] = CC_REW;
  }

  if (cur_pos.beats_per_minute != last_bpm && cur_pos.beats_per_minute > 0.01) {
    status[2] = CC_BPM;
  }

  last_state = cur_state;
  last_frame = cur_pos.frame;
  last_bpm = cur_pos.beats_per_minute;

  void* port_buffer = jack_port_get_buffer(port, nframes);
  jack_midi_clear_buffer(port_buffer);

  if (status[0] > -1) { // PLAY|STOP
    unsigned char* midi_buffer_0 = jack_midi_event_reserve(port_buffer, 0, 3);
    if (midi_buffer_0) {
      midi_buffer_0[0] = 0xB0;
      midi_buffer_0[1] = status[0];
      midi_buffer_0[2] = 100;
    } else { // handle it in the next process cycle
      last_state = -1;
    }
  }

  if (status[1] > -1) { // REW
    unsigned char* midi_buffer_1 = jack_midi_event_reserve(port_buffer, 1, 3);
    if (midi_buffer_1) {
      midi_buffer_1[0] = 0xB0;
      midi_buffer_1[1] = status[1];
      midi_buffer_1[2] = 100;
    } else { // handle it in the next process cycle
      last_frame = -1;
    }
  }

  if (status[2] > -1) { // BPM
    unsigned char* midi_buffer_2 = jack_midi_event_reserve(port_buffer, 2, 3);
    if (midi_buffer_2) {
      midi_buffer_2[0] = 0xB0;
      midi_buffer_2[1] = status[2];
      midi_buffer_2[2] = (int)last_bpm/2;
    } else { // handle it in the next process cycle
      last_bpm = 0.0;
    }
  }

  return 0;
}

int main(int argc, char *argv[])
{
    /* Connect to JACK */
    client = jack_client_open("XT2 Transport", JackNullOption, 0);
    port = jack_port_register(client, "midi-out", JACK_DEFAULT_MIDI_TYPE, JackPortIsOutput, 0);
    jack_set_process_callback(client, process_callback, 0);

    if (jack_activate(client)) {
      printf("Failed to connecy to JACK\n");
      return 1;
    }

    /* Auto-connect to energyXT MIDI Input */
    unsigned int i = 0;
    const char* my_port = jack_port_name(port);
    const char** xt_ports = jack_get_ports(client, "energyXT2", 0, JackPortIsInput);

    if (xt_ports) {
      while (xt_ports[++i]) {
        if (strstr(jack_port_type(jack_port_by_name(client, xt_ports[i])), "midi")) {
          jack_connect(client, my_port, xt_ports[i]);
        }
      }
      jack_free(xt_ports);
    }

    /* Wait until closed */
    while (1) {
      sleep(1);
    }

    /* Disconnect from JACK */
    jack_deactivate(client);
    jack_client_close(client);

    return 0;
}
