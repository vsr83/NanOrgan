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

#ifndef RTMIDIINTERFACE_H
#define RTMIDIINTERFACE_H

#include <RtMidi.h>

typedef void (*noteOnCallback)(unsigned char channel,
                               unsigned char note,
                               unsigned char vel,
                               void *userData);
typedef void (*noteOffCallback)(unsigned char channel,
                                unsigned char note,
                                void *userData);

typedef void (*channelModeCallback)(unsigned char byte1,
                                    unsigned char byte2,
                                    unsigned char bank,
                                    void *userData);


class RtMIDIInterface
{
public:
    RtMIDIInterface();
    ~RtMIDIInterface();

    void setNoteOnCallback(noteOnCallback callback,
                           void *userData);
    void setNoteOffCallback(noteOffCallback callback,
                            void *userData);
    void setChannelModeCallback(channelModeCallback callback,
                                void *userData);

    void openPort(unsigned int port);
private:
    RtMidiIn *midiIn;
    unsigned int nPorts;

    static void RtCallback(double deltaTime,
                           std::vector<unsigned char> *message,
                           void *userData);
    noteOnCallback  _noteOnCallback;
    noteOffCallback _noteOffCallback;
    channelModeCallback _channelModeCallback;

    bool noteOnCallbackEnabled;
    bool noteOffCallbackEnabled;
    bool channelModeCallbackEnabled;

    void *noteOnUserData;
    void *noteOffUserData;
    void *channelModeUserData;
};

#endif // RTMIDIINTERFACE_H
