/*
  ==============================================================================
    This file contains the basic framework code for a JUCE plugin editor.
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class ThreeBandsAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                        public juce::Slider::Listener
{
public:
    ThreeBandsAudioProcessorEditor (ThreeBandsAudioProcessor&);
    ~ThreeBandsAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void sliderValueChanged (juce::Slider *slider) override;

private:

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ThreeBandsAudioProcessor& audioProcessor;
    
    juce::Slider slider1;
    
    juce::Slider lowFreq;
    
    juce::Slider midFreq;
    
    juce::Slider highFreq;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ThreeBandsAudioProcessorEditor)
};
