#ifndef PATCH_H
#define PATCH_H

#include "ADSREnvelope.h"
#include <vector>

class Patch
{
public:
    Patch();
    std::vector<float> timbre;
    ADSREnvelope env;

    float eval  (float t);
    void trigger(unsigned char note, unsigned char vel, float t);
    void release(float t);

    float fmodAmpl, fmodFreq;
    bool fmodEnabled;
    float delayTime, delayCoeff;
    bool delayEnabled;

    unsigned char note, vel;
    float freq, velf;
};

#endif // PATCH_H
