/*
==============================================================================

    DJAudioPlayer.cpp
    Created: 24 Feb 2024 00:36:33
    Author:  roscoe Liew

==============================================================================
*/

#include "DJAudioPlayer.h"
using namespace juce;

// Constructor: Initializes the DJ audio player with an audio format manager
DJAudioPlayer::DJAudioPlayer(AudioFormatManager& _formatManager)
: formatManager(_formatManager), resampleSource(&transportSource, false, 2)
{
     // Set transportSource as the source for resampleSource
}
DJAudioPlayer::~DJAudioPlayer(){
}

// Prepares the audio sources for playback
void DJAudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

}

// Fills the buffer with audio data and processes beats
void DJAudioPlayer::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    resampleSource.getNextAudioBlock(bufferToFill);
    beatDetector.processAudioBuffer(*bufferToFill.buffer);
}

// Releases resources used by the audio sources
void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

// Loads an audio file from a URL
void DJAudioPlayer::loadURL(URL audioURL)
{
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) // good file!
    {
        sampleRate = reader->sampleRate;
        std::unique_ptr<AudioFormatReaderSource> newSource (new AudioFormatReaderSource (reader, true));
        transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset (newSource.release());          
    }
}

// Sets the gain (volume) of the audio playback
void DJAudioPlayer::setGain(double gain)
{
    if (gain < 0 || gain > 2.0)
    {
        std::cout << "DJAudioPlayer::setGain gain should be between 0 and 2.0" << std::endl;
    }
    else {
        transportSource.setGain(gain);
    }
   
}

// Sets the playback speed (ratio)
void DJAudioPlayer::setSpeed(double ratio)
{
  std::cout << "setSpeed called with value: " << ratio << std::endl;
  if (ratio < 0.25 || ratio > 2.0)
    {
        std::cout << "DJAudioPlayer::setSpeed ratio should be between 0.25 and 2.0" << std::endl;
    }
    else {
        std::cout << "Setting speed to: " << ratio << std::endl;
        resampleSource.setResamplingRatio(ratio);
    }
}

// Sets the playback position as a relative value (0 to 1)
void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
     if (pos < 0 || pos > 1.0)
    {
        std::cout << "DJAudioPlayer::setPositionRelative pos should be between 0 and 1" << std::endl;
    }
    else {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

// Returns the length of the loaded audio track in seconds
double DJAudioPlayer::getLengthInSeconds() const
{
    return readerSource ? readerSource->getTotalLength() / sampleRate : 0.0;
}

// Starts playback
void DJAudioPlayer::start()
{
    transportSource.start();
}

// Stops playback
void DJAudioPlayer::stop()
{
  transportSource.stop();
}

// Returns the relative position of the playhead (0 to 1)
double DJAudioPlayer::getPositionRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

// Returns true if the audio player is currently playing
bool DJAudioPlayer::isPlaying() const {
    return transportSource.isPlaying();
}

// Returns true if an audio track is loaded
bool DJAudioPlayer::isLoaded() const {
    return readerSource != nullptr;
}

// Stops playback and unloads the current audio track
void DJAudioPlayer::unloadTrack() {
    stop(); // Stop playback
    transportSource.setSource(nullptr); // Disconnect the current audio source
    readerSource.reset(); // Reset the reader source
}
