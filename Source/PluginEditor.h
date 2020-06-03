/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include <math.h>

# define M_PI           3.14159265358979323846 
//==============================================================================
/**
*/

class OtherLookAndFeel : public LookAndFeel_V4
{
public:
    OtherLookAndFeel()
    {
        setColour(Slider::thumbColourId, Colours::red);
    }
    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
    {
        auto radius = jmin(width / 2, height / 2) - 4.0f;
        auto centreX = x + width * 0.5f;
        auto centreY = y + height * 0.5f;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        // fill
        g.setColour(Colours::red);
        g.fillEllipse(rx, ry, rw, rw);

        // outline
        g.setColour(Colours::darkslategrey);
        g.drawEllipse(rx, ry, rw, rw, 1.0f);

        Path p;
        auto pointerLength = radius * 0.33f;
        auto pointerThickness = 2.0f;
        p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
        p.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY));


        // pointer
        g.setColour(Colours::grey);
        g.fillPath(p);



       
    }
};

class AnalogFattenerAudioProcessorEditor  : public AudioProcessorEditor,public Slider::Listener
{
public:
    AnalogFattenerAudioProcessorEditor (AnalogFattenerAudioProcessor&);
    ~AnalogFattenerAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;


    void sliderValueChanged(Slider* slider) override
    {
        if (slider == &crankSlider) 
        {
            processor.crankValue = static_cast<float>(crankSlider.getValue());
        }
        if (slider == &colorSlider)
        {
            processor.colorValue = static_cast<float>(colorSlider.getValue());
            processor.const1 = processor.colorValue / 200 * M_PI;
            processor.const2 = sin(processor.colorValue / 200 * M_PI);
        }
        if (slider == &boostSlider)
        {
            processor.boostValue = static_cast<float>(boostSlider.getValue());
        }
        if (slider == &limitSlider)
        {
            processor.limitValue = static_cast<float>(limitSlider.getValue());
            processor.threshold_dB = processor.baseline_threshold_dB + processor.limitValue;
        }
    }

private:

    OtherLookAndFeel sliderLookAndFeel;

    Slider crankSlider;
    Slider colorSlider;
    Slider boostSlider;
    Slider limitSlider;
    Label crankLabel;
    Label colorLabel;
    Label boostLabel;
    Label limitLabel;



    const float border = 100.0;
    float lowerKnobSize = 70.0;

    AnalogFattenerAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AnalogFattenerAudioProcessorEditor)
};
