#ifndef __FUNDAMENTALTRACKER__
#define __FUNDAMENTALTRACKER__

class FundamentalTracker
{
public:
    //=============================================================================
    //  Constructor and Destructor
    //=============================================================================
    FundamentalTracker();
    ~FundamentalTracker();

    //=============================================================================
    //  Setup Functions
    //=============================================================================
    void setSampleRate (double sampleRate);
    void reset();

    //=============================================================================
    //  Operation
    //=============================================================================
    void addSample (float newSample);
    float getFundamental();

private:
    double fs;
    float signalBuffer [3], f0Buffer [2];
    int signalTap, f0Tap;
    float gamma;
};

#endif // __FUNDAMENTALTRACKER__
