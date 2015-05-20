#ifndef __ALLPASSFILTER__
#define __ALLPASSFILTER__

/** An IIR allpass filter
 *
 *  An implementation of the allpass filter described [here]
 *  (http://yehar.com/blog/?p=368).
 */
class AllPassFilter
{
public:
    //=============================================================================
    //  Constructor and Destructor
    //=============================================================================
    /** Create a new allpass filter. */
	AllPassFilter();

    /** Destructor */
	~AllPassFilter();

    //=============================================================================
    //  Set and Get Coefficient
    //=============================================================================
    /** Set the filter coefficient.
     *
     *  @param newCoefficient  the new filter coefficient
     */
    void setCoefficient (double newCoefficient);

    /** Returns the current filter coefficient. */
    float getCoefficient() const;

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
     *  @param samples     a pointer to the array of samples to process
     *  @param numSamples  the number of samples to be processed
     */
	void processSamples (float* samples, int numSamples);

protected:
    //SpinLock processLock;

    //=============================================================================
    //  Filter Bits
    //=============================================================================
    float coefficient;
    float inSamples [3], outSamples [3];
    int inTap, outTap;
};

#endif // __ALLPASSFILTER__
