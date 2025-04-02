/*
  ==============================================================================

    DeckGUI.cpp
    Created: 24 Feb 2024 00:36:33
    Author:  roscoe Liew

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"
using namespace juce;

// Constructor: Initializes the DJ deck with controls and visualizations
DeckGUI::DeckGUI(DJAudioPlayer* _player,
                AudioFormatManager &     formatManagerToUse,
                AudioThumbnailCache &     cacheToUse)
                : player(_player),
                waveformDisplay(formatManagerToUse, cacheToUse)
{
    // Create a triangle path for the play icon
    juce::Path playPath;
    playPath.addTriangle(0, 0, 0, 10, 10, 5);

    // Create a square path for the stop icon
    juce::Path stopPath;
    stopPath.addRectangle(0, 0, 10, 10);

    // Create paths for the two vertical bars of the pause icon
    juce::Path pausePath;
    pausePath.addRectangle(0, 0, 3, 10); // First bar
    pausePath.addRectangle(7, 0, 3, 10); // Second bar

    // Create drawable objects for the icons
    auto playDrawable = std::make_unique<juce::DrawablePath>();
    playDrawable->setPath(playPath);
    playDrawable->setFill(juce::Colours::red);

    auto stopDrawable = std::make_unique<juce::DrawablePath>();
    stopDrawable->setPath(stopPath);
    stopDrawable->setFill(juce::Colours::red);

    // Create a drawable object for the pause icon
    auto pauseDrawable = std::make_unique<juce::DrawablePath>();
    pauseDrawable->setPath(pausePath);
    pauseDrawable->setFill(juce::Colours::red);

    // Set the drawables for the buttons
    playButton.setImages(playDrawable.get());
    stopButton.setImages(stopDrawable.get());
    pauseButton.setImages(pauseDrawable.get());

    // Add buttons and sliders to the component
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(pauseButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(removeButton);
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(waveformDisplay);
    
    // Set slider styles and labels
    volSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    speedSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    volSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    speedSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    volSlider.setName("Volume");
    speedSlider.setName("Speed");
    volumeLabel.setText("Volume", juce::dontSendNotification);
    speedLabel.setText("Speed", juce::dontSendNotification);
    addAndMakeVisible(volumeLabel);
    addAndMakeVisible(speedLabel);

    // Add listeners to the buttons
    playButton.addListener(this);
    stopButton.addListener(this);
    pauseButton.addListener(this);
    loadButton.addListener(this);
    removeButton.addListener(this);
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);
    
    // Set up the position slider with a text function
    posSlider.textFromValueFunction = [](double value) {
        int minutes = static_cast<int>(value) / 60;
        int seconds = static_cast<int>(value) % 60;
        return juce::String::formatted("%d:%02d", minutes, seconds);
    };

    // Set ranges for sliders
    volSlider.setRange(0.0, 2.0);
    speedSlider.setRange(0.25, 2.0);
    posSlider.setRange(0.0, 1.0);

    // Set default value for the speed slider
    speedSlider.setValue(1.0, juce::dontSendNotification);
    volSlider.setValue(1.0, juce::dontSendNotification);
    
    // Add the spinning deck and beat visualizer to the component
    addAndMakeVisible(spinningDeck);
    addAndMakeVisible(beatVisualizer);
    
    volSlider.setLookAndFeel(&customLookAndFeel);
    speedSlider.setLookAndFeel(&customLookAndFeel);
    posSlider.setLookAndFeel(&customLookAndFeel);
    loadButton.setLookAndFeel(&customLookAndFeel);
    removeButton.setLookAndFeel(&customLookAndFeel);
    
    // Start the timer
    startTimer(500);
}

// Destructor: Stops the timer and resets the LookAndFeel for sliders
DeckGUI::~DeckGUI()
{
    stopTimer();
    
    volSlider.setLookAndFeel(nullptr);
    speedSlider.setLookAndFeel(nullptr);
}

//Draws the component with a black background and red borders
void DeckGUI::paint (Graphics& g)
{
    g.fillAll(juce::Colours::black); // Set the background color to black
    juce::Colour borderColor = juce::Colours::red; // Set the border color to red
    g.setColour(borderColor);

    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    g.drawRect(playButton.getBounds(), 1); // Draw the outline
    g.drawRect(stopButton.getBounds(), 1); // Draw the outline
    g.drawRect(pauseButton.getBounds(), 1); // Draw the outline

    g.setColour (Colours::white);
    g.setFont (14.0f);
    Rectangle<int> textArea(0, stopButton.getBottom(), getWidth(), 20); // Position the text area just below the stop button
    g.drawText("DeckGUI", textArea, Justification::centred, true);   // Draw the text centered in the text area
}

// Layouts the components within the DeckGUI
void DeckGUI::resized()
{
    double rowH = getHeight() / 14; // Calculate the height for each row
    spinningDeck.setBounds(0, 0, getWidth(), rowH * 4); // Use 4 rows for the spinning deck

    int componentIndex = 4; // Start positioning other components below the spinning deck
    waveformDisplay.setBounds(0, rowH * componentIndex, getWidth(), rowH * 2); // Waveform display takes two rows
    
    componentIndex += 2;
    int buttonWidth = getWidth() / 3; // Divide the width by 3 for each button
    playButton.setBounds(0, rowH * componentIndex, buttonWidth, rowH);
    pauseButton.setBounds(buttonWidth, rowH * componentIndex, buttonWidth, rowH);
    stopButton.setBounds(2 * buttonWidth, rowH * componentIndex, buttonWidth, rowH);
    componentIndex++;

    int knobWidth = getWidth() / 2; // Divide the width by 2 for each knob
    volSlider.setBounds(0, rowH * componentIndex, knobWidth, rowH * 2);
    speedSlider.setBounds(knobWidth, rowH * componentIndex, knobWidth, rowH * 2);
    // Position labels below the sliders and center them
    int labelHeight = 15; // Set the height for the labels
    int labelWidth = getWidth() / 4; // Set the width for the labels
    volumeLabel.setBounds(volSlider.getX() + (volSlider.getWidth() - labelWidth) / 2, volSlider.getBottom() + 5, labelWidth, labelHeight);
    speedLabel.setBounds(speedSlider.getX() + (speedSlider.getWidth() - labelWidth) / 2, speedSlider.getBottom() + 5, labelWidth, labelHeight);
        
    componentIndex += 2;
    posSlider.setBounds(0, (rowH * componentIndex++) + 15 , getWidth(), rowH);
    loadButton.setBounds(0, (rowH * componentIndex++) + 15, getWidth(), rowH);
    removeButton.setBounds(0, (rowH * componentIndex++) + 15, getWidth(), rowH);
    beatVisualizer.setBounds(0, (rowH * componentIndex++) + 15, getWidth(), rowH * 2);
}

//Updates the UI elements periodically
void DeckGUI::timerCallback()
{
    waveformDisplay.setPositionRelative(player->getPositionRelative()); // Update the waveform display position
    
    // Update BeatVisualizer with detected beats
    auto detectedBeats = player->getBeatDetector().getBeats();
    for (auto beat : detectedBeats) {
        beatVisualizer.addBeat(1.0f);
    }
    player->getBeatDetector().clearBeats(); // Clear the detected beats after updating the visualizer
    
    auto positionRelative = player->getPositionRelative();
    
    if (fileLoaded) // Check if a file has been loaded
    {
        auto positionRelative = player->getPositionRelative();
        posSlider.setValue(positionRelative * posSlider.getMaximum(), juce::dontSendNotification); // Update the position slider value
    }

    beatVisualizer.repaint();
}

// Button click event handler: Handles the actions for each button
void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton)
    {
        std::cout << "Play button was clicked " << std::endl;
        player->start();
        spinningDeck.setSpinning(true); // Start spinning
    }
     if (button == &pauseButton)
    {
        std::cout << "Stop button was clicked " << std::endl;
        player->stop();
        spinningDeck.setSpinning(false); // Stop spinning

    }
     if (button == &loadButton)
     {
         auto fileChooserFlags =
         FileBrowserComponent::canSelectFiles;
         fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
                              {
             player->loadURL(URL{chooser.getResult()}); // Load the file into the DJAudioPlayer
             waveformDisplay.loadURL(URL{chooser.getResult()});   // Load the file into the DJAudioPlayer
             double audioLength = player->getLengthInSeconds();
             posSlider.setRange(0.0, audioLength, 0.01);
             fileLoaded = true; // Set fileLoaded to true
             
         });
     }
    
    if (button == &stopButton)
    {
        player->stop();
        player->setPosition(0); // Reset the position to the start
        spinningDeck.setSpinning(false); // Stop spinning
        player->getBeatDetector().clearBeats(); // Clear the detected beats after updating the visualizer
    }
    
    if (button == &removeButton)
    {
        unloadTrack(); // Unload the currently loaded track
    }
}

// Handles the actions for each slider
void DeckGUI::sliderValueChanged (Slider *slider)
{
    std::cout << "Slider value changed" << std::endl;
    
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue()); // Set the gain (volume) for the DJAudioPlayer
    }

    if (slider == &speedSlider)
    {
        std::cout << "Speed slider value: " << slider->getValue() << std::endl;
        player->setSpeed(slider->getValue()); // Set the speed (playback rate) for the DJAudioPlayer
    }
    
    if (slider == &posSlider)
    {
        double newPosition = posSlider.getValue();
                player->setPosition(newPosition); // Set the position for the DJAudioPlayer
    }
    
}

// Checks if the component is interested in the dragged files
bool DeckGUI::isInterestedInFileDrag (const StringArray &files)
{
  std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
  return true;
}

// Loads the dropped file into the DJAudioPlayer
void DeckGUI::filesDropped (const StringArray &files, int x, int y)
{
  std::cout << "DeckGUI::filesDropped" << std::endl;
  if (files.size() == 1)
  {
    player->loadURL(URL{File{files[0]}});
  }
}

// Check if the deck is empty (no track loaded)
bool DeckGUI::isEmpty() const
{
    return !player->isPlaying() && !player->isLoaded();
}

// Load a track from a URL into the DJAudioPlayer and waveform display
void DeckGUI::loadURL(const juce::URL& url)
{
    player->loadURL(url); // Load the URL into the DJAudioPlayer
    waveformDisplay.loadURL(url); // Load the URL into the waveform display
    double audioLength = player->getLengthInSeconds(); // Get the length of the audio track
    posSlider.setRange(0.0, audioLength, 0.01);
    fileLoaded = true; // Set fileLoaded to true
}

// Start playback in the DJAudioPlayer
void DeckGUI::start()
{
    player->start();
}

// Unload the currently loaded track from the DJAudioPlayer and clear the waveform display
void DeckGUI::unloadTrack() {
    
    player->unloadTrack(); // Unload the track from the DJAudioPlayer
    player->setPosition(0); // Reset the position to the start
    spinningDeck.setSpinning(false); // Stop spinning
    player->getBeatDetector().clearBeats(); // Clear the detected beats after updating the visualizer
    waveformDisplay.clear(); // Clear the waveform display
    fileLoaded = false; // Update the fileLoaded flag
}
