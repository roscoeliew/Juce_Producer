/*
  ==============================================================================

    BeatVisualizer.h
    Created: 10 Mar 2024 9:12:47pm
    Author:  roscoe

  ==============================================================================
*/

// WRITTEN THE CODES BELOW PERSONALLY

#pragma once

#include <JuceHeader.h>
#include <vector>

// Represents a single beat with its strength, fade level, and radius.
struct Beat {
    float strength;
    float fade;
    float radius;
};

// A component that visualizes beats as expanding and fading circles.
class BeatVisualizer : public juce::Component {
public:
    BeatVisualizer();
    void paint(juce::Graphics& g) override;
    void addBeat(float beatStrength);

private:
    std::vector<Beat> beats; // List of beats to visualize
    const int maxBeats = 20; // Maximum number of beats to display at once
    const float radiusIncrement = 2.0f; // How much the radius of a beat increases per frame
    const float fadeRate = 0.98f; // Rate at which the beat's opacity fades
};
