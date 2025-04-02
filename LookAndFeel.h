/*
==============================================================================

    LookAndFeel.h
    Created: 27 Feb 2024 1:48:11am
    Author:  roscoe

  ==============================================================================
*/
#pragma once

#include <JuceHeader.h>
using namespace juce;

class CustomLookAndFeel : public juce::LookAndFeel_V4 {
public:
    CustomLookAndFeel();
    ~CustomLookAndFeel() override;

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPosProportional, float rotaryStartAngle,
                          float rotaryEndAngle, juce::Slider& slider) override;
    
    void drawLabel(Graphics& g, Label& label) override;
    
    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
                                                 bool isMouseOverButton, bool isButtonDown) override;
    
};
