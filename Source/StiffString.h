/*
  ==============================================================================

    StiffString.h
    Created: 11 Dec 2023 7:55:35pm
    Author:  Clancy Rowley

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class StiffString {
public:
    StiffString(LEAF *const leaf, int numModes);
    ~StiffString();

    void setFreq(float newFreqHz);
    void setInitialAmplitudes();
    float getNextSample();

    // change parameters
    void setStiffness(float newValue) { stiffness = newValue; }
    void setPickupPos(float newValue);
    void setPluckPos(float newValue) { pluckPos = newValue; }
    void setDecay(float newValue) { decay = newValue; }
    void setDecayHighFreq(float newValue) { decayHighFreq = newValue; }

private:
    void updateOutputWeights();

    LEAF *const leaf;
    const int numModes;

    tCycle *osc;
    float *amplitudes;
    float *outputWeights;
    float freqHz;

    // parameters
    float stiffness = 0.0f;
    float pluckPos = 0.2f;
    float pickupPos = 0.3f;
    float decay = 0.0f;
    float decayHighFreq = 0.0f;
};
