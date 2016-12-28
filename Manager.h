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

#ifdef FREEVERB
#include "freeverb/revmodel.hpp"
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

    // The audio interface calls the method new audio is to be generated.
    static void generateCallback(double t,
                                 double dt,
                                 unsigned int numSamples,
                                 float *outputLeft,
                                 float *outputRight,
                                 void *userData);

    // The manager only handles three types of MIDI messages: Note On, Note Off
    // and Channel Mode messages. Callbacks are called by RtMIDIInterface.
    // An instance of the Manager is passed as the parameter in userData.
    static void noteOnCallback(unsigned char channel,
                               unsigned char note,
                               unsigned char vel,
                               void *userData);
    static void noteOffCallback(unsigned char channel,
                                unsigned char note,
                                void *userData);
    static void channelModeCallback(unsigned char byte1,
                                    unsigned char byte2,
                                    unsigned char bank,
                                    void *userData);

    // List of patches associated to MIDI channels.
    std::vector<Patch> activePatches;
    // List of active patches of each channel.
    std::vector<std::list<Patch> > activeSounds;
    // The channel of the most recent Note On/Off activity is used to associate
    // channel mode messages to channels.
    unsigned char mostRecentChannel;

    bool pedalTimbres[16][16];
    bool sustainStatus;

    // Variables associated to the Delay effect.
    float *delayBuffer;
    unsigned int delayBufferSize;
    unsigned int delayBufferIndex;
    bool delayEnabled;

#ifdef USE_PTHREAD
    pthread_mutex_t mutex;
#endif
#ifdef FREEVERB
    revmodel rm;
    revmodel rm2;
    bool reverbEnabled;
#endif
};

#endif // MANAGER_H
