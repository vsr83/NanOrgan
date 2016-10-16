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

#ifndef AUDIOINTERFACE_H
#define AUDIOINTERFACE_H

#include <portaudio.h>

typedef void (*generateCallbackType)(double t,
                                     double dt,
                                     unsigned int numSamples,
                                     float *outputL,
                                     float *outputR,
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

    double getCurrentTime();
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

    double currentTime;
};

#endif // AUDIOINTERFACE_H
