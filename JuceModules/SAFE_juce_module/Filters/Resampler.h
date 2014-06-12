#ifndef RESAMPLER_H
#define RESAMPLER_H

//==============================================================================
/**
 * This class implements a simple resampling device, returning an audio buffer of  
 * different sampling rate. 
 * Written by Brecht De Man <b.deman@qml.ac.uk> on 26 February 2014. 
 */

class Resampler
{
public:
    //==============================================================================
	
    Resampler(int newResamplingRatio = 1,
              float newCutoffFrequency = 1.0f,
              int newAntiAliasingFilterOrder = 1);
	~Resampler(){};
    
    //==============================================================================
    /** Mutator for private member resamplingRatio
     */
    void setResamplingRatio(int newResamplingRatio);
    
    //==============================================================================
    /** Accessor for private member resamplingRatio
     */
    int  getResamplingRatio();
    
    //==============================================================================
    /** Upsamples the inputBuffer (size numSamples) with ratio resamplingRatio, 
     *  and stores this into outputBuffer (size numSamples times resamplingRatio). 
     */
    void upsample(float* inputBuffer, float* outputBuffer, int numSamples);
    
    //==============================================================================
    /** Downsamples the inputBuffer (size numSamples times resamplingRatio) with ratio 
     *  resamplingRatio, and stores this into outputBuffer (size numSamples).
     */
    void downsample(float* inputBuffer, float* outputBuffer, int numSamples);
    
    //==============================================================================
    /** Sets coefficients for antiAliasingFilter.
     */
    void UpdateAntiAliasingFilters();
	
private:
    //==============================================================================
    
    // parameters
    int _resamplingRatio;   // ratio of up- and downsampling (integer)
    float _cutoffFrequency; // relative, where 1.0f means cutoff at fs/2
    int _antiAliasingFilterOrder;
    
    // anti-aliasing filters (to be replaced with better ones)
    OwnedArray<juce::IIRFilter> _antiAliasingFilterUp;
    OwnedArray<juce::IIRFilter> _antiAliasingFilterDown;
    // replace by modifiable series of biquads?
    
	JUCE_LEAK_DETECTOR (Resampler);
};

#endif   // RESAMPLER_H
