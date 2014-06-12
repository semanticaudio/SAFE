#ifndef __LEVELMETERTIMER__
#define __LEVELMETERTIMER__

class LevelMeterTimer : public MultiTimer
{
public:
    //==========================================================================
    //      Constructor and Destructor
    //==========================================================================
    LevelMeterTimer (int numMeters);
    virtual ~LevelMeterTimer();

    //==========================================================================
    //      Get and Set Level
    //==========================================================================
    void setMeterLevel (int index, double newLevel);
    double getMeterLevel (int index);
    double getMeterTargetLevel (int index);

    //==========================================================================
    //      Set Rise and Decay Times
    //==========================================================================
    void setMeterRiseTime (int riseInMilliseconds);
    void setMeterDecayTime (int decayInMilliseconds);

    //==========================================================================
    //      Timer Callback
    //==========================================================================
    void timerCallback (int index);
    virtual void meterLevelChanged() = 0;

private:
    Array <double> targetLevels, currentLevels, previousLevels;

    int riseTime, decayTime;
    int numRiseSteps, numDecaySteps, numSteps;
    Array <double> stepSizes;

    Array <int> timerCounters;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LevelMeterTimer)
};

#endif // __LEVELMETERTIMER__
