/*
  ==============================================================================
    This file contains the basic framework code for a JUCE plugin processor.
  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ThreeBandsAudioProcessor::ThreeBandsAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

ThreeBandsAudioProcessor::~ThreeBandsAudioProcessor()
{
}

//==============================================================================
const juce::String ThreeBandsAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ThreeBandsAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ThreeBandsAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ThreeBandsAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ThreeBandsAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ThreeBandsAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ThreeBandsAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ThreeBandsAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ThreeBandsAudioProcessor::getProgramName (int index)
{
    return {};
}

void ThreeBandsAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ThreeBandsAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    /* lowShelf.setCoefficients(juce::IIRCoefficients::makeLowPass(sampleRate, 150.0f));
    midPeaking.setCoefficients(juce::IIRCoefficients::makePeakFilter(sampleRate, 1000.0f, 0.4f, juce::Decibels::decibelsToGain(midFreqGain)));
    highShelf.setCoefficients(juce::IIRCoefficients::makeHighShelf(sampleRate, 5000.0f, 2.0f, juce::Decibels::decibelsToGain(highFreqGain))); */
}

void ThreeBandsAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ThreeBandsAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void ThreeBandsAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    double sampleRate = getSampleRate();

    //FILTERS
    
    //LOW SHELF
    lowShelfLeft.setCoefficients(juce::IIRCoefficients::makeLowShelf(sampleRate, 200.0f,  2.0f, juce::Decibels::decibelsToGain(lowFreqGain))); //Cut off of 200Hz to hit referral assessment guidelines
    lowShelfRight.setCoefficients(juce::IIRCoefficients::makeLowShelf(sampleRate, 200.0f, 2.0f, juce::Decibels::decibelsToGain(lowFreqGain)));
    
    //MID PEAK
    midPeakingLeft.setCoefficients(juce::IIRCoefficients::makePeakFilter(sampleRate, 1000.0f, 0.4f, juce::Decibels::decibelsToGain(midFreqGain))); //Q of 0.4 and center frequency of 1000Hz to hit refferal assessment guidelines
    midPeakingRight.setCoefficients(juce::IIRCoefficients::makePeakFilter(sampleRate, 1000.0f, 0.4f, juce::Decibels::decibelsToGain(midFreqGain)));
    
    //HIGH SHELF
    highShelfLeft.setCoefficients(juce::IIRCoefficients::makeHighShelf(sampleRate, 5000.0f, 2.0f, juce::Decibels::decibelsToGain(highFreqGain))); //Cut off of 5KHz to hit referral assessment guidelines
    highShelfRight.setCoefficients(juce::IIRCoefficients::makeHighShelf(sampleRate, 5000.0f, 2.0f, juce::Decibels::decibelsToGain(highFreqGain))); //Cut off of 5KHz to hit referral assessment guidelines
    //highShelf.setCoefficients(juce::IIRCoefficients::makeHighShelf(<#double sampleRate#>, <#double cutOffFrequency#>, <#double Q#>, <#float gainFactor#>));

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample) //goes through all samples in buffer
        {
            channelData [sample] = channelData[sample] * juce::Decibels::decibelsToGain(slider1Gain); //multiplies samples by the slider value to attenuate volume, measured in decibels
        }
    }
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    //LOW SHELF
    lowShelfLeft.processSamples(buffer.getWritePointer(0), buffer.getNumSamples());
    lowShelfRight.processSamples(buffer.getWritePointer(1), buffer.getNumSamples());
    
    //MID PEAK
    midPeakingLeft.processSamples(buffer.getWritePointer(0), buffer.getNumSamples());
    midPeakingRight.processSamples(buffer.getWritePointer(1), buffer.getNumSamples());
    
    //HIGH SHELF
    highShelfLeft.processSamples(buffer.getWritePointer(0), buffer.getNumSamples());
    highShelfRight.processSamples(buffer.getWritePointer(1), buffer.getNumSamples());
    
}

//==============================================================================
bool ThreeBandsAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ThreeBandsAudioProcessor::createEditor()
{
    return new ThreeBandsAudioProcessorEditor (*this);
}

//==============================================================================
void ThreeBandsAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ThreeBandsAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ThreeBandsAudioProcessor();
}
