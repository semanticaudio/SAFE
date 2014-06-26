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
class SafereverbAudioProcessor  : public SAFEAudioProcessor
{
public:
    //==============================================================================
    SafereverbAudioProcessor();
    ~SafereverbAudioProcessor();
    
    void parameterUpdateCalculations (int index);

    //==============================================================================
    void pluginPreparation (double sampleRate, int samplesPerBlock);
    void releaseResources();

    void pluginProcessing (AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

    enum Parameters
	{
        PARAMdampingFreq,
        PARAMdensity,
        PARAMbandwidthFreq,
        PARAMdecay,
        PARAMpredelay,
        PARAMsize,
        PARAMgain,
        PARAMmix,
        PARAMearlyMix
	};
    
    //==============================================================================
    AudioProcessorEditor* createEditor();
    bool hasEditor() const;

private:
    MVerb <float> reverb;
    AudioSampleBuffer tempInput, tempOutput;
    
    float dampingFreq, density, bandwidthFreq, decay, predelay, size, gain, mix, earlyMix;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SafereverbAudioProcessor)
};

#endif  // PLUGINPROCESSOR_H_INCLUDED
