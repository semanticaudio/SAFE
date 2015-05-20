#ifndef __FUNDAMENTALTRACKER__
#define __FUNDAMENTALTRACKER__

/**
 *  A lightweight time domain frequency tracker.
 *
 *  An implementation of the frequency tracker given in
 *  [<em> Audio Bandwidth Extension </em>]
 *  (http://eu.wiley.com/WileyCDA/WileyTitle/productCd-0470858648.html)
 *  (Larsen, E. and Aarts, R. 2004)
 *  With a gamma value of 0.002.
 */
class FundamentalTracker
{
public:
    //=============================================================================
    //  Constructor and Destructor
    //=============================================================================
    /** Create a new fundamental tracker. */
    FundamentalTracker();

    /** Destructor */
    ~FundamentalTracker();

    //=============================================================================
    //  Setup Functions
    //=============================================================================
    /** Set the sample rate of the singal being analysed.
     *
     *  @param sampleRate  the new sample rate
     */
    void setSampleRate (double sampleRate);

    /** Reset the tracker's internal buffers to 0. */
    void reset();

    //=============================================================================
    //  Operation
    //=============================================================================
    /** Add the next sample of the signal being analysed.
     *
     *  Using this function individual samples of the input are added one at a time.
     *  I should get around to putting in some functions where you can add arrays 
     *  of samples but I haven't yet.
     */
    void addSample (float newSample);

    /** Returns an approximation of fundamental frequency of the input signal. */
    float getFundamental();

private:
    double fs;
    float signalBuffer [3], f0Buffer [2];
    int signalTap, f0Tap;
    float gamma;
};

#endif // __FUNDAMENTALTRACKER__
