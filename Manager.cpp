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
            patch.timbre.clear();
            patch.timbre.push_back(1.0);
            patch.timbre.push_back(0.0);
            patch.timbre.push_back(0.0);
            patch.timbre.push_back(0.6);
            patch.timbre.push_back(0.0);
            patch.timbre.push_back(0.0);
        }
        if (channel == 2) {
            patch.timbre.clear();
            patch.timbre.push_back(1.0);
            patch.timbre.push_back(0.6);
            patch.timbre.push_back(0.0);
            patch.timbre.push_back(0.6);
            patch.timbre.push_back(0.0);
            patch.timbre.push_back(0.0);
        }
        if (channel == 3) {
            patch.timbre.clear();
            patch.timbre.push_back(1.0); // 1
            patch.timbre.push_back(0.5); // 2
            patch.timbre.push_back(0.0); // 3
            patch.timbre.push_back(0.6); // 4
            patch.timbre.push_back(0.0); // 5
            patch.timbre.push_back(0.0); // 6
            patch.timbre.push_back(0.0); // 7
            patch.timbre.push_back(1.0); // 8
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

        for (unsigned int delayfac = 1; delayfac < 12; delayfac++) {
            unsigned int delayIndex = (manager->delayBufferIndex - numSamples + sample - delayfac*4000 + manager->delayBufferSize) % manager->delayBufferSize;
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
