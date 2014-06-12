/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
SafecompressorAudioProcessor::SafecompressorAudioProcessor()
    : inputBuffer (1, 1),
      rmsDetector (1, 4096),
      rmsCounter (0)
{
    numChannels = 0;
    numSamples = 0;
    
    currentInputLevel = -80;
    currentOutputLevel = -80;
    
    alphaAttack = alphaRelease = 0;
    
    inputIndB = compIndB = outputIndB = compIndB_prev = 0;
    
    addParameter ("Threshold", thresholdIndB, 0.0f, -50.0f, 0.0f, "dB");
    addParameter ("Ratio", ratio, 2.0f, 1.0f, 20.0f, ":1", 0.4);
    addParameter ("Knee", kneeIndB, 0.0f, 0.0f, 20.0f, "dB");
    addParameter ("Attack Time", atimeInMs, 5.0f, 0.1f, 100.0f, "ms");
    addParameter ("Release Time", rtimeInMs, 200.0f, 10.0f, 2000.0f, "ms");
    addParameter ("Make Up Gain", makeUpGain, 0.0f, 0.0f, 20.0f, "dB"); 
    
    rmsDetector.clear();
}

SafecompressorAudioProcessor::~SafecompressorAudioProcessor()
{
}

//==============================================================================
void SafecompressorAudioProcessor::pluginPreparation (double sampleRate, int samplesPerBlock)
{
    fs = sampleRate;
    
    controlVoltage.allocate (samplesPerBlock * 4, true);
    
    inputBuffer.setSize (1, samplesPerBlock * 4);
    inputBuffer.clear();
}

void SafecompressorAudioProcessor::releaseResources()
{    
}

void SafecompressorAudioProcessor::pluginProcessing (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    numSamples = buffer.getNumSamples();
    numChannels = buffer.getNumChannels();
       
    inputBuffer.clear();
    
    for (int channel = 0; channel < numChannels; ++channel)
    {
        inputBuffer.addFrom (0, 0, buffer, channel, 0, numSamples, 1.0f / numChannels);
    }
        
    compress (inputBuffer);
    
    // Apply control voltage to all channels
    for (int channel = 0; channel < numChannels; ++channel)
    {
        for (int sample = 0; sample < numSamples; ++sample)
        {
           buffer.getWritePointer (channel) [sample] *= controlVoltage [sample];
        }
    }
}

// Compressor function (loosely based on Reiss & McPherson textbook on digital audio effects)
void SafecompressorAudioProcessor::compress(AudioSampleBuffer &buffer)
{
    
    // Knee coefficients (currently calculated once per buffer!)
    float c0 = -((ratio - 1.0f) * (thresholdIndB * thresholdIndB - kneeIndB * thresholdIndB + kneeIndB * kneeIndB / 4.0f)) / (2.0f * kneeIndB * ratio);
    float c1 = ((ratio - 1) * thresholdIndB + (ratio + 1) * kneeIndB / 2.0f) / (kneeIndB * ratio);
    float c2 = (1 - ratio) / (2.0f * kneeIndB * ratio);
    
    // Ballistics coefficients
	alphaAttack  = exp (-1.0f / (0.001f * fs * atimeInMs));
	alphaRelease = exp (-1.0f / (0.001f * fs * rtimeInMs));
	
	for (int sample = 0 ; sample < numSamples ; ++sample) // sample by sample
	{                
	    rmsDetector.copyFrom (0, rmsCounter, buffer, 0, sample, 1);
	    ++rmsCounter %= 4096;
	    
	    inputIndB = Decibels::gainToDecibels (rmsDetector.getRMSLevel (0, 0, 4096));
	    currentInputLevel = inputIndB;
	    
		// Gain computer- static apply input/output curve
		if (inputIndB >= thresholdIndB + kneeIndB / 2.0f)
            outputIndB = thresholdIndB + (inputIndB - thresholdIndB) / ratio;
		else if (inputIndB <= thresholdIndB - kneeIndB / 2.0f)
            outputIndB = inputIndB;
        else // KNEE (quadratic spline)
        {
            outputIndB = inputIndB * inputIndB * c2 + inputIndB * c1 + c0;
        }
        
		compIndB = inputIndB - outputIndB;
		
		// Ballistics: smoothing of the gain
        // Attack
		if (compIndB > compIndB_prev)
           compIndB = alphaAttack * compIndB_prev + (1.0f - alphaAttack ) * compIndB;
        // Release
		else
           compIndB = alphaRelease * compIndB_prev + (1.0f - alphaRelease) * compIndB;
         
		// Calculate control voltag
		controlVoltage [sample] = Decibels::decibelsToGain (- compIndB + makeUpGain);
		compIndB_prev = compIndB;
		
		currentOutputLevel = inputIndB - compIndB + makeUpGain;
	}
}

float SafecompressorAudioProcessor::getInputLevel()
{
    return currentInputLevel;
}

float SafecompressorAudioProcessor::getOutputLevel()
{
    return currentOutputLevel;
}


//==============================================================================
bool SafecompressorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SafecompressorAudioProcessor::createEditor()
{
    return new SafecompressorAudioProcessorEditor (this);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SafecompressorAudioProcessor();
}
