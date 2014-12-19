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
class SafeharmonicsAudioProcessor  : public SAFEAudioProcessor
{
public:
    //==============================================================================
    SafeharmonicsAudioProcessor();
    ~SafeharmonicsAudioProcessor();

    void parameterUpdateCalculations (int index);
    
    //==============================================================================
    void pluginPreparation (double sampleRate, int samplesPerBlock);
    void releaseResources();

    void pluginProcessing (AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

    //==============================================================================
    AudioProcessorEditor* createEditor();
    bool hasEditor() const;
    
    double getFundamental();
    
    enum Parameters
    {
        F0Gain,
        F1Gain,
        F2Gain,
        F3Gain,
        F4Gain,
        F5Gain,
        MasterGain,
        Method
    };
    
    enum HarmonicGenerationMethod
    {
        SSB,
        IAP
    } method;

private:
    double fundamental;
    
    FundamentalTracker f0Tracker;
    
    HeapBlock <float> synthPhases;
    
    OwnedArray <IIRFilter> f0Filters;
    OwnedArray <QuadratureFilter> quadFilters;
    
    AudioSampleBuffer realSignal, imagSignal;
    AudioSampleBuffer f1, f2, f3, f4, f5;
    
    int numInputs;
    double fs;
    
    float f0Amplitude, f1Amplitude, f2Amplitude, f3Amplitude, f4Amplitude, f5Amplitude, gain;
    float methodParam;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SafeharmonicsAudioProcessor)
};

#endif  // PLUGINPROCESSOR_H_INCLUDED
