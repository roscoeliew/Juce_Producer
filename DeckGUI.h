/*
 ==============================================================================
 
 DeckGUI.h
 Created: 24 Feb 2024 00:36:33
 Author:  roscoe Liew
 
 ==============================================================================
 */
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "SpinningDeck.h"
#include "LookAndFeel.h"
#include "BeatVisualizer.h"

using namespace juce;

// DeckGUI class: A component that represents a DJ deck with controls and visualizations
class DeckGUI:  public juce::Component,
                public juce::Button::Listener,
                public juce::Slider::Listener,
                public juce::FileDragAndDropTarget,
                public juce::Timer
{
    
public:
    DeckGUI(DJAudioPlayer* player,
            juce::AudioFormatManager &     formatManagerToUse,
            juce::AudioThumbnailCache &     cacheToUse ); // Constructor
    ~DeckGUI(); // Destructor
    
    void paint (juce::Graphics&) override; // Override the paint method to draw the component
    void resized() override; // Override the resized method to layout the component
    
    void buttonClicked (juce::Button *) override; // Button click event handler
    
    void sliderValueChanged (juce::Slider *slider) override; // Slider value change event handler
    
    bool isInterestedInFileDrag (const juce::StringArray &files) override; // File drag event handler
    void filesDropped (const juce::StringArray &files, int x, int y) override; // File drop event handler
    
    void timerCallback() override; // Timer callback for updating the UI
    
    void unloadTrack(); // Unload the currently loaded track
    
    bool isEmpty() const; // Check if the deck is empty (no track loaded)
    void loadURL(const juce::URL& url); // Load a track from a URL
    void start(); // Start playback
    
private:
    
    juce::DrawableButton playButton{"Play", juce::DrawableButton::ImageFitted}; // Play button
    juce::DrawableButton stopButton{"Stop", juce::DrawableButton::ImageFitted}; // Stop button
    juce::DrawableButton pauseButton{"Pause", juce::DrawableButton::ImageFitted}; // Pause button
    juce::TextButton loadButton{"LOAD"}; // Load track button
    juce::TextButton removeButton{"REMOVE"}; // Remove track button
     
    juce::Slider volSlider; // Volume slider
    juce::Slider speedSlider; // Speed slider
    juce::Slider posSlider; // Position slider
    
    bool fileLoaded = false; // Flag to indicate if a file is loaded
    
    juce::Label volumeLabel;
    juce::Label speedLabel;
    
    juce::FileChooser fChooser{"Select a file..."};
    
    WaveformDisplay waveformDisplay; // Waveform display component
    
    DJAudioPlayer* player; // DJ audio player for playback control
    
    SpinningDeck spinningDeck; // Spinning deck visualization
    
    CustomLookAndFeel customLookAndFeel; // Custom LookAndFeel for styling
    
    BeatVisualizer beatVisualizer; // Beat visualizer component

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI) // Macro to prevent copying and leaking
};
