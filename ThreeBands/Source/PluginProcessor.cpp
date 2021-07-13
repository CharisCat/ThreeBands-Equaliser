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

    // If we want to control our filter with a slider - it needs to be in the process block
    lowShelf.setCoefficients(juce::IIRCoefficients::makeLowShelf(sampleRate, 500.0f,  2.0f, juce::Decibels::decibelsToGain(lowFreqGain)));
    midPeaking.setCoefficients(juce::IIRCoefficients::makePeakFilter(sampleRate, 750.0f, 2.0f, juce::Decibels::decibelsToGain(midFreqGain)));
    highShelf.setCoefficients(juce::IIRCoefficients::makeHighShelf(sampleRate, 1000.0f, 2.0f, juce::Decibels::decibelsToGain(highFreqGain)));
    //highShelf.setCoefficients(juce::IIRCoefficients::makeHighShelf(<#double sampleRate#>, <#double cutOffFrequency#>, <#double Q#>, <#float gainFactor#>));
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    auto* channelData = buffer.getWritePointer (0);
    lowShelf.processSamples(channelData, buffer.getNumSamples());
    midPeaking.processSamples(channelData, buffer.getNumSamples());
    highShelf.processSamples(channelData, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample) //goes through all samples in buffer
        {
            channelData [sample] = channelData[sample] * juce::Decibels::decibelsToGain(slider1Gain); //multiplies samples by the slider value to attenuate volume, measured in decibels
        }
        
        
        
        //buffer.applyGain (channel, 0, buffer.getNumSamples(), 0.5*channel);
        // This is kind of cheating but it technically creates an EQ setting.
        // I'm not sure if they're meant to be effected separately for the task
        

        // ..do something to the data...
    }
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
