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

class Patch
{
public:
    Patch();
    // The Fourier coefficients are assembled into the following vector.
    std::vector<float> timbre;
    std::vector<unsigned int> waveType;
    ADSREnvelope env;

    float eval  (double t);
    void trigger(unsigned char note, unsigned char vel, double t);
    void release(double t);
    bool isFinished();

    float fmodAmpl, fmodFreq;
    bool fmodEnabled;

    unsigned char note, vel;
    double freq, velf;
};

#endif // PATCH_H
