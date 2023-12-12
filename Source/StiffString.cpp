/*
  ==============================================================================

    StiffString.cpp
    Created: 11 Dec 2023 7:55:35pm
    Author:  Clancy Rowley

  ==============================================================================
*/

#include "StiffString.h"

StiffString::StiffString(LEAF *const leaf, int numModes) :
    leaf(leaf),
    numModes(numModes)
{
    amplitudes = new float[numModes];
    outputWeights = new float[numModes];
    osc = new tCycle[numModes];
    for (int i = 0; i < numModes; ++i) {
        tCycle_init(&osc[i], leaf);
    }
    updateOutputWeights();
}

StiffString::~StiffString()
{
    delete[] osc;
    delete[] amplitudes;
    delete[] outputWeights;
}

void StiffString::setFreq(float newFreqHz)
{
    freqHz = newFreqHz;
    float kappa_sq = stiffness * stiffness;
    for (int i = 0; i < numModes; ++i) {
        int n = i + 1;
        int n_sq = n * n;
        float sig = decay + decayHighFreq * n_sq;
        float w0 = n * sqrtf(1.0f + kappa_sq * n_sq);
        float zeta = sig / w0;
        float w = w0 * sqrtf(1.0f - zeta * zeta);
        tCycle_setFreq(&osc[i], freqHz * w);
    }
}

float StiffString::getNextSample()
{
    float sample = 0.0f;
    for (int i = 0; i < numModes; ++i) {
        sample += tCycle_tick(&osc[i]) * amplitudes[i] * outputWeights[i];
        int n = i + 1;
        float sig = decay + decayHighFreq * n * n;
        amplitudes[i] *= exp(-sig * freqHz * leaf->twoPiTimesInvSampleRate);
    }
    return sample;
}

void StiffString::setPickupPos(float newValue)
{
    pickupPos = newValue;
    updateOutputWeights();
}

void StiffString::updateOutputWeights()
{
    float x0 = pickupPos * 0.5 * PI;
    for (int i = 0; i < numModes; ++i) {
        outputWeights[i] = sin((i + 1) * x0);
    }
}

void StiffString::setInitialAmplitudes()
{
    float x0 = pluckPos * 0.5 * PI;
    for (int i = 0; i < numModes; ++i) {
        int n = i + 1;
        float denom = n * n * x0 * (PI - x0);
        jassert(denom != 0);
        amplitudes[i] = 2.0f * sin(x0 * n) / denom;
    }
}
