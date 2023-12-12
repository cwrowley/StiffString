/*
  ==============================================================================

    SynthSound.h
    Created: 11 Dec 2023 10:48:34am
    Author:  Clancy Rowley

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SynthSound : public juce::SynthesiserSound
{
public:
    const static int numModes = 50;

    bool appliesToNote(int midiNoteNumber) override { return true; }
    bool appliesToChannel(int midiChannel) override { return true; }

    void setInitialAmplitudes(float amplitudes[numModes]);
    void updatePickupPos();

    float outputWeights[numModes] = { 0.0f };
    float pluckPos = 0.2f;
    float pickupPos = 0.3f;
    float decay = 0.001f;
    float decayHighFreq = 0.001f;
private:
};
