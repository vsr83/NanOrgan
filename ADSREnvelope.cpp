#include "ADSREnvelope.h"

ADSREnvelope::ADSREnvelope(float _attackTime,
                           float _decayTime,
                           float _releaseTime,
                           float _peakAmplitude,
                           float _sustainAmplitude) {
    attackTime = _attackTime;
    decayTime = _decayTime;
    releaseTime = _releaseTime;
    peakAmplitude = _peakAmplitude;
    sustainAmplitude = _sustainAmplitude;

    triggerTime = 0.0;
    releaseTime = 0.0;
    amplitudeAtRelease = 0.0;
}

void
ADSREnvelope::trigger(float time) {
    state = STATE_ATTACK;
    triggerTime = time;
}

void
ADSREnvelope::release(float time) {
    amplitudeAtRelease = eval(time);
    state = STATE_RELEASE;
}

float
ADSREnvelope::eval(float t) {
    switch (state) {
    case STATE_OFF:
        return 0.0;
        break;
    case STATE_ATTACK:
    {
        float stateTime = t - triggerTime;
        if (stateTime <= attackTime) {
            return peakAmplitude * stateTime / attackTime;
        } else {
            state = STATE_DECAY;
            return eval(t);
        }
        break;
    }
    case STATE_DECAY:
    {
        float stateTime = t - triggerTime - attackTime;
        if (stateTime < decayTime)
        {
            return peakAmplitude - (peakAmplitude - sustainAmplitude) * stateTime/decayTime;
        } else {
            return sustainAmplitude;
        }
    }
        break;
    case STATE_RELEASE:
    {
        float stateTime = t - releasedTime;
        if (stateTime < releaseTime) {
            return sustainAmplitude * (1 - stateTime/releaseTime);
        } else {
            state = STATE_OFF;
            return 0.0;
        }
        break;
    }
    }
}

bool
ADSREnvelope::isFinished() {
    return state == STATE_OFF;
}
