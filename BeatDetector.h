/*
  ==============================================================================

    BeatDetector.h
    Created: 10 Mar 2024 9:07:35pm
    Author:  roscoe liew

  ==============================================================================
*/

// WRITTEN THE CODES BELOW PERSONALLY

#pragma once

#include <JuceHeader.h>
#include <vector>

// BeatDetector class: Detects beats in an audio buffer and estimates BPM
class BeatDetector {
public:
    BeatDetector(); // Constructor
    void processAudioBuffer(const juce::AudioBuffer<float>& buffer); // Process the audio buffer to detect beats
    const std::vector<int>& getBeats() const; // Get the list of detected beat positions
    float estimateBPM(float sampleRate); // Estimate the beats per minute
    void clearBeats(); // Clear the list of detected beats

private:
    float previousSample = 0.0f; // Previous audio sample value
    float threshold = 0.1f; // Threshold for detecting a beat (adjust based on your audio's characteristics)
    bool recentBeat = false; // Flag to indicate if a beat was recently detected
    std::vector<int> beats; // List of detected beat positions
    int debounceCounter = 0; // Counter for debouncing beat detection
};
