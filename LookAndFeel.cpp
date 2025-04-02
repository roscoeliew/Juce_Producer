/*
  ==============================================================================

    LookAndFeel.cpp
    Created: 27 Feb 2024 1:48:11am
    Author:  roscoe

  ==============================================================================
*/
#include "LookAndFeel.h"
using namespace juce;

CustomLookAndFeel::CustomLookAndFeel() {
    
    setColour(juce::ResizableWindow::backgroundColourId, juce::Colours::black);
}

CustomLookAndFeel::~CustomLookAndFeel() {
    // Destructor - clean up any resources if needed
}

void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                                          float sliderPosProportional, float rotaryStartAngle,
                                          float rotaryEndAngle, juce::Slider& slider) {
    auto radius = juce::jmin(width / 2, height / 2) - 4.0f;
    auto centreX = x + width * 0.5f;
    auto centreY = y + height * 0.5f;
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

    // Fill
    g.setColour(juce::Colours::darkgrey);
    g.fillEllipse(rx, ry, rw, rw);

    // Outline
    g.setColour(juce::Colours::darkgrey);
    g.drawEllipse(rx, ry, rw, rw, 1.0f);

    // Pointer
    juce::Path p;
    auto pointerLength = radius * 0.6f;
    auto pointerThickness = 2.0f;
    p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
    p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));
    g.setColour(juce::Colours::grey);
    g.fillPath(p);

    // Draw labels
    if (slider.getName() == "Speed") {
        juce::Array<float> speedPositions = {0.25f, 0.5f, 0.75f, 1.0f, 1.25f, 1.5f, 1.75f, 2.0f};
        float min = 0.25f;
        float max = 2.0f;

        for (float pos : speedPositions) {
            float normalizedPos = (pos - min) / (max - min);
            auto lineAngle = rotaryStartAngle + normalizedPos * (rotaryEndAngle - rotaryStartAngle);
            auto startX = centreX + (radius - 10) * std::cos(lineAngle - juce::MathConstants<float>::halfPi); // Rotate by 90 degrees
            auto startY = centreY + (radius - 10) * std::sin(lineAngle - juce::MathConstants<float>::halfPi);
            auto endX = centreX + radius * std::cos(lineAngle - juce::MathConstants<float>::halfPi);
            auto endY = centreY + radius * std::sin(lineAngle - juce::MathConstants<float>::halfPi);
            
            g.setColour(juce::Colours::purple);
            g.drawLine(startX, startY, endX, endY, 2.0f);
        }
        
    } else if (slider.getName() == "Volume") {
        // Draw lines for the volume slider
        juce::Array<float> volumePositions = {0.0f, 0.5f, 1.0f, 1.5f, 2.0f};
        for (float volume : volumePositions) {
            float normalizedPos = volume / 2.0f; // Normalize to the range [0, 1]
            auto angle = rotaryStartAngle + normalizedPos * (rotaryEndAngle - rotaryStartAngle);
            auto startX = centreX + (radius - 10) * std::cos(angle - juce::MathConstants<float>::halfPi);
            auto startY = centreY + (radius - 10) * std::sin(angle - juce::MathConstants<float>::halfPi);
            auto endX = centreX + radius * std::cos(angle - juce::MathConstants<float>::halfPi);
            auto endY = centreY + radius * std::sin(angle - juce::MathConstants<float>::halfPi);
            
            g.setColour(juce::Colours::purple);
            g.drawLine(startX, startY, endX, endY, 2.0f);
        }
    }
    
    // Value text box
        auto text = juce::String(slider.getValue(), 2); // 2 decimal places
        auto font = g.getCurrentFont().withHeight(juce::jmin(14.0f, radius * 0.5f));
        g.setFont(font);
        g.setColour(juce::Colours::white);
        g.drawText(text, rx, ry, rw, rw, juce::Justification::centred, false);
}

// Draws a label
void CustomLookAndFeel::drawLabel(Graphics& g, Label& label){
    g.fillAll(label.findColour(Label::backgroundColourId)); // Fill the background
    g.setColour(label.findColour(Label::textColourId));     // Set the text color

    auto font = label.getFont();
    g.setFont(font);

    // Calculate the area for the text, taking into account the label's border
    Rectangle<int> textArea = label.getBorderSize().subtractedFrom(label.getLocalBounds());
    g.drawText(label.getText(), textArea, Justification::centred, true);
}

// Draws the background of a button
void CustomLookAndFeel::drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
                          bool isMouseOverButton, bool isButtonDown) {
    
    auto bounds = button.getLocalBounds();
    auto cornerSize = 0.0f;  // Set to 0 for square corners
    auto borderThickness = 1.0f;  // Adjust as needed
    auto baseColour = juce::Colours::red;  // Border color
    auto buttonColour = juce::Colours::black;  // Button color

    // Set the color for the button's border and draw the border
    g.setColour(baseColour);
    g.drawRect(bounds.toFloat(), borderThickness);

    if (isButtonDown) {
        g.setColour(backgroundColour.darker());
    } else if (isMouseOverButton) {
        g.setColour(backgroundColour.brighter());
    } else {
        g.setColour(backgroundColour);
    }
    
    // Set the color for the button's background and fill the background
    g.setColour(juce::Colours::black);
    g.fillRect(bounds.reduced(borderThickness).toFloat());
}
