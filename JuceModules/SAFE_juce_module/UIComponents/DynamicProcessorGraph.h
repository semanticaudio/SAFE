#ifndef __DYNAMICPROCESSORGRAPH__
#define __DYNAMICPROCESSORGRAPH__

/**
 *  A component for displaying the activity of a dynamics processor.
 */
class DynamicProcessorGraph : public FunctionGraph,
                              public LevelMeterTimer
{
public:
    //==========================================================================
    //      Constructor and Destructor
    //==========================================================================
    /** Create a new dynamic processor graph. */
    DynamicProcessorGraph();

    /** Destructor */
    ~DynamicProcessorGraph();

    //==========================================================================
    //      Paint
    //==========================================================================
    /** Draw the component. */
    void paint (Graphics&);

    /** Draw the component's border. */
    void paintOverChildren (Graphics&);

    /** Function called when the size of the graph is changed. */
    void resized();

    //==========================================================================
    //      Metering
    //==========================================================================
    /** Set the position of the dot on the graph.
     *
     *  @param inputLevel   the input level to display
     *  @param outputLevel  the output level to display
     */
    void setMeterLevels (float inputLevel, float outputLevel);

    /** Callback function which is called when the meter updates. */
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
