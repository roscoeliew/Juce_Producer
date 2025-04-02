/*
 ==============================================================================
 
 SpinningDeck.cpp
 Created: 24 Feb 2024 00:36:33
 Author:  roscoe liew
 
 ==============================================================================
 */

// WRITTEN THE CODES BELOW PERSONALLY

#include "SpinningDeck.h"
using namespace juce;

// Constructor: Initializes the deck image and sets up the component
SpinningDeck::SpinningDeck() {
    // Load an image for the deck (not used in this example)
    deckImage = juce::ImageFileFormat::loadFrom(juce::File("/Users/roscoe/OOP/NewProject/test.jpg"));
}

// Paint method: Draws the spinning deck with a circular disc and text
void SpinningDeck::paint(juce::Graphics& g) {
    // Clear the background
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
    
    // Draw a red border around the component
    g.setColour(juce::Colours::red);
    g.drawRect(getLocalBounds(), 1);

    // Calculate the center and radius of the circular disc
    auto width = getWidth();
    auto height = getHeight();
    auto centerX = width * 0.5f;
    auto centerY = height / 2.0f;
    auto radius = juce::jmin(width, height) * 0.4f;

    // Apply the rotation transform to the graphics context
    g.addTransform(juce::AffineTransform::rotation(rotationAngle, centerX, centerY));

    // Draw the black circular disc
    g.setColour(juce::Colours::black);
    g.fillEllipse(centerX - radius, centerY - radius, 2 * radius, 2 * radius);

    // Draw a red border around the disc
    g.setColour(juce::Colours::red);
    g.drawEllipse(centerX - radius, centerY - radius, 2 * radius, 2 * radius, 2.0f);

    // Draw the "DJ DECK" text in red on the disc
    g.setColour(juce::Colours::red);
    g.setFont(juce::Font(20.0f, juce::Font::bold));
    auto textWidth = g.getCurrentFont().getStringWidth("DJ DECK");
    g.drawText("DJ DECK", centerX - textWidth / 2, centerY - 10, textWidth, 20, juce::Justification::centred, false);
}

// Set the rotation angle for the spinning animation
void SpinningDeck::setRotationAngle(float angle) {
    if (isSpinning) {
        rotationAngle = angle;
        repaint();
    }
}

// Set an image for the deck (not used in this example)
void SpinningDeck::setImage(const juce::Image& image) {
    deckImage = image;
    repaint();
}

// Timer callback: Updates the rotation angle and repaints the component
void SpinningDeck::timerCallback() {
    if (isSpinning) {
        rotationAngle += 0.05f; // Adjust this value to control the speed
        repaint();
    }
}

// Start or stop the spinning animation
void SpinningDeck::setSpinning(bool shouldSpin) {
    isSpinning = shouldSpin;
    if (isSpinning) {
        startTimer(40); // Start the timer with an interval of 40 milliseconds
    } else {
        stopTimer(); // Stop the timer when not spinning
        rotationAngle = 0.0f; // Reset the angle when stopping
        repaint();
    }
}
