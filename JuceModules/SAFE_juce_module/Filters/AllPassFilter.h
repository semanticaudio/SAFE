#ifndef __ALLPASSFILTER__
#define __ALLPASSFILTER__

class AllPassFilter
{
public:
    //=============================================================================
    //  Constructor and Destructor
    //=============================================================================
	AllPassFilter();
	~AllPassFilter();

    //=============================================================================
    //  Set and Get Coefficient
    //=============================================================================
    void setCoefficient (double newCoefficient);
    float getCoefficient() const;

    //=============================================================================
    //  Clear Sample Buffers
    //=============================================================================
	void reset();

    //=============================================================================
    //  Process Some Audio
    //=============================================================================
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
