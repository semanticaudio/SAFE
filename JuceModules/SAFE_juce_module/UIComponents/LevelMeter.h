#ifndef __LEVELMETER__
#define __LEVELMETER__

class LevelMeter : public Component,
                   public LevelMeterTimer
{
public:
    //==========================================================================
    //      constructor and destructor
    //==========================================================================
    LevelMeter();
    ~LevelMeter();

    //==========================================================================
    //      Paint
    //==========================================================================
    void paint (Graphics&);

    //==========================================================================
    //      Utility Functions
    //==========================================================================
    float positionToGain (float position);
    float gainToPosition (float gain);

    //==========================================================================
    //      Metering
    //==========================================================================
    void setLevel (float level);
    void meterLevelChanged();

private:
    bool isVertical;
};

#endif // __LEVELMETER__
