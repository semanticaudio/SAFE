#ifndef __FUNCTIONGRAPH__
#define __FUNCTIONGRAPH__

class FunctionGraph : public Component
{
public:
    //==========================================================================
    //      Constructor and Destructor
    //==========================================================================
    FunctionGraph();
    ~FunctionGraph();

    //==========================================================================
    //      Paint
    //==========================================================================
    void paint (Graphics&);
    void resized();

    //==========================================================================
    //      Change Appearance
    //==========================================================================
    void setLineSeparation (float newValue);
    void setXAxisRange (float newLowX, float newHighX);
    void setYAxisRange (float newLowY, float newHighY);
    void setTraceColour (Colour newColour);
    void setBackgroundColour (Colour newColour);

    //==========================================================================
    //      Utility Functions
    //==========================================================================
    float xToAxisValue (float xPos);
    float axisValueToX (float axisValue);

    float yToAxisValue (float yPos);
    float axisValueToY (float axisValue);

    //==========================================================================
    //      Function to Plot
    //==========================================================================
    Array <float> getInputValues();
    void setOutputValues (Array <float> newOutputValues);

protected:
    int axisLabelWidth;

    Array <float> inputValues;

private:
    float lineSeparation;
    float lowX, highX;   
    float lowY, highY;   
    Colour traceColour;
    Colour backgroundColour;

    Array <float> outputValues;

    Path gridPath, tracePath;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FunctionGraph)
};

#endif // __FUNCTIONGRAPH__
