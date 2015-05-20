//=================================================================================
//  A quadrature filter based of one described by Olli Niemitalo
//  http://yehar.com/blog/?p=368
//=================================================================================

#ifndef __QUADRATUREFILTER__
#define __QUADRATUREFILTER__

/** A quadrature filter pair.
 *
 *  An implementation of the quadrature pair filter described [here]
 *  (http://yehar.com/blog/?p=368).
 *
 *  Processes the input to create two output signals with
 *  the same magnitude spectrum as the input but phase spectra
 *  which differ by \f$ \frac{\pi}{2} \f$ radians for a large portion 
 *  of the frequency range.
 *
 *  Similar to Max/MSP's [hilbert~]
 *  (https://docs.cycling74.com/max5/refpages/msp-ref/hilbert~.html)
 *  object.
 */
class QuadratureFilter
{
public:
    //=============================================================================
    //  Constructor and Destructor
    //=============================================================================
    /** Create a new quadrature filter. */
    QuadratureFilter();

    /** Destructor */
    ~QuadratureFilter();

    //=============================================================================
    //  Clear Sample Buffers
    //=============================================================================
    /** Reset the filter's internal buffers to 0. */
    void reset();

    //=============================================================================
    //  Process Some Audio
    //=============================================================================
    /** Process a buffer of audio samples.
     *
     *  @param intput      a pointer to the array of samples to process
     *  @param realOutput  a pointer to an array which will contain
     *                     the real part of the ouput
     *  @param imagOutput  a pointer to an array which will contain
     *                     the real part of the ouput
     *  @param numSamples  the number of samples to process - the arrays passed 
     *                     to the first three parameters should all contain
     *                     at least this man samples
     */
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
