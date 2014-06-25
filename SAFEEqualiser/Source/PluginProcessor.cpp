/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
SafeequaliserAudioProcessor::SafeequaliserAudioProcessor()
{
    numFilters = 5;
    gains.allocate (numFilters, true);
    freqs.allocate (numFilters, true);
    qFactors.allocate (numFilters, true);
    
    float frequencySkewFactor = 0.25;
    float qSkewFactor = 0.5;
    
    addDBParameter ("Band 1 Gain", gains [0], 0.0f, -12.0f, 12.0f, "dB");
    addParameter ("Band 1 Frequency", freqs [0], 150.0f, 22.0f, 1000.0f, "Hz", frequencySkewFactor);
    qFactors [0] = 0.71f;
    
    addDBParameter ("Band 2 Gain", gains [1], 0.0f, -12.0f, 12.0f, "dB");
    addParameter ("Band 2 Frequency", freqs [1], 560.0f, 82.0f, 3900.0f, "Hz", frequencySkewFactor);
    addParameter ("Band 2 Q Factor", qFactors [1], 0.71f, 0.1f, 10.0f, String::empty, qSkewFactor);
    
    addDBParameter ("Band 3 Gain", gains [2], 0.0f, -12.0f, 12.0f, "dB");
    addParameter ("Band 3 Frequency", freqs [2], 1000.0f, 180.0f, 4700.0f, "Hz", frequencySkewFactor);
    addParameter ("Band 3 Q Factor", qFactors [2], 0.71f, 0.1f, 10.0f, String::empty, qSkewFactor);    
    
    addDBParameter ("Band 4 Gain", gains [3], 0.0f, -12.0f, 12.0f, "dB");
    addParameter ("Band 4 Frequency", freqs [3], 3300.0f, 220.0f, 10000.0f, "Hz", frequencySkewFactor);
    addParameter ("Band 4 Q Factor", qFactors [3], 0.71f, 0.1f, 10.0f, String::empty, qSkewFactor);
    
    addDBParameter ("Band 5 Gain", gains [4], 0.0f, -12.0f, 12.0f, "dB");
    addParameter ("Band 5 Frequency", freqs [4], 8200.0f, 580.0f, 20000.0f, "Hz", frequencySkewFactor);
    qFactors [4] = 0.71f;
        
    fs = 44100;
    
    numChannels = 0;
}

SafeequaliserAudioProcessor::~SafeequaliserAudioProcessor()
{
}

//==============================================================================
void SafeequaliserAudioProcessor::parameterUpdateCalculations (int index)
{     
    if (index == PARAMgain0 || index == PARAMfreq0)
    {
        updateFilters (0);
    }
    else if (index == PARAMgain1 || index == PARAMfreq1 || index == PARAMqFactor1)
    {
        updateFilters (1);
    }
    else if (index == PARAMgain2 || index == PARAMfreq2 || index == PARAMqFactor2)
    {
        updateFilters (2);
    }
    else if (index == PARAMgain3 || index == PARAMfreq3 || index == PARAMqFactor3)
    {
        updateFilters (3);
    }
    else if (index == PARAMgain4 || index == PARAMfreq4)
    {
        updateFilters (4);
    }
}

void SafeequaliserAudioProcessor::updateFilters (int band)
{     
    IIRCoefficients filterCoefficients;
    
    if (band == 0) // Lowest band (low shelf)
	{
		filterCoefficients = IIRCoefficients::makeLowShelf(fs,
                                                           freqs [0],
                                                           qFactors [0],
                                                           gains [0]);
	}
	else if (band == numFilters - 1) // Highest band (high shelf)
	{
		filterCoefficients = IIRCoefficients::makeHighShelf(fs,
                                                            freqs [numFilters - 1],
                                                            qFactors [numFilters - 1],
                                                            gains [numFilters - 1]);
	}
	else // Other bands (peaking filter)
	{
		filterCoefficients = IIRCoefficients::makePeakFilter(fs,
                                                             freqs [band],
                                                             qFactors [band],
                                                             gains [band]);
	}
	
	for (int channel = 0; channel < numChannels; ++channel)
    {
	    eqFilters [band * numChannels + channel]->setCoefficients (filterCoefficients);
    }
}

//==============================================================================
void SafeequaliserAudioProcessor::pluginPreparation (double sampleRate, int samplesPerBlock)
{
    fs = sampleRate;
    numChannels = getNumInputChannels();
    
    eqFilters.clear();
    for (int band = 0; band < numFilters; ++band)
    {
        for (int channel = 0; channel < numChannels; ++channel)
        {
            eqFilters.add (new IIRFilter);
        }
        
        updateFilters (band);
    }
}

void SafeequaliserAudioProcessor::releaseResources()
{
}

void SafeequaliserAudioProcessor::pluginProcessing (AudioSampleBuffer& buffer)
{    
    int numSamples = buffer.getNumSamples();
    
    for (int channel = 0; channel < numChannels; ++channel)
    {    
        float* channelData = buffer.getWritePointer (channel);
        
        for (int band = 0; band < numFilters; ++band)
        {
            eqFilters [band * numChannels + channel]->processSamples (channelData, numSamples);
        }
    }
}

//==============================================================================
bool SafeequaliserAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SafeequaliserAudioProcessor::createEditor()
{
    return new SafeequaliserAudioProcessorEditor (this);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SafeequaliserAudioProcessor();
}
