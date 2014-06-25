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
class SafeequaliserAudioProcessor  : public SAFEAudioProcessor
{
public:
    //==============================================================================
    SafeequaliserAudioProcessor();
    ~SafeequaliserAudioProcessor();
    
    //==============================================================================
    void parameterUpdateCalculations (int index);
    void updateFilters (int band);

    //==============================================================================
    void pluginPreparation (double sampleRate, int samplesPerBlock);
    void releaseResources();

    void pluginProcessing (AudioSampleBuffer& buffer);
    
    //==============================================================================
    AudioProcessorEditor* createEditor();
    bool hasEditor() const;

    enum Parameters
	{       
        PARAMgain0,
        PARAMfreq0,
        
        PARAMgain1,
        PARAMfreq1,
        PARAMqFactor1,
        
        PARAMgain2,
        PARAMfreq2,
        PARAMqFactor2,
        
        PARAMgain3,
        PARAMfreq3,
        PARAMqFactor3,
        
        PARAMgain4,
        PARAMfreq4  
	};
    
private:
    int numFilters;
    double fs;
    
    HeapBlock <float> gains;
    HeapBlock <float> freqs;
    HeapBlock <float> qFactors;
    
    int numChannels;
    
    OwnedArray <IIRFilter> eqFilters;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SafeequaliserAudioProcessor)
};

#endif  // PLUGINPROCESSOR_H_INCLUDED
