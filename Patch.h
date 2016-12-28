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

#ifndef PATCH_H
#define PATCH_H

#include "ADSREnvelope.h"
#include <vector>

// This class implements the linear synthesis of an audio signal.
// Sinusoidal, triangular, saw and square waves are supported.

enum {
    WAVE_SIN,
    WAVE_TRI,
    WAVE_SAW,
    WAVE_SQU
};

// A Patch object is associated to each active sound and a MIDI channel.

class Patch
{
public:
    Patch();
    // The waveform is summed from a finite number of basic waveforms.
    // The length of timbreAmplitudes determines the number of summed
    // waveforms. The amplitudes, harmonic numbers and waveforms of the
    // summed terms are respectively determined in the following three
    // vectors.
    std::vector<float> timbreAmplitudes;
    std::vector<float> timbreCoeff;
    std::vector<unsigned int> waveType;    

    // The basic waveform is multiplied with an ADSR envelope.
    ADSREnvelope envelope;

    float eval  (double t);

    void trigger(unsigned char note, unsigned char vel, double t);
    void release(double t);
    bool isFinished();

    // Parameters for frequency modulation.
    float fmodAmpl;   // Amplitude of the sinusoidal signal used in FM.
    float fmodFreq;   // Frequency ratio of the sinusoidal w.r.t. patch frequency.
    bool fmodEnabled;

    // Note and velocity in the MIDI Note On message.
    unsigned char note;
    unsigned char vel;

    // The MIDI data is used to compute the frequency and basic amplitude
    // of the waveform.
    double frequency;
    double amplitude;
};

#endif // PATCH_H
