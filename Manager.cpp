#include "Manager.h"
#include "AudioInterface.h"
#include "RtMIDInterface.h"
#include "Patch.h"

#include <iterator>
#include <list>
#include <vector>
#include <iostream>
#include <math.h>

Manager::Manager(unsigned int port) {
    for (unsigned int channel = 0; channel < 16; channel++) {
        std::list<Patch> patchlist;
        Patch patch;

        if (channel == 1) {
            // Quintadena 8'
            patch.timbre = {0.3981072, 0.0251189, 1.0, 0.0251189, 0.0375837, 0.0281838, 0.0112202, 0.0112202, 0.0031623, 0.0112202};
            patch.waveType = {WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN,
                              WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN};
        }
        if (channel == 2) {
            // Subbas 16'
            patch.timbre = {1.0, 0.0223872, 0.0891251, 0.0223872, 0.0014125, 0.0079433, 0.0039811, 0.0079433, 0.0015849, 0.0039811};
            patch.waveType = {WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN,
                              WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN};
        }
/*        if (channel == 3) {
            // Principal 8'
            patch.timbre = {1.0, 0.7087858, 0.3166030, 0.0708786, 0.0563009, 0.355234, 0.0089231, 0.0141421, 0.0100119, 0.0447214};
            patch.waveType = {WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN,
                              WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN};
        }
        if (channel == 4) {
            // Principal 8' + Subbas 16'
            patch.timbre = {1.0, 0.3655865, 0.2028641, 0.0466329, 0.0288567, 0.1815886, 0.0064521, 0.110427, 0.0057984, 0.0243513};
            patch.waveType = {WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN,
                              WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN};
        }*/
        if (channel == 3) {
            // Principal 8' + Quintadena 8'
            patch.timbre = {1.0, 0.5249451, 0.9417357, 0.0686648, 0.0671535, 0.2742499, 0.014408, 0.0181411, 0.009232, 0.0400137};
            patch.waveType = {WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN,
                              WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN};
        }
/*        if (channel == 5) {
            // Subbas 8' + Quintadena 8'
            patch.timbre = {1.0, 0.0399800, 0.779026, 0.03398, 0.0278931, 0.0258409, 0.0108731, 0.0137072, 0.0033956, 0.0108731};
            patch.waveType = {WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN,
                              WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN};
        }*/
        if (channel == 4) {
            // Principal 8' + Subbas 8' + Quintadena 8'
            patch.timbre = {1.0, 0.3151216, 0.58572, 0.049327, 0.0397071, 0.1630671, 0.0100518, 0.0138773, 0.006149, 0.0246978};
            patch.waveType = {WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN,
                              WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN};
        }
        if (channel == 13) {
            patch.timbre = {0.6};
            patch.waveType = {WAVE_TRI};
        }
        if (channel == 14) {
            patch.timbre = {0.6, 0.4, 0.6};
            patch.waveType = {WAVE_TRI, WAVE_SQU, WAVE_SAW};
        }
        if (channel == 15) {
            patch.timbre = {0.9, 0.5, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6};
            patch.waveType = {WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN,
                              WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN};
        }
        activeSounds.push_back(patchlist);
        defaultPatches.push_back(patch);
    }

    delayBufferSize = 44100*2;
    delayBuffer = new float[delayBufferSize];
    for (unsigned int ind = 0; ind < delayBufferSize; ind++)
        delayBuffer[ind] = 0.0;

    std::cout << "Initialization of Audio Interface" << std::endl;
    audioInterface = new AudioInterface(44100, 64);
    audioInterface->setGenerateCallback(&Manager::generateCallback, this);
    audioInterface->openStream();

    std::cout << "Initialization of RtMIDI" << std::endl;
    MIDIInterface = new RtMIDIInterface();

    MIDIInterface->openPort(port);
    MIDIInterface->setNoteOnCallback(&noteOnCallback, this);
    MIDIInterface->setNoteOffCallback(&noteOffCallback, this);
}

Manager::~Manager() {
    delete MIDIInterface;
    delete audioInterface;
    MIDIInterface = 0;
    audioInterface = 0;

    delete [] delayBuffer;
    delayBuffer = 0;
}

double
Manager::getCurrentTime() {
    return audioInterface->getCurrentTime();
}

void
Manager::generateCallback(double t, double dt, unsigned int numSamples, float *output, void *userData) {
    Manager *manager = (Manager*) userData;

    float *outputTmp = new float[numSamples];
    for (unsigned int sample = 0; sample < numSamples; sample++) {
        outputTmp[sample] = 0.0;
        output[sample] = 0.0;
    }

    for (unsigned int channel = 0; channel < 16; channel++) {
        for (std::list<Patch>::iterator it = manager->activeSounds[channel].begin();
             it != manager->activeSounds[channel].end();) {

            if ((*it).isFinished()) {
                manager->activeSounds[channel].erase(it++);
            } else {
                for (unsigned int sample = 0; sample < numSamples; sample++) {
                    outputTmp[sample] += (*it).eval(manager->getCurrentTime() + dt*sample);
                }
                ++it;
            }
        }
    }

    for (unsigned int sample = 0; sample < numSamples; sample++)
    {
        manager->delayBuffer[manager->delayBufferIndex] = outputTmp[sample];
        manager->delayBufferIndex = (manager->delayBufferIndex + 1) % manager->delayBufferSize;
    }

    for (unsigned int sample = 0; sample < numSamples; sample++)
    {
        output[sample] = outputTmp[sample];

        for (unsigned int delayfac = 1; delayfac < 10; delayfac++) {
            unsigned int delayIndex = (manager->delayBufferIndex - numSamples + sample - delayfac*5000 + manager->delayBufferSize) % manager->delayBufferSize;
            output[sample] += powf(0.5, delayfac)*manager->delayBuffer[delayIndex];
        }
    }

    delete [] outputTmp;
}

void
Manager::noteOnCallback(unsigned char channel, unsigned char note, unsigned char vel, void *userData) {
    Manager *manager = (Manager*) userData;
    Patch patch;
    patch = manager->defaultPatches[channel];
    patch.trigger(note, vel, manager->getCurrentTime());
    manager->activeSounds[channel].push_back(patch);

    std::cout << "NOTEON ch=" << (int)channel << " note=" << (int)note << std::endl;
}

void
Manager::noteOffCallback(unsigned char channel, unsigned char note, void *userData) {
    Manager *manager = (Manager*) userData;

    std::cout << "NOTEOFF ch=" << (int)channel << " note=" << (int)note << std::endl;

    for (std::list<Patch>::iterator it = manager->activeSounds[channel].begin();
         it != manager->activeSounds[channel].end(); it++) {
        if ((*it).note == note)
            (*it).release(manager->getCurrentTime());
    }
}
