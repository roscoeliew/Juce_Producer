/*
  ==============================================================================

    BeatDetector.cpp
    Created: 10 Mar 2024 9:07:35pm
    Author:  roscoe liew

  ==============================================================================
*/

// WRITTEN THE CODES BELOW PERSONALLY

#include "BeatDetector.h"

// Constructor: Initializes the beat detector with default values
BeatDetector::BeatDetector() : previousSample(0.0f), threshold(0.1f), recentBeat(false), debounceCounter(0) {
}

// Process audio buffer: Analyzes the audio buffer to detect beats
void BeatDetector::processAudioBuffer(const juce::AudioBuffer<float>& buffer) {
    auto numSamples = buffer.getNumSamples();
    auto* channelData = buffer.getReadPointer(0);
    const int debounceSamples = 4410;

    for (int i = 0; i < numSamples; ++i) {
        auto currentSample = std::abs(channelData[i]);
        if (currentSample - previousSample > threshold && !recentBeat) {
            beats.push_back(i); // Add the beat position to the list
            recentBeat = true; // Mark that a beat has been detected
            debounceCounter = debounceSamples; // Start the debounce counter
        } else if (debounceCounter > 0) {
            --debounceCounter;
            if (debounceCounter == 0) {
                recentBeat = false; // Reset the recent beat flag after debounce period
            }
        }
        previousSample = currentSample; // Store the current sample for the next iteration
    }
}

// Get beats: Returns the list of detected beat positions
const std::vector<int>& BeatDetector::getBeats() const {
    return beats;
}

// Estimate BPM: Calculates the beats per minute based on the detected beats
float BeatDetector::estimateBPM(float sampleRate) {
    if (beats.size() < 2) {
        return 0.0f; // Not enough beats to estimate BPM
    }

    std::vector<float> intervals;
    for (size_t i = 1; i < beats.size(); ++i) {
        float intervalInSeconds = (beats[i] - beats[i - 1]) / sampleRate;
        intervals.push_back(intervalInSeconds);
    }

    // Find the most common interval (simple approach)
    std::sort(intervals.begin(), intervals.end());
    float mostCommonInterval = intervals[intervals.size() / 2];

    // Convert interval to BPM
    float bpm = 60.0f / mostCommonInterval;
    return bpm;
}

// Clear beats: Clears the list of detected beats
void BeatDetector::clearBeats() {
    beats.clear();
}

