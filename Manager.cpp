#include "Manager.h"
#include "AudioInterface.h"
#include "RtMIDInterface.h"
#include "Patch.h"

#include <iterator>
#include <list>
#include <vector>
#include <iostream>

Manager::Manager() {
    for (unsigned int channel = 0; channel < 16; channel++) {
        std::list<Patch> patchlist;
        Patch patch;

        activeSounds.push_back(patchlist);
        defaultPatches.push_back(patch);
    }

    std::cout << "Initialization of Audio Interface" << std::endl;
    audioInterface = new AudioInterface(44100, 64);
    audioInterface->setGenerateCallback(&Manager::generateCallback, this);
    audioInterface->openStream();

    std::cout << "Initialization of RtMIDI" << std::endl;
    MIDIInterface = new RtMIDIInterface();

    MIDIInterface->openPort(0);
    MIDIInterface->setNoteOnCallback(&noteOnCallback, this);
    MIDIInterface->setNoteOffCallback(&noteOffCallback, this);
}

Manager::~Manager() {
    delete MIDIInterface;
    delete audioInterface;
    MIDIInterface = 0;
    audioInterface = 0;
}

double
Manager::getCurrentTime() {
    return audioInterface->getCurrentTime();
}

void
Manager::generateCallback(double t, double dt, unsigned int numSamples, float *output, void *userData) {
    Manager *manager = (Manager*) userData;

    for (unsigned int sample = 0; sample < numSamples; sample++) {
        output[sample] = 0.0;
    }

    for (unsigned int channel = 0; channel < 16; channel++) {
        for (std::list<Patch>::iterator it = manager->activeSounds[channel].begin();
             it != manager->activeSounds[channel].end();) {

            if ((*it).isFinished()) {
                manager->activeSounds[channel].erase(it++);
            } else {
                for (unsigned int sample = 0; sample < numSamples; sample++) {
                    output[sample] += (*it).eval(manager->getCurrentTime() + dt*sample);
                }
                ++it;
            }
        }
    }
}

void
Manager::noteOnCallback(unsigned char channel, unsigned char note, unsigned char vel, void *userData) {
    Manager *manager = (Manager*) userData;
    Patch patch;
    patch = manager->defaultPatches[channel];
    patch.trigger(note, vel, manager->getCurrentTime());
    manager->activeSounds[channel].push_back(patch);

   // std::cout << manager->activeSounds[channel].size() << std::endl;
    std::cout << "NOTEON " << (int)channel << " " << (int)note << std::endl;
}

void
Manager::noteOffCallback(unsigned char channel, unsigned char note, void *userData) {
    Manager *manager = (Manager*) userData;

    for (std::list<Patch>::iterator it = manager->activeSounds[channel].begin();
         it != manager->activeSounds[channel].end(); it++) {
        if ((*it).note == note)
            (*it).release(manager->getCurrentTime());
    }
}
