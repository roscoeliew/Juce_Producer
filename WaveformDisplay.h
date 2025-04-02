/*
  ==============================================================================

    WaveformDisplay.h
    Created: 14 Mar 2020 3:50:16pm
    Author:  roscoe liew

  ==============================================================================
*/
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
using namespace juce;

// WaveformDisplay class: Displays the waveform of an audio file and the playhead position
class WaveformDisplay    : public Component,
                           public ChangeListener
{
public:
    WaveformDisplay( AudioFormatManager & 	formatManagerToUse,
                    AudioThumbnailCache & 	cacheToUse );
    ~WaveformDisplay();

    void paint (Graphics&) override; // Override the paint method to draw the waveform and playhead
    void resized() override; // Override the resized method to handle component resizing

    void changeListenerCallback (ChangeBroadcaster *source) override; // Callback for handling changes in the audio thumbnail

    void loadURL(URL audioURL); // Load an audio file from a URL
    
    void clear(); // Clear the waveform display

    void setPositionRelative(double pos);  // Set the relative position of the playhead

private:
    AudioThumbnail audioThumb; // Audio thumbnail for displaying the waveform
    bool fileLoaded; // Flag to indicate if a file is loaded
    double position; // Relative position of the playhead
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
