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

    // ADSR Envelopes exist in on of four states. When a new sound is initiated,
    // the ADSR envelope of the patch is set to Attack state. Once the attackTime
    // period of the sound has been generated, the sound is set to decay state.
    // When the sound is released the sound is set to release state and after
    // releaseTime to OFF.
    enum {STATE_OFF, STATE_ATTACK, STATE_DECAY, STATE_RELEASE};

    void trigger(double t);  // Trigger a ADSR Envelope to ATTACK state.
    void release(double t);  // Release a ADSR Envelope.
    double eval(double t);   // Evaluate the envelope at time t.
    bool isFinished();       // Is the envelope in OFF state.
    unsigned int getState(); // Get the current state.

    double attackTime, decayTime, releaseTime;
    // Amplitudes after attack and decay stages, repsectively.
    double peakAmplitude, sustainAmplitude;

    // The time at trigger and release of the envelope.
    double triggerTime, releasedTime;
private:
    // Current state of the envelope.
    unsigned int state;

    // If the release is triggered before the attack and decay stages are
    // completed, the release is started from the amplitude at then release
    // event.
    double amplitudeAtRelease;
};

#endif // ADSRENVELOPE_H
