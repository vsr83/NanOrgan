#ifndef PATCH_H
#define PATCH_H

#include "ADSREnvelope.h"
#include <vector>

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
