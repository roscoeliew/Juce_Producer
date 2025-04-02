/*
  ==============================================================================

    DJAudioPlayer.h
     Created: 24 Feb 2024 00:36:33
     Author:  roscoe Liew

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "BeatDetector.h"

using namespace juce;

// DJAudioPlayer class: Handles audio playback and processing for a DJ application
class DJAudioPlayer : public juce::AudioSource {
  public:

    DJAudioPlayer(juce::AudioFormatManager& _formatManager);
    ~DJAudioPlayer();

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override; // Prepare to play
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override; // Get next audio block
    void releaseResources() override; // Release resources
    
    void loadURL(juce::URL audioURL); // Load an audio file from a URL
    void setGain(double gain); // Set the gain (volume)
    void setSpeed(double ratio); // Set the playback speed
    void setPosition(double posInSecs); // Set the playback position in seconds
    void setPositionRelative(double pos); // Set the playback position as a relative value
    

    void start(); // Start playback
    void stop(); // Stop playback
    void unloadTrack(); // Unload the currently loaded track

    double getPositionRelative(); // Get the relative position of the playhead
    double getLengthInSeconds() const; // Get the length of the loaded audio track in seconds
    
    bool isPlaying() const; // Check if the audio player is currently playing
    bool isLoaded() const; // Check if an audio track is loaded
    
    BeatDetector& getBeatDetector() { return beatDetector; } // Get the beat detector instance

private:
    juce::AudioFormatManager& formatManager; // Audio format manager for reading audio files
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource; // Source for reading audio files
    juce::AudioTransportSource transportSource;  // Transport source for controlling playback
    juce::ResamplingAudioSource resampleSource{&transportSource, false, 2}; // Resampling source for changing playback speed
    
    double sampleRate = 0.0; // Sample rate of the loaded audio track
    
    BeatDetector beatDetector; // Beat detector for analyzing the audio waveform
};
