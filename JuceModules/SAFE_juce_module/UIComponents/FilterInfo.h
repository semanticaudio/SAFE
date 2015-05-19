#ifndef __FILTERINFO__
#define __FILTERINFO__

/**
 *  A small class to hold the response of a filter at a given frequency.
 */
class FilterResponse
{
public:
    //==========================================================================
    //      Constructor and Destructor
    //==========================================================================
    /** Create a new filter response. 
     *
     *  @param magnitudeInit  the initial value for the magnitude
     *  @param phaseInit      the initial value for the phase
     */
    FilterResponse (double magnitudeInit, double phaseInit);
    
    /** Destructor */
    ~FilterResponse();
    
    double magnitudeValue, phaseValue;
};

/** 
 *  A class to hold information about a digital filter.
 */
class FilterInfo
{
public:    
    //==========================================================================
    //      Constructor and Destructor
    //==========================================================================
    /** Create a new filter info object. */
    FilterInfo();

    /** Destructor */
    ~FilterInfo();
    
    //==========================================================================
    //      Set Filter Stuff
    //==========================================================================
    /** Set the sample rate the filter was designed to run at.
     *
     *  @param sampleRate  the new sample rate
     */
    void setSampleRate (double sampleRate);

    /** Set the gain to be applied to the filter response.
     *
     *  @param newGain  the new gain value
     */
    void setGain (double newGain);
     
    /** Reset the filter coefficients.
     *
     *  This function resets the filter coefficients to a filter which lets
     *  audio pass unaltered.
     */
    void resetCoefficients();

    /** Set the filter coefficients.
     *
     *  @param newCoeffs  the new filtrer coefficients
     */
    void setCoefficients (IIRCoefficients newCoeffs);

    /** Set the filter coefficients.
     *
     *  @param newNumeratorCoeffs    the new numerator coefficients
     *  @param newDenominatorCoeffs  the new denominator coefficients
     */
    void setCoefficients (Array <double> newNumeratorCoeffs, Array <double> newDenominatorCoeffs);

    //==========================================================================
    //      Get Response
    //==========================================================================
    /** Get the filter's response to a certain frequency.
     *
     *  @param inputFrequency  the frequency to get the response of the filter at
     */
    FilterResponse getResponse (double inputFrequency) const;
    
private:
    double fs;
    int numNumeratorCoeffs, numDenominatorCoeffs;

    Array <double> numeratorCoeffs;
    Array <double> denominatorCoeffs;
    double gain;
};

#endif // __FILTERINFO__
