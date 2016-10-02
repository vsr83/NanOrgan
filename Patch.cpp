#include "Patch.h"
#include <math.h>

Patch::Patch() {
    timbre.push_back(1.0);
    fmodAmpl = 0.0;
    fmodFreq = 0.0;
    fmodEnabled = false;

    delayTime = 0.2;
    delayCoeff = 0.2;
    delayEnabled = false;
}

void
Patch::trigger(unsigned char _note, unsigned char _vel, float t) {
    env.trigger(t);
    float freq = 8.175 * 0.5 * powf(2, (((float)note)/12));
    velf = 0.5*((float)vel)/256.0;
}

void
Patch::release(float t) {
    env.release(t);
}

float
Patch::eval(float t) {
    float envval = env.eval(t);
    float out = 0;

    for (unsigned int indTimbre; indTimbre < timbre.size(); indTimbre++) {
        float fcoeff = 1.0 + (float) indTimbre;
        out += 0.25*(float)sin(2*M_PI*fcoeff*freq*t);
    }
    out *= envval;
    return out;
}
