#ifndef __FILTERINFO__
#define __FILTERINFO__

class FilterResponse
{
public:
    //==========================================================================
    //      Constructor and Destructor
    //==========================================================================
    FilterResponse (double magnitudeInit, double phaseInit);
    ~FilterResponse();
    
    double magnitudeValue, phaseValue;
};

class FilterInfo
{
public:    
    //==========================================================================
    //      Constructor and Destructor
    //==========================================================================
    FilterInfo();
    ~FilterInfo();
    
    //==========================================================================
    //      Set Filter Stuff
    //==========================================================================
    void setSampleRate (double sampleRate);
    void setGain (double newGain);
    
    void resetCoefficients();
    void setCoefficients (IIRCoefficients newCoeffs);
    void setCoefficients (Array <double> newNumeratorCoeffs, Array <double> newDenominatorCoeffs);

    //==========================================================================
    //      Get Response
    //==========================================================================
    FilterResponse getResponse (double inputFrequency) const;
    
private:
    double fs;
    int numNumeratorCoeffs, numDenominatorCoeffs;

    Array <double> numeratorCoeffs;
    Array <double> denominatorCoeffs;
    double gain;
};

#endif // __FILTERINFO__
