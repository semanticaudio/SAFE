#ifndef __DYNAMICPROCESSORGRAPH__
#define __DYNAMICPROCESSORGRAPH__
class DynamicProcessorGraph : public FunctionGraph,
                              public LevelMeterTimer
{
public:
    //==========================================================================
    //      Constructor and Destructor
    //==========================================================================
    DynamicProcessorGraph();
    ~DynamicProcessorGraph();

    //==========================================================================
    //      Paint
    //==========================================================================
    void paint (Graphics&);
    void paintOverChildren (Graphics&);
    void resized();

    //==========================================================================
    //      Metering
    //==========================================================================
    void setMeterLevels (float inputLevel, float outputLevel);
    void meterLevelChanged();

private:
    //==========================================================================
    //      Bounding Component for Meter Dot
    //==========================================================================
    class DynamicsMeter : public Component
    {
    public:
        //==========================================================================
        //      Constructor and Destructor
        //==========================================================================
        DynamicsMeter();
        ~DynamicsMeter();

        //==========================================================================
        //      Set Dot Position
        //==========================================================================
        void setDotPosition (int xPos, int yPos);

    private:
        //==========================================================================
        //      Dot Component
        //==========================================================================
        class Dot : public Component
        {
        public:
            //==========================================================================
            //      Constructor and Destructor
            //==========================================================================
            Dot();
            ~Dot();

            //==========================================================================
            //      Paint
            //==========================================================================
            void paint (Graphics& g);

            Colour colour;
        };

        Dot dot;
    };

    DynamicsMeter meterArea;

    int xPos, yPos;

    Path linePath;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DynamicProcessorGraph)
};

#endif // __DYNAMICPROCESSORGRAPH__
