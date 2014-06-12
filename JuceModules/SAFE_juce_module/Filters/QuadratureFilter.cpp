//=================================================================================
//  Constructor and Destructor
//=================================================================================
QuadratureFilter::QuadratureFilter()
{
    realFilter1.setCoefficient (0.6923878);
    realFilter2.setCoefficient (0.9360654322959);
    realFilter3.setCoefficient (0.9882295226860);
    realFilter4.setCoefficient (0.9987488452737);

    imagFilter1.setCoefficient (0.4021921162426);
    imagFilter2.setCoefficient (0.8561710882420);
    imagFilter3.setCoefficient (0.9722909545651);
    imagFilter4.setCoefficient (0.9952884791278);

    delayCounter = 0;
}

QuadratureFilter::~QuadratureFilter()
{
}

//=================================================================================
//  Clear Sample Buffers
//=================================================================================
void QuadratureFilter::reset()
{
    realFilter1.reset();
    realFilter2.reset();
    realFilter3.reset();
    realFilter4.reset();

    imagFilter1.reset();
    imagFilter2.reset();
    imagFilter3.reset();
    imagFilter4.reset();
    
    FloatVectorOperations::clear (delaySamples, 2);
}

//=================================================================================
//  Process Some Audio
//=================================================================================
void QuadratureFilter::processSamples (const float* input, float* realOutput, float* imagOutput, int numSamples)
{
    FloatVectorOperations::copy (realOutput, input, numSamples);
    FloatVectorOperations::copy (imagOutput, input, numSamples);

    realFilter1.processSamples (realOutput, numSamples);
    realFilter2.processSamples (realOutput, numSamples);
    realFilter3.processSamples (realOutput, numSamples);
    realFilter4.processSamples (realOutput, numSamples);

    delaySamples [delayCounter] = realOutput [numSamples - 1];
    for (int sampleNumber = numSamples - 1; sampleNumber > 0; --sampleNumber)
    {
        realOutput [sampleNumber] = realOutput [sampleNumber - 1];
    }
    delayCounter = 1 - delayCounter;
    realOutput [0] = delaySamples [delayCounter];

    imagFilter1.processSamples (imagOutput, numSamples);
    imagFilter2.processSamples (imagOutput, numSamples);
    imagFilter3.processSamples (imagOutput, numSamples);
    imagFilter4.processSamples (imagOutput, numSamples);
    FloatVectorOperations::multiply (imagOutput, -1, numSamples);
}
