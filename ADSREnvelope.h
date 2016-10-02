#ifndef ADSRENVELOPE_H
#define ADSRENVELOPE_H

class ADSREnvelope
{
public:
    ADSREnvelope(float _attackTime,
                 float _decayTime,
                 float _releaseTime,
                 float _peakAmplitude,
                 float _sustainAmplitude);

    enum {STATE_OFF, STATE_ATTACK, STATE_DECAY, STATE_RELEASE};

    void trigger(float t);
    void release(float t);
    float eval(float t);
    bool isFinished();

    float attackTime, decayTime, releaseTime;
    float peakAmplitude, sustainAmplitude;

    float triggerTime, releasedTime;
private:
    unsigned int state;
    float amplitudeAtRelease;
};

#endif // ADSRENVELOPE_H
