#ifndef ADSRENVELOPE_H
#define ADSRENVELOPE_H

class ADSREnvelope
{
public:
    ADSREnvelope();
    ADSREnvelope(double _attackTime,
                 double _decayTime,
                 double _releaseTime,
                 double _peakAmplitude,
                 double _sustainAmplitude);

    enum {STATE_OFF, STATE_ATTACK, STATE_DECAY, STATE_RELEASE};

    void trigger(double t);
    void release(double t);
    double eval(double t);
    bool isFinished();
    unsigned int getState();

    double attackTime, decayTime, releaseTime;
    double peakAmplitude, sustainAmplitude;

    double triggerTime, releasedTime;
private:
    unsigned int state;
    double amplitudeAtRelease;
};

#endif // ADSRENVELOPE_H
