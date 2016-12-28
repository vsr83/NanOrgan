/*  NanOrgan - Simple Organ Synthesizer
 *
 *  Copyright (C) 2016 Ville Räisänen <vsr at vsr.name>
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
    outputParameters.device = Pa_GetDefaultOutputDevice();
    if (outputParameters.device == paNoDevice) {
        std::cerr << "Error: No default device";
        handleError();
    }
    outputParameters.channelCount = 2;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    generateCallbackSet = false;
    generateUserData = 0;
    currentTime = 0;
}

AudioInterface::~AudioInterface() {
    closeStream();
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

double
AudioInterface::getCurrentTime() {
    return currentTime;
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
    float *outLeft  = new float[framesPerBuffer]();
    float *outRight = new float[framesPerBuffer]();
    double dt = 1.0/(double)iface->sampleRate;

    if (iface->generateCallbackSet) {
        (*iface->generateCallback)(iface->currentTime,
                                   dt,
                                   framesPerBuffer,
                                   outLeft,
                                   outRight,
                                   iface->generateUserData);
    }

    for (unsigned int indSample = 0; indSample < framesPerBuffer; indSample++) {
        out[indSample*2] = outLeft[indSample];
        out[indSample*2 + 1] = outRight[indSample];
    }
    iface->currentTime += dt*(double)framesPerBuffer;

    delete [] outLeft;
    delete [] outRight;
    return paContinue;
}

void
AudioInterface::setGenerateCallback(generateCallbackType callback, void *userData) {
    generateCallback = callback;
    generateUserData = userData;
    generateCallbackSet = true;
}
