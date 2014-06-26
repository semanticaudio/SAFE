/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
SafereverbAudioProcessor::SafereverbAudioProcessor()
    : tempInput (2, 44100),
      tempOutput (2, 44100)
{
    tempInput.clear();
    tempOutput.clear();
    
    addParameter ("Damping Frequency", dampingFreq, 0);
    addParameter ("Density", density, 0.5);
    addParameter ("Bandwidth Frequency", bandwidthFreq, 1.0);
    addParameter ("Decay", decay, 0.5);
    addParameter ("PreDelay", predelay, 0);
    addParameter ("Size", size, 0.5);
    addParameter ("Gain", gain, 1.0);
    addParameter ("Mix", mix, 0.15);
    addParameter ("Early Mix", earlyMix, 0.75);
    
    for (int i = 0; i < 9; ++i)
    {
        parameterUpdateCalculations (i);
    }
}

SafereverbAudioProcessor::~SafereverbAudioProcessor()
{
}

void SafereverbAudioProcessor::parameterUpdateCalculations (int index)
{
    switch(index)
    {
        case MVerb<float>::DAMPINGFREQ:
            reverb.setParameter (index, dampingFreq);
            break;
            
        case MVerb<float>::DENSITY:
            reverb.setParameter (index, density);
            break;

        case MVerb<float>::BANDWIDTHFREQ:
            reverb.setParameter (index, bandwidthFreq);
            break;

        case MVerb<float>::PREDELAY:
            reverb.setParameter (index, predelay);
            break;

        case MVerb<float>::DECAY:
            reverb.setParameter (index, decay);
            break;

        case MVerb<float>::SIZE:
            reverb.setParameter (index, size);
            break;
            
        case MVerb<float>::GAIN:
            reverb.setParameter (index, gain);
            break;

        case MVerb<float>::MIX:
            reverb.setParameter (index, mix);
            break;

        case MVerb<float>::EARLYMIX:
            reverb.setParameter (index, earlyMix);
            break;
    }
}

//==============================================================================
void SafereverbAudioProcessor::pluginPreparation(double sampleRate, int samplesPerBlock)
{
    reverb.reset();
    reverb.setSampleRate (sampleRate);
}

void SafereverbAudioProcessor::releaseResources()
{
}

void SafereverbAudioProcessor::pluginProcessing (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    int numSamples = buffer.getNumSamples();
    int numChannels = buffer.getNumChannels();
    
    tempInput.copyFrom (0, 0, buffer, 0, 0, numSamples);
    
    if (numChannels > 1)
    {
        tempInput.copyFrom (1, 0, buffer, 1, 0, numSamples);
    }
    
    float** input = tempInput.getArrayOfWritePointers();
    float** output = tempOutput.getArrayOfWritePointers();
    
    reverb.process (input, output, numSamples);
    
    buffer.copyFrom (0, 0, output [0], numSamples);
    
    if (numChannels > 1)
    {       
        buffer.copyFrom (1, 0, output [1], numSamples);
    }
}

//==============================================================================
bool SafereverbAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SafereverbAudioProcessor::createEditor()
{
    return new SafereverbAudioProcessorEditor (this);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SafereverbAudioProcessor();
}
