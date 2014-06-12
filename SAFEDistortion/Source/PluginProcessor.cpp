/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
SafedistortionAudioProcessor::SafedistortionAudioProcessor()
    : freqDCBlocking (20.0f),
      resamplingRatio (4),
      antiAliasingCutoff (0.99f),
      antiAliasingOrder (8)
{
    fs = 44100;
    
    addDBParameter ("Input Gain", inputGain, 0.0f, -50.0f, 50.0f, "dB");
    addParameter ("Knee", knee, 0.0f, 0.0f, 1.0f);
    addParameter ("Bias", bias, 0.0f, 0.0f, 1.0f);
    addParameter ("Tone", tone, 1.0f, 0.0f, 1.0f);
    addDBParameter ("Output Gain", outputGain, 0.0f, -60.0f, 6.0f, "dB");
    
    numChannels = 0;
}

SafedistortionAudioProcessor::~SafedistortionAudioProcessor()
{
}

void SafedistortionAudioProcessor::parameterUpdateCalculations (int index)
{
    if (index == PARAMtone)
    {
        // Frequency bounds of second order low-pass filter
        float lowFreq   = 200.0f;   // Hz
        float highFreq  = 20000.0f; // Hz
    
        // Calculate coefficients based on parameters
        IIRCoefficients filterCoefficients;
        filterCoefficients = IIRCoefficients::makeLowPass(fs,
                                                      lowFreq + (powf(10.0f, tone)-1.0f)*(highFreq-lowFreq)/9.0f);
                                                      
        for (int channel = 0; channel < numChannels; ++channel)
        {
            toneFilters [channel]->setCoefficients(filterCoefficients);  
        }
    }
}

//==============================================================================
void SafedistortionAudioProcessor::pluginPreparation (double sampleRate, int samplesPerBlock)
{
    fs = sampleRate;
    numChannels = getNumInputChannels();
    
    toneFilters.clear();
    dcBlockingFilters.clear();
    resamplers.clear();
    for (int channel = 0; channel < numChannels; ++channel)
    {
        toneFilters.add (new IIRFilter);
        toneFilters [channel]->reset();
        
        dcBlockingFilters.add (new IIRFilter);
        dcBlockingFilters [channel]->reset();
        
        resamplers.add (new Resampler (resamplingRatio, antiAliasingCutoff, antiAliasingOrder));
    }
    
    upsampledData.allocate (samplesPerBlock * 4 * resamplingRatio, true);
    
    // Set up tone filter
    parameterUpdateCalculations (PARAMtone);
    
    // DC blocking filter (static)
    IIRCoefficients filterCoefficients;
    filterCoefficients = IIRCoefficients::makeHighPass(sampleRate,
                                                       freqDCBlocking);
    for (int channel = 0; channel < numChannels; ++channel)
    {
        dcBlockingFilters [channel]->setCoefficients(filterCoefficients);
    }
}

void SafedistortionAudioProcessor::releaseResources()
{
}

void SafedistortionAudioProcessor::pluginProcessing (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    int numSamples = buffer.getNumSamples();
    
    // Auxiliary variables
    float oneMinusBias = 1.0f - bias;
    float oneMinusKnee = 1.0f - knee;
    float onePlusKnee  = 1.0f + knee;
    
    // Knee parameters (calculate for speed)
    float c2 = - outputGain / (4.0f * knee);
    float c1 = outputGain * onePlusKnee / (2.0f * knee);
    float c0 = - outputGain * oneMinusKnee * oneMinusKnee / (4.0f * knee);   
    
    for (int channel = 0; channel < numChannels; ++channel)
    {
        // Processing
        float* originalData = buffer.getWritePointer (channel);
        
        // Tone filter (LPF, before non-linearity?)
        toneFilters [channel]->processSamples(originalData, numSamples);
        
        // Upsampling (from originalData to upsampledData)
        resamplers [channel]->upsample (originalData, upsampledData, numSamples);
        
        // Apply distortion
        for (int sample = 0; sample < numSamples * resamplingRatio; ++sample)
        {
            upsampledData[sample] *= inputGain; // apply gain
            
            // Soft clipping
            if(upsampledData[sample]  > oneMinusKnee)
                if(upsampledData[sample] >= onePlusKnee) // positive clipping
                    upsampledData[sample] = outputGain;
                else // soft knee (positive)
                {
                    upsampledData[sample] = c2*upsampledData[sample]*upsampledData[sample] + c1*upsampledData[sample] + c0;
                }
            else
            {
                if(upsampledData[sample]  < - oneMinusBias*oneMinusKnee)
                {
                    if(upsampledData[sample] <= - oneMinusBias*onePlusKnee) // negative clipping
                        upsampledData[sample] = -oneMinusBias*outputGain;
                    else // soft knee (negative)
                    {
                        upsampledData[sample] = -c2*upsampledData[sample]*upsampledData[sample]/oneMinusBias + c1*upsampledData[sample] - c0*oneMinusBias;
                    }
                }
                else // linear region
                {
                    upsampledData[sample] *= outputGain;
                }
            }
        }
    
        // Downsampling (from upsampledData to originalData)
        resamplers [channel]->downsample (upsampledData, originalData, numSamples);
        
        // DC blocking filter
        dcBlockingFilters [channel]->processSamples(originalData, numSamples);   
    }   
}

//==============================================================================
bool SafedistortionAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SafedistortionAudioProcessor::createEditor()
{
    return new SafedistortionAudioProcessorEditor (this);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SafedistortionAudioProcessor();
}
