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
    timbre = {1.0, 0.6, 0.0, 0.9};
    waveType = {WAVE_SIN, WAVE_SIN, WAVE_SIN, WAVE_SIN};

    fmodAmpl = 0.0;
    fmodFreq = 0.0;
    fmodEnabled = false;
}

void
Patch::trigger(unsigned char _note, unsigned char _vel, double t) {
    env.trigger(t);
    note = _note;
    vel = _vel;

    freq = 8.175 * powf(2, (((float)note)/12));
    velf = 2.0*((float)vel)/256.0;
    std::cout << velf << std::endl;
}

void
Patch::release(double t) {
    env.release(t);
}

bool
Patch::isFinished() {
    return env.isFinished();
}

float
Patch::eval(double t) {
    float envval = env.eval(t);
    float out = 0, outenv = 0, ampl;
    double fcoeff;

    assert(waveType.size() == timbre.size());

    for (unsigned int indTimbre= 0; indTimbre < timbre.size(); indTimbre++) {
        fcoeff = 1.0 + (double) indTimbre;

        float wave, angle;
        double modFreq;
        double dt = t - env.triggerTime;

        if (fcoeff * freq > 21000.0) continue;

        // Apply frequency modulation, if enabled.
        if (fmodEnabled) {
            modFreq = freq * (1.0 + envval * velf * velf * fmodAmpl * (float)sin(dt * freq * fmodFreq * 2.0 * M_PI));
            ampl = velf * 5.0;
        } else {
            modFreq = freq;
            ampl = 1.0;
        }
        angle = fcoeff * modFreq * dt;

        switch(waveType[indTimbre])
        {
        case WAVE_SIN:
            wave = (float)sin(2.0*M_PI*angle);
            break;
        case WAVE_SQU:
            if (fmodf(angle, 1.0) < 0.5) {
                wave = 1.0;
            } else {
                wave = -1.0;
            }
            break;
        case WAVE_TRI:
            wave = 2.0*fabs(2.0*fmodf(angle, 1) - 1.0) - 1.0;
            break;
        case WAVE_SAW:
            wave = 2.0*fmodf(angle, 1) - 1.0;
            break;
        }
        out += ampl * timbre[indTimbre]*wave;
        //std::cout << indTimbre << " " << fcoeff << std::endl;

    }
    outenv = out * envval;
    //std::cout << t << " " << env.getState() << " " << envval << std::endl;
    //std::cout <<t << " " << freq << " " << fcoeff << " " <<out << " " << envval << " " << outenv<< std::endl;
    return outenv;
}
