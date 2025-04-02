/*
 ==============================================================================
 
 SpinningDeck.h
 Created: 24 Feb 2024 00:36:33
 Author:  roscoe liew
 
 ==============================================================================
 */

// WRITTEN THE CODES BELOW PERSONALLY

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

// SpinningDeck class: A component that represents a spinning deck with a circular disc
class SpinningDeck : public juce::Component, public juce::Timer {
public:
    SpinningDeck(); // Constructor
    void paint(juce::Graphics&) override; // Override the paint method to draw the component
    void setRotationAngle(float angle); // Set the rotation angle for the spinning animation
    void setImage(const juce::Image& image); // Set an image for the deck (not used in this example)
    void setSpinning(bool shouldSpin); // Start or stop the spinning animation
    void timerCallback() override; // Timer callback for updating the rotation angle

private:
    juce::Image deckImage; // Image for the deck (not used in this example)
    float rotationAngle = 0.0f; // Current rotation angle of the deck
    bool isSpinning = false; // Flag to indicate whether the deck is spinning

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpinningDeck) // Macro to prevent copying and leaking
};
