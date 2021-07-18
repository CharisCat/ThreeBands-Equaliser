/*
  ==============================================================================
    This file contains the basic framework code for a JUCE plugin processor.
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class ThreeBandsAudioProcessor  : public juce::AudioProcessor
{
public:
    
    //==============================================================================
    ThreeBandsAudioProcessor();
    ~ThreeBandsAudioProcessor() override;

    //==============================================================================
    
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::IIRFilter lowShelfLeft;
    juce::IIRFilter lowShelfRight;
    
    juce::IIRFilter midPeakingLeft;
    juce::IIRFilter midPeakingRight;
    
    juce::IIRFilter highShelfLeft;
    juce::IIRFilter highShelfRight;
    
    float slider1Gain { 0.5f }; //This is the value that is set by slider1 in the GUI
    float lowFreqGain { 0.5f };
    float midFreqGain { 0.5f };
    float highFreqGain { 0.5f };
    
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ThreeBandsAudioProcessor)
};
