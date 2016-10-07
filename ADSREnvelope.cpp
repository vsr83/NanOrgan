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
