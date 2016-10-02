#ifndef RTMIDIINTERFACE_H
#define RTMIDIINTERFACE_H

#include <RtMidi.h>

typedef void (*noteOnCallback)(unsigned char channel,
                               unsigned char note,
                               unsigned char vel,
                               void *userData);
typedef void (*noteOffCallback)(unsigned char channel,
                                unsigned char note,
                                void *userData);

class RtMIDIInterface
{
public:
    RtMIDIInterface();
    ~RtMIDIInterface();

    void setNoteOnCallback(noteOnCallback callback,
                           void *userData);
    void setNoteOffCallback(noteOffCallback callback,
                            void *userData);

    void openPort(unsigned int port);
private:
    RtMidiIn *midiIn;
    unsigned int nPorts;

    static void RtCallback(double deltaTime,
                           std::vector<unsigned char> *message,
                           void *userData);
    noteOnCallback  _noteOnCallback;
    noteOffCallback _noteOffCallback;

    bool noteOnCallbackEnabled;
    bool noteOffCallbackEnabled;

    void *noteOnUserData;
    void *noteOffUserData;
};

#endif // RTMIDIINTERFACE_H
