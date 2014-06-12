/* Extension of the JUCE IIRFilters, but with an arbitrary number of coefficients.
 *
 * by Brecht De Man on 1 March 2014
 */

#ifndef BrechtsIIRFilter_H
#define BrechtsIIRFilter_H

//==============================================================================
/**
    An IIR filter class for high-order filters. 
*/
class BrechtsIIRFilter
{
public:
    //==============================================================================
    /** Creates a filter.

        Initially the filter is inactive, so will have no effect on samples that
        you process with it. Use the setCoefficients() method to turn it into the
        type of filter needed.
    */
    BrechtsIIRFilter(int filterOrder = 1) noexcept;

    /** Creates a copy of another filter. */
    BrechtsIIRFilter (const BrechtsIIRFilter&) noexcept;

    /** Destructor. */
    ~BrechtsIIRFilter() noexcept;

    //==============================================================================
    /** Sets the filter to active (default if coefficients are set). */
    void makeActive() noexcept;
    
    //==============================================================================
    /** Clears the filter so that any incoming data passes through unchanged. */
    void makeInactive() noexcept;
    
    //==============================================================================
    /** Toggles the filter's activity. */
    void toggleActivity() noexcept;
    
    /** Returns the coefficients that this filter is using. */
    float * getCoefficients() const noexcept    { return _coefficients; }

    /** Applies a set of coefficients to this filter. */
    void setCoefficients (float * coefficients, int filterOrder) noexcept;

    //==============================================================================
    /** Resets the filter's processing pipeline, ready to start a new stream of data.

        Note that this clears the processing state, but the type of filter and
        its coefficients aren't changed. To put a filter into an inactive state, use
        the makeInactive() method.
    */
    void reset() noexcept;

    /** Performs the filter operation on one sample. */
    float processSingleSampleRaw (const float inputSample) noexcept;
    
    /** Performs the filter operation on the given set of samples. */
    void processSamples (float* samples, int numSamples) noexcept;
    
    // make butterworth LPF/HPF/BPF, shelves, peaking
    // make chebychev
    // make elliptical
    
protected:
    //==============================================================================
    // SpinLock processLock; //?
    int _filterOrder;
    float * _coefficients;
    float * _memory; // memory elements
    bool _active;

    BrechtsIIRFilter& operator= (const BrechtsIIRFilter&);
};


#endif   // BrechtsIIRFilter_H
