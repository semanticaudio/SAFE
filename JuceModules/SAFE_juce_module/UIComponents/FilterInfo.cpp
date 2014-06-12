#ifndef double_E
#define double_E 2.71828183
#endif

//==========================================================================
//      Constructor and Destructor
//==========================================================================
FilterResponse::FilterResponse (double magnitudeInit, double phaseInit)
    : magnitudeValue (magnitudeInit),
      phaseValue (phaseInit)
{
}

FilterResponse::~FilterResponse()
{
}

//==========================================================================
//      Constructor and Destructor
//==========================================================================
FilterInfo::FilterInfo()
    : fs (44100),
      gain (1)
{
    resetCoefficients();
}

FilterInfo::~FilterInfo()
{
}

//==========================================================================
//      Set Filter Stuff
//==========================================================================
void FilterInfo::setSampleRate (double sampleRate)
{
    fs = sampleRate;
}

void FilterInfo::setGain (double newGain)
{
    gain = newGain;
}

void FilterInfo::resetCoefficients()
{
    numNumeratorCoeffs = 1;
    numDenominatorCoeffs = 1;

    numeratorCoeffs.resize (1);
    denominatorCoeffs.resize (1);
    
    numeratorCoeffs.set (0, 1);
    denominatorCoeffs.set (0, 1);
}

void FilterInfo::setCoefficients (IIRCoefficients newCoeffs)
{
    numNumeratorCoeffs = 3;
    numDenominatorCoeffs = 3;
    
    numeratorCoeffs.resize (3);
    denominatorCoeffs.resize (3);

    for (int numOrder = 0; numOrder < numNumeratorCoeffs; numOrder++)
    {
        numeratorCoeffs.set (numOrder, newCoeffs.coefficients [numOrder]);
    }
    
    for (int denOrder = 1; denOrder < numDenominatorCoeffs; denOrder++)
    {
        denominatorCoeffs.set (denOrder, newCoeffs.coefficients [denOrder + 2]);
    }
    
    denominatorCoeffs.set (0, 1);
}

void FilterInfo::setCoefficients (Array <double> numCoeffs, Array <double> denCoeffs)
{
    numNumeratorCoeffs = numCoeffs.size();
    numDenominatorCoeffs = denCoeffs.size();
    
    numeratorCoeffs = numCoeffs;
    denominatorCoeffs = denCoeffs;
}

//==========================================================================
//      Get Response
//==========================================================================
FilterResponse FilterInfo::getResponse (double inputFrequency) const
{
    std::complex <double> normalisedFrequency (0, (2 * double_Pi * inputFrequency / fs));
    std::complex <double> z = pow (double_E, normalisedFrequency);
    
    std::complex <double> num (0, 0);
    std::complex <double> den (0, 0);
    
    for (int numOrder = 0; numOrder < numNumeratorCoeffs; numOrder++)
    {
        num += numeratorCoeffs [numOrder] / pow (z, numOrder);
    }
    
    for (int denOrder = 0; denOrder < numDenominatorCoeffs; denOrder++)
    {
        den += denominatorCoeffs [denOrder] / pow (z, denOrder);
    }
    
    std::complex <double> transferFunction = num / den;
    
    return FilterResponse (abs (transferFunction) * gain, arg (transferFunction));
}
