/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
/**
*/
class SafecompressorAudioProcessor  : public SAFEAudioProcessor
{
public:
    //==============================================================================
    SafecompressorAudioProcessor();
    ~SafecompressorAudioProcessor();

    //==============================================================================
    void pluginPreparation (double sampleRate, int samplesPerBlock);
    void releaseResources();

    void pluginProcessing (AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

    //==============================================================================
    AudioProcessorEditor* createEditor();
    bool hasEditor() const;
    
    enum Parameters
	{
        PARAMthreshold,
        PARAMratio,
        PARAMknee,
        PARAMatime,
        PARAMrtime,
        PARAMmakeUpGain
	}; 
	
	void compress (AudioSampleBuffer &buffer);
	
	float getInputLevel();
	float getOutputLevel();
    
private:
    int numChannels;
    double fs;
    int numSamples;
    
    // Compressor parameters
    float thresholdIndB;
    float ratio;
    float kneeIndB;
    float atimeInMs;
    float rtimeInMs;
    float makeUpGain;
    float alphaAttack;
    float alphaRelease;
    
    // Internal variables
    float inputIndB, compIndB, outputIndB, compIndB_prev;   // input, compression, output, previous compression
    HeapBlock <float> controlVoltage; // control
    
    AudioSampleBuffer inputBuffer;
    AudioSampleBuffer rmsDetector;
    int rmsCounter;
    
    float currentInputLevel, currentOutputLevel;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SafecompressorAudioProcessor)
};

#endif  // PLUGINPROCESSOR_H_INCLUDED
