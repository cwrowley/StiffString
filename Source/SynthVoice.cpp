/*
  ==============================================================================

    SynthVoice.cpp
    Created: 11 Dec 2023 10:48:56am
    Author:  Clancy Rowley

  ==============================================================================
*/

#include "SynthVoice.h"
#include "SynthSound.h"

SynthVoice::SynthVoice(LEAF *const leaf, int numModes) :
    leaf(leaf),
    numModes(numModes),
    stiffString(leaf, numModes)
{}

bool SynthVoice::canPlaySound (juce::SynthesiserSound* sound)
{
    return dynamic_cast<SynthSound*>(sound) != nullptr;
}

void SynthVoice::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition)
{
    if (!playing) {
        stiffString.setInitialAmplitudes();
        auto cyclesPerSecond = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        stiffString.setFreq(cyclesPerSecond);
    }
    playing = true;
    masterAmplitude = 0.7f;
}

void SynthVoice::stopNote (float velocity, bool allowTailOff)
{
    if (playing) {
        masterAmplitude = 0.0f;
        playing = false;
    }
}

void SynthVoice::controllerMoved (int controllerNumber, int newControllerValue) {}

void SynthVoice::pitchWheelMoved (int newPitchWheelValue) {}

void SynthVoice::prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels)
{
    // WORK HERE

    prepared = true;
}

void SynthVoice::renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples)
{
    jassert (prepared);

    if (! isVoiceActive()) return;

    voiceBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    voiceBuffer.clear();

    for (int s = 0; s < numSamples; ++s) {
        float val = masterAmplitude * stiffString.getNextSample();
        for (int ch = 0; ch < outputBuffer.getNumChannels(); ++ch) {
            voiceBuffer.addSample(ch, s, val);
        }
    }

    for (int ch = 0; ch < outputBuffer.getNumChannels(); ++ch) {
        outputBuffer.addFrom(ch, startSample, voiceBuffer, ch, 0, numSamples);
    }
}
