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


#include "Patch.h"
#include <math.h>
#include <iostream>
#include <assert.h>

Patch::Patch() {
    timbreAmplitudes = {1.0, 0.6, 0.0, 0.9};
    waveType = {WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN};

    fmodAmpl = 0.0;
    fmodFreq = 0.0;
    fmodEnabled = false;

    frequency = 0.0;
    amplitude = 0.0;
}

void
Patch::trigger(unsigned char _note, unsigned char _vel, double t) {
    envelope.trigger(t);
    note = _note;
    vel = _vel;

    // Compute the frequency and amplitude of the signal from the integer-valued
    // MIDI data:
    frequency = 8.175 * powf(2, (((float)note)/12));
    amplitude = 1.0*((float)vel)/127.0;
}

void
Patch::release(double t) {
    envelope.release(t);
}

bool
Patch::isFinished() {
    return envelope.isFinished();
}

float
Patch::eval(double t) {
    float envval = envelope.eval(t);      // Evaluate the envelope value.
    float out = 0;                        // Output before multiplication with the envelope.
    float outWithEnv = 0;                 // Output after multiplication with the envelope.
    double dt = t - envelope.triggerTime; // Time after trigger.

    assert(waveType.size() == timbreAmplitudes.size());

    for (unsigned int indTimbre= 0; indTimbre < timbreAmplitudes.size(); indTimbre++) {
        float wave;            // Synthesized waveform.
        float timeNormalized;  // Normalized time used to determine the phase.
        float amplitudeFinal;  // Amplitude after multiplication with velocity.
        double frequencyFinal; // Frequency after possible frequency modulation.

        // Ratio of the frequency to the patch frequency.
        double fcoeff = 1.0 + (double) indTimbre;

        if (timbreCoeff.size() > indTimbre)
        {
            fcoeff = timbreCoeff[indTimbre];
        }
        // Try to avoid aliasing by synthesizing only signals under 21kHz. Note that
        // this will only work for purely sinusoidal waveforms.
        if (fcoeff * frequency > 21000.0) continue;

        // Apply frequency modulation, if enabled.
        if (fmodEnabled) {
            frequencyFinal = frequency * (1.0 + envval * amplitude *  fmodAmpl
                                       * (float)sin(dt * frequency * fmodFreq * 2.0 * M_PI));
            amplitudeFinal = amplitude * 1.0;
        } else {
            frequencyFinal = frequency;
            amplitudeFinal = 0.5;
        }
        // Normalize the time of the patch to be peridic with a period of 1s.
        timeNormalized = fcoeff * frequencyFinal * dt;

        switch(waveType[indTimbre])
        {
        case WAVE_SIN:
            wave = (float)sin(2.0 * M_PI * timeNormalized);
            break;
        case WAVE_SQU:
            if (fmodf(timeNormalized, 1.0) < 0.5) {
                wave = 1.0;
            } else {
                wave = -1.0;
            }
            break;
        case WAVE_TRI:
            wave = 2.0 * fabs(2.0 * fmodf(timeNormalized, 1) - 1.0) - 1.0;
            break;
        case WAVE_SAW:
            wave = 2.0 * fmodf(timeNormalized, 1) - 1.0;
            break;
        }
        out += amplitudeFinal * timbreAmplitudes[indTimbre] * wave;
    }
    outWithEnv = out * envval;
    return outWithEnv;
}
