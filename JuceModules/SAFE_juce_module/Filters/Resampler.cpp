Resampler::Resampler(int newResamplingRatio, float newCutoffFrequency, int newAntiAliasingFilterOrder){
    // set member variables
    _resamplingRatio            = newResamplingRatio;
    _cutoffFrequency            = newCutoffFrequency;
    _antiAliasingFilterOrder    = newAntiAliasingFilterOrder;
    
    // clear and allocate storage for filter arrays
    _antiAliasingFilterDown.clear();
    _antiAliasingFilterDown.ensureStorageAllocated(_antiAliasingFilterOrder);
    _antiAliasingFilterUp.clear();
    _antiAliasingFilterUp.ensureStorageAllocated(_antiAliasingFilterOrder);
    
    // add _antiAliasingFilterOrder filters
    for (int filterNr = 0; filterNr < _antiAliasingFilterOrder; filterNr++)
    {
        _antiAliasingFilterDown.add(new juce::IIRFilter);
        _antiAliasingFilterUp.add(new juce::IIRFilter);
    }
    
    UpdateAntiAliasingFilters();
}

void Resampler::setResamplingRatio(int newResamplingRatio){
    _resamplingRatio = newResamplingRatio;
    UpdateAntiAliasingFilters();
}

int Resampler::getResamplingRatio(){
    return _resamplingRatio;
}

void Resampler::upsample(float* inputBuffer,    // low sampling rate signal
                         float* outputBuffer,   // high sampling rate signal
                         int numSamples)        // number of samples in INPUT buffer
{
    // fill outputBuffer with zeroes, except every resamplingRatio samples
    for (int sample = 0; sample < numSamples*_resamplingRatio; sample++)
    {
        if (sample % _resamplingRatio == 0)
        {
            // add gain _resamplingRatio
            // (see http://melodi.ee.washington.edu/courses/ee518/notes/lec9.pdf p. 9-7)
            outputBuffer[sample] = inputBuffer[sample/_resamplingRatio]*_resamplingRatio;
        }
        else
        {
            outputBuffer[sample] = 0.0f; // set sample to zero
        }
    }
    
    // apply high order low pass filter at cutoff (cascade of biquads)
    for (int filterNr = 0; filterNr < _antiAliasingFilterOrder; filterNr++)
    {
        _antiAliasingFilterUp[filterNr]->processSamples(outputBuffer, numSamples*_resamplingRatio);
    }
}

void Resampler::downsample(float* inputBuffer,  // high sampling rate signal
                           float* outputBuffer, // low sampling rate signal
                           int numSamples)      // number of samples in OUTPUT buffer
{
    // apply high order low pass filter at cutoff (cascade of biquads)
    for (int filterNr = 0; filterNr < _antiAliasingFilterOrder; filterNr++)
    {
        _antiAliasingFilterDown[filterNr]->processSamples(inputBuffer, numSamples*_resamplingRatio);
    }
    
    // copy every [resamplingRatio]th input sample to outputBuffer
    for (int sample = 0; sample < numSamples; sample++)
    {
        outputBuffer[sample] = inputBuffer[sample*_resamplingRatio];
    }
}

void Resampler::UpdateAntiAliasingFilters(){
    juce::IIRCoefficients filterCoefficients;
    
    // lowpass at fs_orig/2 (if relative cutoff frequency = 1.0)
    filterCoefficients = juce::IIRCoefficients::makeLowPass(_resamplingRatio, _cutoffFrequency/2.0);
    
    // set these coefficients for every filter (cascade of biquads)
    for (int filterNr = 0; filterNr < _antiAliasingFilterOrder; filterNr++)
    {
        _antiAliasingFilterUp[filterNr]->setCoefficients(filterCoefficients);
        _antiAliasingFilterDown[filterNr]->setCoefficients(filterCoefficients);
    }
}
