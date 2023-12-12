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
    bool appliesToNote(int midiNoteNumber) override { return true; }
    bool appliesToChannel(int midiChannel) override { return true; }
};
