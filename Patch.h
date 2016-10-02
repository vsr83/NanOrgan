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

    float eval  (double t);
    void trigger(unsigned char note, unsigned char vel, double t);
    void release(double t);
    bool isFinished();

    float fmodAmpl, fmodFreq;
    bool fmodEnabled;
    double delayTime, delayCoeff;
    bool delayEnabled;

    unsigned char note, vel;
    double freq, velf;
};

#endif // PATCH_H
