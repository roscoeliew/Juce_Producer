/*
  ==============================================================================

    BeatVisualizer.cpp
    Created: 10 Mar 2024 9:12:47pm
    Author:  roscoe

  ==============================================================================
*/

// WRITTEN THE CODES BELOW PERSONALLY

#include "BeatVisualizer.h"

BeatVisualizer::BeatVisualizer() {}

// Adds a new beat with the given strength to the visualizer.
void BeatVisualizer::addBeat(float beatStrength) {
    beats.push_back({beatStrength, 1.0f, 5.0f}); // Start with a small radius
    repaint();
}

// Draws the beat visualizer.
void BeatVisualizer::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::black); // Fill the background with black
    g.setColour(juce::Colours::red); // Set the border color to red
    g.drawRect(getLocalBounds(), 1); // Draw the border around the component
    
    auto center = getLocalBounds().getCentre().toFloat(); // Center of the component

    // Iterate through each beat and draw it as an expanding and fading circle
    for (auto& beat : beats) {
        float diameter = beat.radius * 2.0f;
        g.setColour(juce::Colours::purple.withAlpha(beat.fade)); // Set the color with fading
        g.drawEllipse(center.x - beat.radius, center.y - beat.radius, diameter, diameter, 2.0f);
        beat.radius += radiusIncrement; // Increase the radius for the expanding effect
        beat.fade *= fadeRate; // Reduce the fade value for fading out
    }

    // Remove beats that have faded out completely
    beats.erase(std::remove_if(beats.begin(), beats.end(), [](const Beat& beat) {
        return beat.fade < 0.01f;
    }), beats.end());

    repaint(); // Continuously repaint to animate the ripples
}
