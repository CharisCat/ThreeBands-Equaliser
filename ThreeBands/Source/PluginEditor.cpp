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
    setSize (200, 300);
    
    // these define the parameters of our slider object (https://docs.juce.com/master/classSlider.html)
    slider1.setSliderStyle (juce::Slider::LinearBarVertical);
    slider1.setRange (0.0f, 1.0f, 0.01f);
    slider1.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 90, 10);
    slider1.setPopupDisplayEnabled (true, false, this);
    slider1.setTextValueSuffix (" Gain");
    slider1.setValue(1.0f); //default value
    //slider1.setSkewFactor(0.2); //sets the sensitivity to be different across the slider
    
    // this function adds the slider to the editor
    addAndMakeVisible (&slider1); //this gain slider is a child component of the editor
    
    // add the listener to the slider
    slider1.addListener (this);
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
    g.drawFittedText ("Volume", getLocalBounds(), juce::Justification::centred, 1);
}

void ThreeBandsAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    // sets the position and size of the slider with arguments (x, y, width, height)
    slider1.setBounds (40, 30, 20, getHeight() - 60);
}

void ThreeBandsAudioProcessorEditor::sliderValueChanged (juce::Slider* slider)
{
    
    if (slider == &slider1)
    {
        audioProcessor.slider1Gain = slider1.getValue();
    }
    
//I had to change this to audioProcessor instead of processor, I don't understand why but found advice to do so here (https://forum.juce.com/t/no-member-named-gain-in-juce-audioprocessor-error/40655) and it worked, so.
}
