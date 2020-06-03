/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <math.h>



//==============================================================================
AnalogFattenerAudioProcessor::AnalogFattenerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

AnalogFattenerAudioProcessor::~AnalogFattenerAudioProcessor()
{
}

//==============================================================================
const String AnalogFattenerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AnalogFattenerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AnalogFattenerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AnalogFattenerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AnalogFattenerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AnalogFattenerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AnalogFattenerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AnalogFattenerAudioProcessor::setCurrentProgram (int index)
{
}

const String AnalogFattenerAudioProcessor::getProgramName (int index)
{
    return {};
}

void AnalogFattenerAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void AnalogFattenerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void AnalogFattenerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AnalogFattenerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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


template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

void AnalogFattenerAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        const auto inputData = buffer.getReadPointer (channel);
        auto* outputData = buffer.getWritePointer(channel);
        
        // ..do something to the data...
        for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            float CrankedSignal = outputData[sample] * crankValue;
            float ColoredSignal = fmin(fmax(sin(fmax(fmin(CrankedSignal, 1.0), -1.0) * const1) / const2, -1.0), 1.0);
            float SummedSignal = CrankedSignal + ColoredSignal;
            float dBL = amp_dB * log(abs(SummedSignal)) + boostValue;

            if (dBL > threshold_dB)
            {
                over_dB = dBL - threshold_dB;
                over_dB = a * over_dB + b * over_dB * over_dB;
                dBL = fmin(threshold_dB + over_dB, limitValue);
            }

            outputData[sample] = exp(dBL / amp_dB) * sgn(outputData[sample]);
        }

    }
}


//==============================================================================
bool AnalogFattenerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* AnalogFattenerAudioProcessor::createEditor()
{
    return new AnalogFattenerAudioProcessorEditor (*this);
}

//==============================================================================
void AnalogFattenerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void AnalogFattenerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AnalogFattenerAudioProcessor();
}
