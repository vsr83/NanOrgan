#ifndef AUDIOINTERFACE_H
#define AUDIOINTERFACE_H

#include <portaudio.h>

typedef void (*generateCallbackType)(float t,
                                     float dt,
                                     unsigned numsamples,
                                     float *output,
                                     void *userData);

class AudioInterface {
public:
    AudioInterface(unsigned int _sampleRate = 44100,
                   unsigned int _framesPerBuffer = 64);
    ~AudioInterface();

    void setGenerateCallback(generateCallbackType callback,
                             void *userData);
    void openStream();
    void closeStream();
private:
    void handleError();

    unsigned int sampleRate;
    unsigned int framesPerBuffer;

    static int PaCallback(const void *inputBuffer,
                          void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo *timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void *userData);

    PaStreamParameters outputParameters;
    PaStream *paStream;
    PaError err;

    bool generateCallbackSet;
    generateCallbackType generateCallback;
    void *generateUserData;

    float currentTime;
};

#endif // AUDIOINTERFACE_H
