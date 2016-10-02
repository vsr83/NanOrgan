#include "Patch.h"
#include <math.h>
#include <iostream>

Patch::Patch() {
    timbre.push_back(1.0);
    timbre.push_back(0.6);
    timbre.push_back(0.0);
    timbre.push_back(0.9);
    fmodAmpl = 0.0;
    fmodFreq = 0.0;
    fmodEnabled = false;

    delayTime = 0.2;
    delayCoeff = 0.2;
    delayEnabled = false;
}

void
Patch::trigger(unsigned char _note, unsigned char _vel, double t) {
    env.trigger(t);
    note = _note;
    vel = _vel;
    freq = 8.175 * powf(2, (((float)note)/12));
    velf = 0.5*((float)vel)/256.0;
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
    float out = 0, outenv = 0;
    double fcoeff;

    for (unsigned int indTimbre= 0; indTimbre < timbre.size(); indTimbre++) {
        fcoeff = 1.0 + (double) indTimbre;
        out += 0.100*timbre[indTimbre]*(float)sin(2.0*M_PI*fcoeff*freq*t+(double)indTimbre);
        //std::cout << indTimbre << " " << fcoeff << std::endl;

    }
    outenv = out * envval;
    //std::cout << t << " " << env.getState() << " " << envval << std::endl;
    //std::cout <<t << " " << freq << " " << fcoeff << " " <<out << " " << envval << " " << outenv<< std::endl;
    return outenv;
}
