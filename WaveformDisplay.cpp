/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 14 Mar 2020 3:50:16pm
    Author:  roscoe liew

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveformDisplay.h"

using namespace juce;

// Constructor: Initializes the waveform display with the audio format manager and thumbnail cache
WaveformDisplay::WaveformDisplay(AudioFormatManager & 	formatManagerToUse,
                                 AudioThumbnailCache & 	cacheToUse) :
                                 audioThumb(1000, formatManagerToUse, cacheToUse), 
                                 fileLoaded(false), 
                                 position(0){

     audioThumb.addChangeListener(this); // Register as a listener to the audio thumbnail
}

WaveformDisplay::~WaveformDisplay(){
}

// Draws the waveform and playhead
void WaveformDisplay::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::red);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::purple);
    if(fileLoaded)
    {
      audioThumb.drawChannel(g, 
        getLocalBounds(), 
        0, 
        audioThumb.getTotalLength(), 
        0, 
        1.0f // Draw the waveform
      );
      g.setColour(Colours::lightgreen);
      g.drawRect(position * getWidth(), 0, getWidth() / 20, getHeight());
    }
    else 
    {
      g.setFont (20.0f);
      g.drawText ("File not loaded...", getLocalBounds(),
                  Justification::centred, true);

    }
}

// Handles component resizing (not used in this implementation)
void WaveformDisplay::resized(){

}

// Loads an audio file from a URL into the waveform display
void WaveformDisplay::loadURL(URL audioURL)
{
  audioThumb.clear();
  fileLoaded  = audioThumb.setSource(new URLInputSource(audioURL));
  if (fileLoaded)
  {
    std::cout << "wfd: loaded! " << std::endl;
    repaint();
  }
  else {
    std::cout << "wfd: not loaded! " << std::endl;
  }

}

// Called when the audio thumbnail changes
void WaveformDisplay::changeListenerCallback (ChangeBroadcaster *source)
{
    std::cout << "wfd: change received! " << std::endl;

    repaint();

}

// Set the relative position of the playhead
void WaveformDisplay::setPositionRelative(double pos)
{
  if (pos != position)
  {
    position = pos;
    repaint();
  }
}

// Clear the waveform display
void WaveformDisplay::clear() {
    audioThumb.clear(); // Clear the AudioThumbnail
    repaint(); // Repaint the component to reflect the cleared state
}


