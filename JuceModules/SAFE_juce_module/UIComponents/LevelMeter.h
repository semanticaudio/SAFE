#ifndef __LEVELMETER__
#define __LEVELMETER__

/**
 *  A really bare bones level meter. 
 *
 *  Probably not usable unless you really like rectangles which are black and yellow.
 */
class LevelMeter : public Component,
                   public LevelMeterTimer
{
public:
    //==========================================================================
    //      constructor and destructor
    //==========================================================================
    /** Create a new level meter. */
    LevelMeter();

    /** Destructor */
    ~LevelMeter();

    //==========================================================================
    //      Paint
    //==========================================================================
    /** Draw the component. */
    void paint (Graphics&);

    //==========================================================================
    //      Utility Functions
    //==========================================================================
    /** Get the gain which corresponds to a given position along the meter.
     *  
     *  @param position  the position along the meter to convert to a gain
     */
    float positionToGain (float position);

    /** Get the position along the meter which corresponds to a given gain.
     *  
     *  @param gain  the gain value to convert to a position along the meter
     */
    float gainToPosition (float gain);

    //==========================================================================
    //      Metering
    //==========================================================================
    /** Set the meter level.
     *
     *  @param level  the new meter level
     */
    void setLevel (float level);

    /** Callback called whenever the meter updates.
     *
     *  As in many places this class should privately inherit from LevelMeterTimer
     *  I may get round to changing it all at some point. For the mean time just don't
     *  call this function.
     */
    void meterLevelChanged();

private:
    bool isVertical;
};

#endif // __LEVELMETER__
