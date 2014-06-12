//=================================================================================
//  Constructor and Destructor
//=================================================================================
AllPassFilter::AllPassFilter()
{
    coefficient = 0.5;
    inTap = 0;
    outTap = 1;
}

AllPassFilter::~AllPassFilter()
{
}

//=================================================================================
//  Set and Get Coefficient
//=================================================================================
void AllPassFilter::setCoefficient (double newCoefficient)
{
    coefficient = (float) newCoefficient;
}

float AllPassFilter::getCoefficient() const
{
    return coefficient;
}

//=================================================================================
//  Clear Sample Buffers
//=================================================================================
void AllPassFilter::reset()
{
    FloatVectorOperations::clear (inSamples, 3);
    FloatVectorOperations::clear (outSamples, 3);
}

//=================================================================================
//  Process Some Audio
//=================================================================================
void AllPassFilter::processSamples (float* samples, int numSamples)
{
    float coeffSquared = coefficient * coefficient;

    for (int i = 0; i < numSamples; ++i)
    {
        inSamples [inTap] = samples [i];
        outSamples [inTap] = coeffSquared * (inSamples [inTap] + outSamples [outTap]) - inSamples [outTap];
        samples [i] = outSamples [inTap];
        
        // deal with those pesky denormals
        if (! (outSamples [inTap] < -1.0e-8 || outSamples [inTap] > 1.0e-8))
            outSamples [inTap] = 0;

        ++inTap;
        ++outTap;
        if (inTap > 2)
            inTap -= 3;
        if (outTap > 2)
            outTap -= 3;
    }
}
