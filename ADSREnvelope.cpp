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

#include "ADSREnvelope.h"
#include <iostream>
#include <math.h>

ADSREnvelope::ADSREnvelope() {
    attackTime       = 0.03;
    decayTime        = 0.100;
    releaseTime      = 0.1;
    peakAmplitude    = 1.0;
    sustainAmplitude = 0.5;

    triggerTime = 0.0;
    releasedTime = 0.0;
    amplitudeAtRelease = 0.0;

    state = STATE_OFF;
}

ADSREnvelope::ADSREnvelope(double _attackTime,
                           double _decayTime,
                           double _releaseTime,
                           double _peakAmplitude,
                           double _sustainAmplitude) {
    attackTime = _attackTime;
    decayTime = _decayTime;
    releaseTime = _releaseTime;
    peakAmplitude = _peakAmplitude;
    sustainAmplitude = _sustainAmplitude;

    triggerTime = 0.0;
    releaseTime = 0.0;
    amplitudeAtRelease = 0.0;

    state = STATE_OFF;
}

void
ADSREnvelope::trigger(double time) {
    state = STATE_ATTACK;
    triggerTime = time;
}

void
ADSREnvelope::release(double time) {
    amplitudeAtRelease = eval(time);
    releasedTime = time;
    state = STATE_RELEASE;

}

unsigned int
ADSREnvelope::getState() {
    return state;
}

double
ADSREnvelope::eval(double t) {
    // We use linear attack and exponential decay.

    switch (state) {
    case STATE_OFF:
        return 0.0;
        break;
    case STATE_ATTACK:
    {
        double stateTime = t - triggerTime;
        if (stateTime <= attackTime) {
            //return peakAmplitude * exp((stateTime - attackTime) * 2.0 / attackTime);
            return peakAmplitude * stateTime / attackTime;
        } else {
            state = STATE_DECAY;
            return eval(t);
        }
        break;
    }
    case STATE_DECAY:
    {
        double stateTime = t - triggerTime - attackTime;
        if (stateTime < decayTime)
        {
            //return peakAmplitude - (peakAmplitude - sustainAmplitude) * stateTime/decayTime;
            return sustainAmplitude + (peakAmplitude - sustainAmplitude)
                      * exp(-stateTime * 4.0 / decayTime);
        } else {
            return sustainAmplitude;
        }
    }
        break;
    case STATE_RELEASE:
    {
        double stateTime = t - releasedTime;

        if (stateTime < releaseTime) {
            return amplitudeAtRelease * (1 - stateTime/releaseTime);
        } else {
            state = STATE_OFF;
            return 0.0;
        }
        break;
    }
    }
    std::cout << "ERROR" << std::endl;
}

bool
ADSREnvelope::isFinished() {
    return state == STATE_OFF;
}
