#ifndef MANAGER_H
#define MANAGER_H

#include <vector>
#include <list>

#include "AudioInterface.h"
#include "RtMIDInterface.h"
#include "Patch.h"

#ifdef USE_PTHREAD
#include <pthread.h>
#endif

class Manager
{
public:
    Manager(unsigned int port = 0);
    ~Manager();

    double getCurrentTime();
private:
    AudioInterface *audioInterface;
    RtMIDIInterface *MIDIInterface;

    std::vector<Patch> channelPatches;

    static void generateCallback(double t,
                                 double dt,
                                 unsigned int numSamples,
                                 float *output,
                                 void *userData);
    static void noteOnCallback(unsigned char channel,
                               unsigned char note,
                               unsigned char vel,
                               void *userData);
    static void noteOffCallback(unsigned char channel,
                                unsigned char note,
                                void *userData);

    std::vector<std::list<Patch> > activeSounds;
    std::vector<Patch> defaultPatches;

    float *delayBuffer;
    unsigned int delayBufferSize;
    unsigned int delayBufferIndex;

#ifdef USE_PTHREAD
    pthread_mutex_t mutex;
#endif

};

#endif // MANAGER_H
