//=================================================================================
//  A quadrature filter based of one described by Olli Niemitalo
//  http://yehar.com/blog/?p=368
//=================================================================================

#ifndef __QUADRATUREFILTER__
#define __QUADRATUREFILTER__

class QuadratureFilter
{
public:
    //=============================================================================
    //  Constructor and Destructor
    //=============================================================================
    QuadratureFilter();
    ~QuadratureFilter();

    //=============================================================================
    //  Clear Sample Buffers
    //=============================================================================
    void reset();

    //=============================================================================
    //  Process Some Audio
    //=============================================================================
    void processSamples (const float* input, float* realOutput, float* imagOutput, int numSamples);

private:
    //=============================================================================
    //  Filters
    //=============================================================================
    AllPassFilter realFilter1, realFilter2, realFilter3, realFilter4, imagFilter1, imagFilter2, imagFilter3, imagFilter4;
    
    //=============================================================================
    //  Delay Bits
    //=============================================================================
    float delaySamples [2];
    int delayCounter;
};

#endif // __QUADRATUREFILTER__
