/*
  ==============================================================================

    PlaylistComponent.h
    Created: 10 Mar 2024 9:07:35pm
    Author:  roscoe liew

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include "DJAudioPlayer.h"
#include "DeckGUI.h" 

using namespace juce;

// Manages a playlist of tracks and provides a UI for loading and inserting tracks into decks
class PlaylistComponent  : public juce::Component, public TableListBoxModel, public Button::Listener
{
public:
    PlaylistComponent(DJAudioPlayer* _player, DeckGUI* _deckGUI1, DeckGUI* _deckGUI2); // Initializes the playlist component with pointers to the DJAudioPlayer and two DeckGUIs
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override; // Paint method to draw the component
    void resized() override; // Resized method to layout the component
    
    // TableListBoxModel overrides:
    int getNumRows() override; // Returns the number of rows in the table (number of tracks)
    void paintRowBackground(Graphics & g, int rowNumber, int width, int height, bool rowIsSelected) override; // Paints the background of a table row
    void paintCell(Graphics & g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override; // Paints the content of a table cell

    // Creates or updates a custom component for a table cell
    Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate) override;
    
    void buttonClicked(Button * button) override; // Button click event handler
    
private:
    TableListBox tableComponent; // The table component for displaying the playlist
    std::vector<std::string> trackTitles; // Vector of track titles
    std::vector<juce::URL> trackFiles; // Vector of track file URLs
    
    juce::FileChooser fChooser{"Select a file..."}; // File chooser for loading tracks
    
    DJAudioPlayer* player; // Pointer to the DJAudioPlayer
    
    DeckGUI* deckGUI1; // Pointer to the first DeckGUI
    DeckGUI* deckGUI2; // Pointer to the second DeckGUI
    
    CustomLookAndFeel customLookAndFeel; // Custom LookAndFeel for styling the playlist
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
