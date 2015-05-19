#ifndef __LEVELMETERTIMER__
#define __LEVELMETERTIMER__

/**
 *  A base class for metering components.
 *
 *  This class provides functionality for graphical meters with configurable
 *  rise and decay time.
 */

class LevelMeterTimer : public MultiTimer
{
public:
    //==========================================================================
    //      Constructor and Destructor
    //==========================================================================
    /** Create a new level meter timer.
     *
     *  @param numMeters  the number of separate meters to keep track of
     */
    LevelMeterTimer (int numMeters);

    /** Destructor */
    virtual ~LevelMeterTimer();

    //==========================================================================
    //      Get and Set Level
    //==========================================================================
    /** Set the level the meter should display.
     *
     *  @param index     the index of the meter to be set - this should be
     *                   between 0 and (numMeters - 1)
     *  @param newLevel  the level to display
     */
    void setMeterLevel (int index, double newLevel);

    /** Return the value of the meter taking into account rise and decay time.
     *
     *  @param index  the index of the meter to get the value of - this should be
     *                between 0 and (numMeters - 1)
     */
    double getMeterLevel (int index);

    /** Return the value the meter is heading towards (the value set with setMeterLevel()).
     *
     *  @param index  the index of the meter to get the value of - this should be
     *                between 0 and (numMeters - 1)
     */
    double getMeterTargetLevel (int index);

    //==========================================================================
    //      Set Rise and Decay Times
    //==========================================================================
    /** Set the time it takes for the meter to rise to its new value
     *
     *  @param riseInMilliseconds  the new rise time in milliseconds
     */
    void setMeterRiseTime (int riseInMilliseconds);
    
    /** Set the time it takes for the meter to decay to its new value
     *
     *  @param decayInMilliseconds  the new decay time in milliseconds
     */
    void setMeterDecayTime (int decayInMilliseconds);

    //==========================================================================
    //      Timer Callback
    //==========================================================================
    /** The timer callback which updates the meter levels.
     *
     *  This function should not be called. This class really should privately
     *  inherit from MultiTimer but I haven't got round to changing that yet.
     */
    void timerCallback (int index);

    /** A callback function that is called whenever the value of the meter has been updated.
     *
     *  Classes which inherit from LevelMeterTimer should implement this to
     *  update their graphics when the meter changes level.
     */
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
