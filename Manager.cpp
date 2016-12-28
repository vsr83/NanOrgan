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

#include "Manager.h"
#include "AudioInterface.h"
#include "RtMIDInterface.h"
#include "Patch.h"

#include <iterator>
#include <list>
#include <vector>
#include <iostream>
#include <math.h>
#include <assert.h>

Manager::Manager(unsigned int port) {
    for (unsigned int channel = 0; channel < 16; channel++) {
        std::list<Patch> patchlist;
        Patch patch;
#ifdef FREEVERB
        reverbEnabled = true;
        delayEnabled = false;
        rm.setroomsize(0.5f);
        rm.setwet(0.7f);
        rm2.setroomsize(0.05f);
        rm2.setwet(0.7f);
#else
        delayEnabled = true;
#endif
        if (channel == 1) {
            // Quintadena 8'
            patch.timbreAmplitudes = {0.3981072, 0.0251189, 1.0, 0.0251189, 0.0375837, 0.0281838, 0.0112202, 0.0112202, 0.0031623, 0.0112202};
            patch.waveType = {WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN,
                              WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN};
         }
        if (channel == 2) {
            // Subbas 16'
            //patch.timbreAmplitudes = {1.0, 0.0223872, 0.0891251, 0.0223872, 0.0014125, 0.0079433, 0.0039811, 0.0079433, 0.0015849, 0.0039811};
            //patch.waveType = {WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN,
            //                  WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN};
        }
        if (channel == 3) {
            // Principal 8'
            patch.timbreAmplitudes = {1.0, 0.7087858, 0.3166030, 0.0708786, 0.0563009, 0.355234, 0.0089231, 0.0141421, 0.0100119, 0.0447214};
            patch.waveType = {WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN,
                              WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN};
            patch.timbreAmplitudes = {1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
            patch.waveType = {WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN,
                              WAVE_SIN, WAVE_SIN, WAVE_SIN};
        }
        if (channel == 4) {
            // Principal 8' + Subbas 16'
            patch.timbreAmplitudes = {1.0, 0.3655865, 0.2028641, 0.0466329, 0.0288567, 0.1815886, 0.0064521, 0.110427, 0.0057984, 0.0243513};
            patch.waveType = {WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN,
                              WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN};
        }
        if (channel == 5) {
            // Subbas 8' + Quintadena 8'
            patch.timbreAmplitudes = {1.0, 0.0399800, 0.779026, 0.03398, 0.0278931, 0.0258409, 0.0108731, 0.0137072, 0.0033956, 0.0108731};
            patch.waveType = {WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN,
                              WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN};
        }
        if (channel == 6) {
            // Principal 8' + Subbas 8' + Quintadena 8'
            patch.timbreAmplitudes = {0.625, 1.0, 0.375, 0.5625, 0.5625};
            patch.waveType = {WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN};
        }
        if (channel == 7) {
            patch.timbreAmplitudes = {1.0};
            patch.waveType = {WAVE_SIN};
            patch.fmodEnabled = true;
            patch.fmodAmpl = 0.005;
            patch.fmodFreq = 0.5;
        }
        if (channel == 8) {
            patch.timbreAmplitudes = {1.0};
            patch.waveType = {WAVE_SIN};
            patch.fmodEnabled = true;
            patch.fmodAmpl = 0.005;
            patch.fmodFreq = 0.5;
            patch.envelope.sustainAmplitude = 0.25;
            patch.envelope.attackTime = 0.125;
            patch.envelope.decayTime = 0.5;
            patch.envelope.releaseTime = 0.25;
        }
        if (channel == 9) {
            patch.timbreAmplitudes = {1.0, 1.0};
            patch.waveType = {WAVE_SAW, WAVE_SAW};
            patch.timbreCoeff = {1.0, 1.01};
            //patch.fmodEnabled = true;
            //patch.fmodAmpl = 0.05;
            //patch.fmodFreq = 1.0;
            patch.envelope.sustainAmplitude = 0.5;
            patch.envelope.attackTime = 0.25;
            patch.envelope.decayTime = 0.5;
            patch.envelope.releaseTime = 0.25;
        }
        if (channel == 10) {
            patch.timbreAmplitudes = {1.0, 0.5};
            patch.waveType = {WAVE_SIN, WAVE_SIN};
            patch.fmodEnabled = true;
            patch.fmodAmpl = 0.05;
            patch.fmodFreq = 1.0;
            patch.envelope.sustainAmplitude = 0.025;
            patch.envelope.attackTime = 0.020;
            patch.envelope.decayTime = 0.5;
            patch.envelope.releaseTime = 0.25;
        }
        if (channel == 11) {
            patch.timbreAmplitudes = {1.0};
            patch.waveType = {WAVE_SIN};
            patch.fmodEnabled = true;
            patch.fmodAmpl = 0.01;
            patch.fmodFreq = 2.0;
            patch.envelope.sustainAmplitude = 0.25;
            patch.envelope.attackTime = 0.25;
            patch.envelope.decayTime = 0.5;
            patch.envelope.releaseTime = 0.25;
        }

        if (channel == 12) {
            patch.timbreAmplitudes = {1.0};
            patch.waveType = {WAVE_SAW};
            patch.envelope.sustainAmplitude = 0.5;
            patch.envelope.attackTime = 0.1;
            patch.envelope.decayTime = 0.05;
            patch.envelope.releaseTime = 0.25;
        }
        if (channel == 13) {
            patch.timbreAmplitudes = {1.0};
            patch.waveType = {WAVE_TRI};
        }
        if (channel == 14) {
            patch.timbreAmplitudes = {0.6, 0.4, 0.6};
            patch.waveType = {WAVE_TRI, WAVE_SQU, WAVE_SAW};
        }
        if (channel == 15) {
            patch.timbreAmplitudes = {0.9, 0.5, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6};
            patch.waveType = {WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN,
                              WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN};
        }
        for (int ind_timbre = 0; ind_timbre < 16; ind_timbre++) {
            if (patch.timbreAmplitudes.size() < 16) {
                patch.timbreAmplitudes.push_back(0);
                patch.waveType.push_back(WAVE_SIN);
            }
            pedalTimbres[channel][ind_timbre] = false;
        }
        activeSounds.push_back(patchlist);
        activePatches.push_back(patch);
    }

    sustainStatus = false;
    mostRecentChannel = 0;
    delayBufferSize = 44100*2;
    delayBuffer = new float[delayBufferSize];
    for (unsigned int ind = 0; ind < delayBufferSize; ind++)
        delayBuffer[ind] = 0.0;

#ifdef USE_PTHREAD
    assert(pthread_mutex_init(&mutex, NULL) == 0);
#endif

    std::cout << "Initialization of Audio Interface" << std::endl;
    audioInterface = new AudioInterface(44100, 64);
    audioInterface->setGenerateCallback(&Manager::generateCallback, this);
    audioInterface->openStream();

    std::cout << "Initialization of RtMIDI" << std::endl;
    MIDIInterface = new RtMIDIInterface();

    MIDIInterface->openPort(port);
    MIDIInterface->setNoteOnCallback(&noteOnCallback, this);
    MIDIInterface->setNoteOffCallback(&noteOffCallback, this);
    MIDIInterface->setChannelModeCallback(&channelModeCallback, this);
}

Manager::~Manager() {
    delete MIDIInterface;
    delete audioInterface;
    MIDIInterface = 0;
    audioInterface = 0;

#ifdef USE_PTHREAD
    pthread_mutex_destroy(&mutex);
#endif
    delete [] delayBuffer;
    delayBuffer = 0;
}

double
Manager::getCurrentTime() {
    return audioInterface->getCurrentTime();
}

void
Manager::generateCallback(double t, double dt, unsigned int numSamples, float *outputLeft, float *outputRight, void *userData) {
    Manager *manager = (Manager*) userData;

#ifdef USE_PTHREAD
    pthread_mutex_lock(&manager->mutex);
#endif

    float *outputMono  = new float[numSamples];
    float *outputMonoAfterDelay  = new float[numSamples];


    for (unsigned int sample = 0; sample < numSamples; sample++) {
        outputMono[sample] = 0.0;
    }

    // Iterate through all active sounds on all channels, evaluate them and mix
    // the results.
    for (unsigned int channel = 0; channel < 16; channel++) {
        for (std::list<Patch>::iterator it = manager->activeSounds[channel].begin();
             it != manager->activeSounds[channel].end();) {

            if ((*it).isFinished()) {
                manager->activeSounds[channel].erase(it++);
            } else {
                for (unsigned int sample = 0; sample < numSamples; sample++) {
                    outputMono[sample] += 0.1 * (*it).eval(manager->getCurrentTime() + dt*sample);
                }
                ++it;
            }
        }
    }

    if (manager->delayEnabled) {
        // Apply the built-in delay effect.
        for (unsigned int sample = 0; sample < numSamples; sample++)
        {
            // Store previous samples in a ring buffer.
            manager->delayBuffer[manager->delayBufferIndex] = outputMono[sample];
            manager->delayBufferIndex = (manager->delayBufferIndex + 1) % manager->delayBufferSize;
        }

        for (unsigned int sample = 0; sample < numSamples; sample++)
        {
            outputMonoAfterDelay[sample] = outputMono[sample];

            // Add exponentially attenuated previous samples to each sample.
            for (unsigned int delayfac = 1; delayfac < 10; delayfac++) {
                unsigned int delayIndex = (manager->delayBufferIndex - numSamples + sample - delayfac*5000 + manager->delayBufferSize) % manager->delayBufferSize;
                outputMonoAfterDelay[sample] += powf(0.5, delayfac)*manager->delayBuffer[delayIndex];
            }
        }
    } else {
        for (unsigned int sample = 0; sample < numSamples; sample++) {
            outputMonoAfterDelay[sample] = outputMono[sample];
        }
    }

#ifdef FREEVERB
    if (manager->reverbEnabled) {
        manager->rm.processreplace(outputMonoAfterDelay, outputMonoAfterDelay, outputLeft, outputRight, numSamples, 1);
    } else {
        for (unsigned int sample = 0; sample < numSamples; sample++) {
            outputLeft[sample] = outputMonoAfterDelay[sample];
            outputRight[sample] = outputMonoAfterDelay[sample];
        }
    }
#else
    for (unsigned int sample = 0; sample < numSamples; sample++) {
        outputLeft[sample] = outputMonoAfterDelay[sample];
        outputRight[sample] = outputMonoAfterDelay[sample];
    }
#endif
    delete [] outputMono;
    delete [] outputMonoAfterDelay;
#ifdef USE_PTHREAD
    pthread_mutex_unlock(&manager->mutex);
#endif
}

/* The handling of Channel Mode messages is based on the MIDI output of an
 * Akai MPK261 MIDI keyboard so that:
 * - The sliders F1-F8 are configured to set the timbre of the waveform.
 * - The knobs K1-K8 configure that ADSR and FM parameters.
 * - The pitch bend wheel configures the room size of the reverb.
 */
void
Manager::channelModeCallback(unsigned char byte1, unsigned char byte2, unsigned char bank, void *userData) {
    Manager *manager = (Manager*) userData;
#ifdef USE_PTHREAD
    pthread_mutex_lock(&manager->mutex);
#endif

    unsigned char mostRecentChannel = manager->mostRecentChannel;

    if (byte1 == 1) { // pitch bend
#ifdef FREEVERB
        manager->rm.setroomsize(1.0f*((float)byte2)/127.0);
//        manager->rm.setwet(1.0f*((float)byte2)/127.0);
        manager->rm.setwet(0.7f);
#endif

    }

    if (byte1 == 64) { // pitch bend
        manager->sustainStatus = (byte2 == 127);
        std::cout << "Sustain Status: " << manager->sustainStatus << std::endl;
    }

    if (byte1 >= 12 && byte1 <= 19) { // sliders 1-8
        int knob = byte1 - 12;
        double value = ((float)byte2)/127.0;

        std::cout << knob << " " << value << std::endl;
        if (bank == 0) {
            manager->activePatches[mostRecentChannel].timbreAmplitudes[knob] = ((float)byte2)/127.0;
        } else if (bank == 1) {
            manager->activePatches[mostRecentChannel].timbreAmplitudes[knob + 8] = ((float)byte2)/127.0;
        }
    }
    if (byte1 >= 22 && byte1 <= 29) { // knobs 1-8
        int knob = byte1 - 22;
        double value = ((float)byte2 + 1.0)/127.0;
        std::cout << knob << " " << value << std::endl;

        switch (knob) {
        case 0:
            manager->activePatches[mostRecentChannel].envelope.attackTime = value * 1.0;
            break;
        case 1:
            manager->activePatches[mostRecentChannel].envelope.decayTime = value * 1.0;
            break;
        case 2:
            manager->activePatches[mostRecentChannel].envelope.releaseTime = value * 1.0;
            break;
        case 3:
            manager->activePatches[mostRecentChannel].envelope.peakAmplitude = value;
            break;
        case 4:
            manager->activePatches[mostRecentChannel].envelope.sustainAmplitude = value;
            break;
        case 5:
        {
            double value = ((float)byte2)/127.0;
            manager->activePatches[mostRecentChannel].fmodEnabled = true;
            manager->activePatches[mostRecentChannel].fmodAmpl = value;
        }
            break;
        case 6:
            manager->activePatches[mostRecentChannel].fmodEnabled = true;
            manager->activePatches[mostRecentChannel].fmodFreq = value * 4.0;
            break;
        }
        std::cout << "attackTime = " << manager->activePatches[mostRecentChannel].envelope.attackTime
                  << ", decayTime = " << manager->activePatches[mostRecentChannel].envelope.decayTime
                  << ", sustainAmpl = " << manager->activePatches[mostRecentChannel].envelope.sustainAmplitude
                  << ", releaseTime = " << manager->activePatches[mostRecentChannel].envelope.releaseTime
                  << ", fmodfreq = " << manager->activePatches[mostRecentChannel].fmodFreq
                  << ", fmodampl = " << manager->activePatches[mostRecentChannel].fmodAmpl
                  << std::endl;
    }
    if (byte1 >= 32 && byte1 <= 39) { // slider buttons 1-8
        int knob = byte1 - 32;
        bool enabled = (byte2 == 127);
        if (bank == 0) {
            manager->pedalTimbres[manager->mostRecentChannel][knob] = enabled;
        } else if (bank == 1) {
            manager->pedalTimbres[manager->mostRecentChannel][knob + 8] = enabled;
        }
        //manager->activePatches[mostRecentChannel].waveType[knob] = (manager->activePatches[mostRecentChannel].waveType[knob] + 1) % 4;
    }

#ifdef USE_PTHREAD
    pthread_mutex_unlock(&manager->mutex);
#endif

}

void
Manager::noteOnCallback(unsigned char channel, unsigned char note, unsigned char vel, void *userData) {
    Manager *manager = (Manager*) userData;

#ifdef USE_PTHREAD
    pthread_mutex_lock(&manager->mutex);
#endif
    // Set the channel with most recent Note On data:
    manager->mostRecentChannel = channel;

    // Inherit patch data according to channel of the MIDI On message.
    Patch patch;
    patch = manager->activePatches[channel];

    for (int ind_timbre = 0; ind_timbre < patch.timbreAmplitudes.size(); ind_timbre++) {
        if (manager->sustainStatus && manager->pedalTimbres[channel][ind_timbre]) {
            patch.timbreAmplitudes[ind_timbre] = 0.0;
        }
    }

    // Set sound to ATTACK state and add it to the list of active sounds.
    patch.trigger(note, vel, manager->getCurrentTime());
    manager->activeSounds[channel].push_back(patch);

    std::cout << "NOTEON ch=" << (int)channel << " note=" << (int)note << std::endl;
#ifdef USE_PTHREAD
    pthread_mutex_unlock(&manager->mutex);
#endif
}

void
Manager::noteOffCallback(unsigned char channel, unsigned char note, void *userData) {
    Manager *manager = (Manager*) userData;
#ifdef USE_PTHREAD
    pthread_mutex_lock(&manager->mutex);
#endif

    std::cout << "NOTEOFF ch=" << (int)channel << " note=" << (int)note << std::endl;
    // Switch sound to RELEASED state.

    for (std::list<Patch>::iterator it = manager->activeSounds[channel].begin();
         it != manager->activeSounds[channel].end(); it++) {
        if ((*it).note == note)
            (*it).release(manager->getCurrentTime());
    }
#ifdef USE_PTHREAD
    pthread_mutex_unlock(&manager->mutex);
#endif
}
