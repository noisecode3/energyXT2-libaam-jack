#ifndef LIBAAM_H
#define LIBAAM_H

#include <jack/jack.h>
#include <jack/midiport.h>

#include <QtCore/QList>
#include <QtCore/QSettings>

#define MAX_AUDIO_INS  32
#define MAX_AUDIO_OUTS 32
#define MAX_MIDI_INS  4
#define MAX_MIDI_OUTS 4

#define MIDI_BUFFER_SIZE 512

#define JACK_AUDIO_PORT 0
#define JACK_MIDI_PORT 1

const int STREAM_INIT = 0;
const int STREAM_OPEN = 1;
const int STREAM_CLOSE = 2;
const int STREAM_PLAY = 3; //project loaded
const int STREAM_STOP = 4; //project closed
const int STREAM_EXIT = 5;
const int STREAM_ABOUT = 6;
const int STREAM_AUDIO_COUNT = 7;
const int STREAM_AUDIO_NAME = 8;
const int STREAM_FRAMES = 9;
const int STREAM_RATE = 10;
// 11 - 16
const int STREAM_LATENCY = 17;
const int STREAM_RELOAD = 18;
// 19 - 29
const int MIDI_IN_COUNT = 30;
const int MIDI_IN_NAME = 31;
const int MIDI_IN_ENABLE = 32;
const int MIDI_IN_ENABLED = 33;
// 34
const int MIDI_OUT_COUNT = 35;
const int MIDI_OUT_NAME = 36;
const int MIDI_OUT_ENABLE = 37;
const int MIDI_OUT_ENABLED = 38;
// 39
const int AUDIO_IN_COUNT = 40;
const int AUDIO_IN_NAME = 41;
const int AUDIO_IN_ENABLE = 42;
const int AUDIO_IN_ENABLED = 43;
// 44 - 49
const int AUDIO_OUT_COUNT = 50;
const int AUDIO_OUT_NAME = 51;
const int AUDIO_OUT_ENABLE = 52;
const int AUDIO_OUT_ENABLED = 53;

#ifdef WIN32
int libaam(int index, int value1, int value2);
#else
int libaam(int index, int value1, int value2) asm ("libaam");
#endif

typedef int (CProcess) (float** xt_inputs, float** xt_outputs, int xt_nIns, int xt_nOuts, int xt_frames, void* xt_midiData, int xt_midiProducerIndex);

struct CMIDIData
{
    int data;     // midi bytes
    char port;    // midi port index
    void* buffer; // pointer to buffer (sysex etc)
};

int i, j;
unsigned int k;
char name[24];

volatile bool audio_reload;
unsigned int xt_frames, xt_rate;

void stream_init();
void stream_exit();

const char* get_audio_name();
const char* get_midi_in_name(int);
const char* get_midi_out_name(int);
const char* get_audio_in_name(int);
const char* get_audio_out_name(int);

jack_client_t* jack_client;

int process_callback(jack_nframes_t nframes, void *arg);
int buffer_size_callback(jack_nframes_t buffer_size, void* arg);
void shutdown_callback(void *arg);

struct jack_port {
    jack_port_t* port;
    void* buffer;
    const char* name;
    bool enabled;
};

struct jack_driver_t {
    bool active;
    bool enabled;
    QList<struct jack_port> midi_ins;
    QList<struct jack_port> midi_outs;
    QList<struct jack_port> audio_ins;
    QList<struct jack_port> audio_outs;
};

jack_driver_t jack_driver;

QSettings settings("energyXT2", "XT Software AS");

#endif // LIBAAM_H
