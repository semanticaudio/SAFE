/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <complex>


//==============================================================================
SafeharmonicsAudioProcessor::SafeharmonicsAudioProcessor()
    : method (IAP),
      fundamental (0),
      smoothedFundamental (0),
      analysisTap (0),
      realSignal (1, 1),
      imagSignal (1, 1),
      f1 (1, 1),
      f2 (1, 1),
      f3 (1, 1),
      f4 (1, 1),
      f5 (1, 1),
      numInputs (1),
      fs (44100)      
{  
    analysisBuffer.allocate (4096, true);
    
    xtract_init_wavelet_f0_state();
    
    f0Smoother.setCoefficients (IIRCoefficients::makeLowPass (44100, 1000));
    amplitudeSmoother.setCoefficients (IIRCoefficients::makeLowPass (44100, 1000));
    
    addDBParameter ("F0", f0Amplitude, 0, -100, 12);
    addDBParameter ("F1", f1Amplitude, -100, -100, 12);
    addDBParameter ("F2", f2Amplitude, -100, -100, 12);
    addDBParameter ("F3", f3Amplitude, -100, -100, 12);
    addDBParameter ("F4", f4Amplitude, -100, -100, 12);
    addDBParameter ("F5", f5Amplitude, -100, -100, 12);
    addDBParameter ("Gain", gain, 0, -20, 20);
    addParameter ("Method", methodParam);
}

SafeharmonicsAudioProcessor::~SafeharmonicsAudioProcessor()
{
}

void SafeharmonicsAudioProcessor::parameterUpdateCalculations (int index)
{
    if (index == Method)
    {
        if (methodParam < 0.5)
        {
            method = SSB;
        }
        else
        {
            method = IAP;
        }
    }
}

//==============================================================================
void SafeharmonicsAudioProcessor::pluginPreparation (double sampleRate, int samplesPerBlock)
{
    fs = sampleRate;
    
    f0Smoother.reset();
    amplitudeSmoother.reset();
    
    f0Filters.clear();
    quadFilters.clear();
    
    numInputs = getNumInputChannels();
    
    for (int channel = 0; channel < numInputs; ++channel)
    {
        f0Filters.add (new IIRFilter);
        f0Filters [channel]->reset();
        
        quadFilters.add (new QuadratureFilter);
        quadFilters [channel]->reset();
    }
    
    realSignal.setSize (numInputs, samplesPerBlock * 4);
    imagSignal.setSize (numInputs, samplesPerBlock * 4);
    f1.setSize (numInputs, samplesPerBlock * 4);
    f2.setSize (numInputs, samplesPerBlock * 4);
    f3.setSize (numInputs, samplesPerBlock * 4);
    f4.setSize (numInputs, samplesPerBlock * 4);
    f5.setSize (numInputs, samplesPerBlock * 4);
}

void SafeharmonicsAudioProcessor::releaseResources()
{
}

void SafeharmonicsAudioProcessor::pluginProcessing (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    int numSamples = buffer.getNumSamples();
    
    const float* channelData = buffer.getReadPointer (0);
    
    for (int i = 0; i < numSamples; ++i)
    {
        analysisBuffer [analysisTap] = channelData [i];
        ++analysisTap %= 4096;
        
        if (analysisTap == 0)
        {
            xtract_wavelet_f0 (analysisBuffer, 4096, &fs, &fundamental);
            smoothedFundamental = f0Smoother.processSingleSampleRaw (fundamental);
            
            IIRCoefficients coeffs = IIRCoefficients::makeLowPass (fs, smoothedFundamental);
            
            for (int channel = 0; channel < numInputs; ++channel)
            {
                f0Filters [channel]->setCoefficients (coeffs);
            }
        }
        
        for (int channel = 0; channel < numInputs; ++channel)
        {
            f0Filters [channel]->processSamples (buffer.getWritePointer (channel) + i, 1);
        }
    }
    
    for (int channel = 0; channel < numInputs; ++channel)
    {
        const float* inputSamples = buffer.getReadPointer (channel);
        float* realSamples = realSignal.getWritePointer (channel);
        float* imagSamples = imagSignal.getWritePointer (channel);
        float* f1Samples = f1.getWritePointer (channel);
        float* f2Samples = f2.getWritePointer (channel);
        float* f3Samples = f3.getWritePointer (channel);
        float* f4Samples = f4.getWritePointer (channel);
        float* f5Samples = f5.getWritePointer (channel);
        
        quadFilters [channel]->processSamples (inputSamples, realSamples, imagSamples, numSamples);
        
        for (int i = 0; i < numSamples; ++i)
        {
            std::complex <float> currentComplexSample (realSamples [i], imagSamples [i]);
            
            switch (method)
            {
                case SSB:
                    f1Samples [i] = real (pow (currentComplexSample, 2)); 
                    f2Samples [i] = real (pow (currentComplexSample, 3));
                    f3Samples [i] = real (pow (currentComplexSample, 4));
                    f4Samples [i] = real (pow (currentComplexSample, 5));
                    f5Samples [i] = real (pow (currentComplexSample, 6));
                    break;
                    
                case IAP:
                    float phase = arg (currentComplexSample);
                    float amplitude = abs (currentComplexSample);
                    f1Samples [i] = amplitude * cos (2 * phase);
                    f2Samples [i] = amplitude * cos (3 * phase);
                    f3Samples [i] = amplitude * cos (4 * phase);
                    f4Samples [i] = amplitude * cos (5 * phase);
                    f5Samples [i] = amplitude * cos (6 * phase);
                    break;
            }
        }
        
        buffer.applyGain (channel, 0, numSamples, f0Amplitude);
        buffer.addFrom (channel, 0, f1, channel, 0, numSamples, f1Amplitude);
        buffer.addFrom (channel, 0, f2, channel, 0, numSamples, f2Amplitude);
        buffer.addFrom (channel, 0, f3, channel, 0, numSamples, f3Amplitude);
        buffer.addFrom (channel, 0, f4, channel, 0, numSamples, f4Amplitude);
        buffer.addFrom (channel, 0, f5, channel, 0, numSamples, f5Amplitude);
    }
    
    buffer.applyGain (gain);
}

//==============================================================================
bool SafeharmonicsAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SafeharmonicsAudioProcessor::createEditor()
{
    return new SafeharmonicsAudioProcessorEditor (this);
}

double SafeharmonicsAudioProcessor::getFundamental()
{
    return smoothedFundamental;
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SafeharmonicsAudioProcessor();
}
