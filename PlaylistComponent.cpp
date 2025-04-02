/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 10 Mar 2024 9:07:35pm
    Author:  roscoe liew

  ==============================================================================
*/
#include <JuceHeader.h>
#include "PlaylistComponent.h"
#include "DeckGUI.h"

using namespace juce;

// Initializes the playlist component with references to the player and decks
PlaylistComponent::PlaylistComponent(DJAudioPlayer* _player, DeckGUI* _deckGUI1, DeckGUI* _deckGUI2)
: player(_player), deckGUI1(_deckGUI1), deckGUI2(_deckGUI2)
{
    // Add some sample tracks
    trackTitles.push_back("Track 1");
    trackTitles.push_back("Track 2");
    trackTitles.push_back("Track 3");
    trackTitles.push_back("Track 4");
    trackTitles.push_back("Track 5");
    trackTitles.push_back("Track 6");

    // Set up the table columns
    tableComponent.getHeader().addColumn("Track title", 1, 400);
    tableComponent.getHeader().addColumn("Insert", 2, 200);
    tableComponent.getHeader().addColumn("Load", 3, 200);

    // Set the table model and add it to the component
    tableComponent.setModel(this);
    addAndMakeVisible(tableComponent);
    
    trackFiles.resize(trackTitles.size());
}

PlaylistComponent::~PlaylistComponent(){
}

// Draws the background and border of the playlist component
void PlaylistComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);  // clear the background
    g.setColour (juce::Colours::red);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

// Sets the bounds of the table component
void PlaylistComponent::resized()
{
    tableComponent.setBounds(0, 0, getWidth(), getHeight());
}

// Returns the number of rows in the table
int PlaylistComponent::getNumRows()
{
    return trackTitles.size();
}

// Fills the background of each row
void PlaylistComponent::paintRowBackground(Graphics &g, int rowNumber, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected) {
        g.fillAll(Colours::darkgrey);
    } else {
        g.fillAll(Colours::black);
    }
}

// Draws the content of each cell
void PlaylistComponent::paintCell(Graphics &g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    if (columnId == 1) {
        g.setColour(juce::Colours::white); // Set text color to white
        g.drawText(trackTitles[rowNumber], 2, 0, width - 4, height, Justification::centredLeft, true);
    }
}

// Updates the component for each cell
Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate)
{
    if (columnId == 2) {
        // Play button
        if (existingComponentToUpdate == nullptr) {
            TextButton* btn = new TextButton{"Insert"};
            String id{std::to_string(rowNumber)};
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    } else if (columnId == 3) {
        // Load button
        if (existingComponentToUpdate == nullptr) {
            TextButton* btn = new TextButton{"Load"};
            String id{std::to_string(rowNumber)};
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate;
}

// Handles button click events
void PlaylistComponent::buttonClicked(Button* button) {
    int id = std::stoi(button->getComponentID().toStdString());

    if (button->getButtonText() == "Load") {
        auto fileChooserFlags = FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this, id](const FileChooser& chooser) {
            auto file = chooser.getResult();
            if (file.existsAsFile()) {
                trackFiles[id] = URL{file};
                trackTitles[id] = file.getFileNameWithoutExtension().toStdString(); // Update track title with file name
                tableComponent.updateContent(); // Refresh the table to show the updated title
                std::cout << "Loaded file: " << file.getFullPathName() << " for track " << id << std::endl;
            }
        });
    } else if (button->getButtonText() == "Insert") {
        if (trackFiles[id].isEmpty()) {
            std::cout << "No file loaded for track " << id << std::endl;
        } else {
            // Check if either of the decks is empty and load the track into the first empty deck
            if (deckGUI1->isEmpty()) {
                deckGUI1->loadURL(trackFiles[id]);
                deckGUI1->start();
                std::cout << "Loaded and playing file in Deck 1: " << trackFiles[id].toString(true) << std::endl;
            } else if (deckGUI2->isEmpty()) {
                deckGUI2->loadURL(trackFiles[id]);
                deckGUI2->start();
                std::cout << "Loaded and playing file in Deck 2: " << trackFiles[id].toString(true) << std::endl;
            } else {
                std::cout << "Both decks are occupied. Please stop a deck before loading a new track." << std::endl;
            }
        }
    }
}
