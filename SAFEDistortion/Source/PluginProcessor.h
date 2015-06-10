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
class SafedistortionAudioProcessor  : public SAFEAudioProcessor
{
public:
    //==============================================================================
    SafedistortionAudioProcessor();
    ~SafedistortionAudioProcessor();
    
    void parameterUpdateCalculations (int index);

    //==============================================================================
    void pluginPreparation (double sampleRate, int samplesPerBlock);
    void releaseResources();

    void pluginProcessing (AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

    //==============================================================================
    AudioProcessorEditor* createEditor();
    bool hasEditor() const;

    enum Parameters
	{
        PARAMinputGain,
        PARAMknee,
        PARAMbias,
        PARAMtone,
        PARAMoutputGain
	};    
	
	virtual String getPluginCode() override;
    
private:
    OwnedArray <IIRFilter> toneFilters;
    OwnedArray <IIRFilter> dcBlockingFilters;
    
    float freqDCBlocking;
    
    double fs;
    
    int resamplingRatio;
    float antiAliasingCutoff;
    int antiAliasingOrder;
    OwnedArray <Resampler> resamplers;
    
    HeapBlock <float> upsampledData;
    
    int numChannels;

    float inputGain, knee, bias, tone, outputGain;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SafedistortionAudioProcessor)
};

#endif  // PLUGINPROCESSOR_H_INCLUDED
