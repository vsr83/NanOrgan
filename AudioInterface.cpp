#include "AudioInterface.h"
#include <portaudio.h>
#include <iostream>
#include <stdlib.h>

AudioInterface::AudioInterface(unsigned int _sampleRate, unsigned int _framesPerBuffer) {
    err = Pa_Initialize();
    if (err != paNoError) {
        handleError();
    }

    framesPerBuffer = _framesPerBuffer;
    sampleRate = _sampleRate;
    outputParameters.channelCount = 2;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    generateCallbackSet = false;
    generateUserData = 0;
    currentTime = 0;
}

void
AudioInterface::openStream() {
    err = Pa_OpenStream(&paStream, NULL, &outputParameters, sampleRate,
                        framesPerBuffer, paNoFlag, &AudioInterface::PaCallback,
                        (void*) this);
    if (err != paNoError) handleError();

    err = Pa_StartStream(paStream);
    if (err != paNoError) handleError();

}

void
AudioInterface::closeStream() {
    err = Pa_StopStream(paStream);
    if (err != paNoError) handleError();

    err = Pa_CloseStream(paStream);
    if (err != paNoError) handleError();
}

void
AudioInterface::handleError() {
    Pa_Terminate();
    std::cerr << "PortAudio error " << err << " \"" << Pa_GetErrorText(err) << "\"" << std::endl;
    exit(1);
}

int
AudioInterface::PaCallback(const void *inputBuffer,
                           void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo *timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData) {
    AudioInterface *iface = (AudioInterface *) userData;
    float *out = (float*) outputBuffer;
    float *outMono = new float[framesPerBuffer]();

    iface->currentTime += (float)framesPerBuffer / (float)iface->sampleRate;

    if (iface->generateCallbackSet) {
        (*iface->generateCallback)(iface->currentTime,
                                   1.0/(float)iface->sampleRate,
                                   framesPerBuffer,
                                   outMono,
                                   iface->generateUserData);
    }

    for (unsigned int indSample = 0; indSample < framesPerBuffer; indSample++) {
        out[indSample*2] = outMono[indSample];
        out[indSample*2 + 1] = outMono[indSample];
    }

    delete [] outMono;
    return paContinue;
}

void
AudioInterface::setGenerateCallback(generateCallbackType callback, void *userData) {
    generateCallback = callback;
    generateUserData = userData;
    generateCallbackSet = true;
}
