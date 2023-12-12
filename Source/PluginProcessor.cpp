/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SynthSound.h"
#include "SynthVoice.h"

//==============================================================================
StiffStringAudioProcessor::StiffStringAudioProcessor() :
    AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    params(*this, nullptr, "Parameters", createParams())
{
    LEAF_init(&leaf, 44100, leafMemory, leafMemSize, []() { return (float) rand() / RAND_MAX; });

    synth.addSound(new SynthSound());

    for (int i=0; i < numVoices; ++i) {
        synth.addVoice(new SynthVoice(&leaf));
    }
}

StiffStringAudioProcessor::~StiffStringAudioProcessor()
{
}

//==============================================================================

double StiffStringAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

void StiffStringAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    LEAF_setSampleRate(&leaf, sampleRate);
    synth.setCurrentPlaybackSampleRate(sampleRate);
    for (int i = 0; i < synth.getNumVoices(); ++i) {
        if (auto voice = static_cast<SynthVoice *>(synth.getVoice(i))) {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }
}

void StiffStringAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool StiffStringAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void StiffStringAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    // auto totalNumOutputChannels = getTotalNumOutputChannels();

    buffer.clear();
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool StiffStringAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* StiffStringAudioProcessor::createEditor()
{
    //return new StiffStringAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void StiffStringAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void StiffStringAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new StiffStringAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout StiffStringAudioProcessor::createParams()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "PLUCKPOS", 1}, "Pluck pos", juce::NormalisableRange<float> { 0.01f, 0.99f, 0.01f }, 0.2f, ""));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "PICKUPPOS", 1}, "Pickup pos", juce::NormalisableRange<float> { 0.01f, 0.99f, 0.01f }, 0.1f, ""));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "STIFFNESS", 1}, "Stiffness", juce::NormalisableRange<float> { 0.0f, 3.0f, 0.001f }, 0.01f, ""));

    return { params.begin(), params.end() };
}
