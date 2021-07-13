/*
  ==============================================================================
    This file contains the basic framework code for a JUCE plugin editor.
  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ThreeBandsAudioProcessorEditor::ThreeBandsAudioProcessorEditor (ThreeBandsAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    //This is where our plugins editor size is set
    setSize (580, 250);
    
    //SLIDER 1
    // these define the parameters of our slider object (https://docs.juce.com/master/classSlider.html)
    slider1.setSliderStyle (juce::Slider::LinearBar);
    //slider1.setRotaryParameters(4.18879f, 5.23599f, true); //rotary dial parameters
    slider1.setRange (-60.0f, 1.0f, 0.01f); //set like this so it is measured in decibels
    slider1.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 150, 25);
    slider1.setPopupDisplayEnabled (true, false, this);
    slider1.setTextValueSuffix (" Gain");
    slider1.setValue(-20.0f); //default value
    //slider1.setSkewFactor(0.2); //sets the sensitivity to be different across the slider
    
    // this function adds the slider to the editor
    addAndMakeVisible (&slider1); //this gain slider is a child component of the editor
    
    // add the listener to the slider
    slider1.addListener (this);
    
    //LOW FREQ
    lowFreq.setSliderStyle (juce::Slider::RotaryHorizontalDrag);
    lowFreq.setRange (-60.0f, 1.0f, 0.01f);
    lowFreq.setValue (-20.0f);
    lowFreq.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 150, 25);
    addAndMakeVisible (&lowFreq);
    
    //MID FREQ
    midFreq.setSliderStyle (juce::Slider::RotaryHorizontalDrag);
    midFreq.setRange (-60.0f, 1.0f, 0.01f);
    midFreq.setValue (-20.0f);
    midFreq.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 150, 25);
    addAndMakeVisible (&midFreq);
    
    //HIGH FREQ
    highFreq.setSliderStyle (juce::Slider::RotaryHorizontalDrag);
    highFreq.setRange (-60.0f, 1.0f, 0.01f);
    highFreq.setValue (-20.0f);
    highFreq.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 150, 25);
    addAndMakeVisible (&highFreq);
    
}

ThreeBandsAudioProcessorEditor::~ThreeBandsAudioProcessorEditor()
{
}

//==============================================================================
void ThreeBandsAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::rebeccapurple); //makes P U R P L E

    g.setFont (15.0f);
    //g.drawFittedText ("Volume", getLocalBounds(), juce::Justification::centred, 1);
}

void ThreeBandsAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    // sets the position and size of the slider with arguments (x, y, width, height)
    slider1.setBounds (10, 20, 560, 20);
    
    lowFreq.setBounds (10, getHeight() / 2 - 50, 150, 150);
    midFreq.setBounds (210, getHeight() / 2 - 50, 150, 150);
    highFreq.setBounds (410, getHeight() / 2 - 50, 150, 150);

}

void ThreeBandsAudioProcessorEditor::sliderValueChanged (juce::Slider* slider)
{
    
    if (slider == &slider1)
    {
        audioProcessor.slider1Gain = slider1.getValue();
    }
    
//I had to change this to audioProcessor instead of processor, I don't understand why but found advice to do so here (https://forum.juce.com/t/no-member-named-gain-in-juce-audioprocessor-error/40655) and it worked, so.
}
