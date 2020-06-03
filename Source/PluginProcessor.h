/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <math.h>

# define M_PI           3.14159265358979323846 

//==============================================================================
/**
*/
class AnalogFattenerAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    AnalogFattenerAudioProcessor();
    ~AnalogFattenerAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;


    float crankValue = 1;
    float colorValue = 1;
    float boostValue = 0;
    float limitValue = 0;

    float const1 = colorValue / 200 * M_PI;
    float const2 = sin(colorValue / 200 * M_PI);

    float threshold_dB = baseline_threshold_dB + limitValue;
    float over_dB;

    float amp_dB = 8.6562;
    float baseline_threshold_dB = -5.0;
    float a = 1.017;
    float b = -0.025;

    float bpos = 0;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AnalogFattenerAudioProcessor)
};
