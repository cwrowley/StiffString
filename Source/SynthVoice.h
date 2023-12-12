/*
  ==============================================================================

    SynthVoice.h
    Created: 11 Dec 2023 10:48:56am
    Author:  Clancy Rowley

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "StiffString.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
    SynthVoice(LEAF *const leaf, int numModes);
    bool canPlaySound (juce::SynthesiserSound* sound) override;
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void controllerMoved (int controllerNumber, int newControllerValue) override;
    void pitchWheelMoved (int newPitchWheelValue) override;
    void prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels);
    void renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples) override;

    void setStiffness(float stiffness) { stiffString.setStiffness(stiffness); }
    void setPluckPos(float pluckPos) { stiffString.setPluckPos(pluckPos); }
    void setPickupPos(float pickupPos) { stiffString.setPickupPos(pickupPos); }
    void setDecay(float decay) { stiffString.setDecay(decay); }
    void setDecayHighFreq(float decayHF) { stiffString.setDecayHighFreq(decayHF); }

private:
    LEAF *const leaf;
    const int numModes;
    juce::AudioBuffer<float> voiceBuffer;
    bool prepared = false;
    bool playing = false;

    float masterAmplitude = 0.0f;
    StiffString stiffString;
};
