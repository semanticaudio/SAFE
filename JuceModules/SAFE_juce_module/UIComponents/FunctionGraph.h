#ifndef __FUNCTIONGRAPH__
#define __FUNCTIONGRAPH__

/**
 *  A graphical component to display some function on a graph.
 */
class FunctionGraph : public Component
{
public:
    //==========================================================================
    //      Constructor and Destructor
    //==========================================================================
    /** Create a new function graph. */
    FunctionGraph();

    /** Destructor */
    ~FunctionGraph();

    //==========================================================================
    //      Paint
    //==========================================================================
    /** Draw the component. */
    void paint (Graphics&);

    /** Function called when the size of the graph is changed. */
    void resized();

    //==========================================================================
    //      Change Appearance
    //==========================================================================
    /** Set the distance between grid lines on the graph
     *
     *  @param newValue  the new grid line separation
     */
    void setLineSeparation (float newValue);

    /** Set the value range for the x axis on the graph.
     *
     *  @param newLowX   the new low end of the x axis
     *  @param newHighX  the new hight end of the x axis
     */
    void setXAxisRange (float newLowX, float newHighX);

    /** Set the value range for the y axis on the graph.
     *
     *  @param newLowY   the new low end of the y axis
     *  @param newHighY  the new hight end of the y axis
     */
    void setYAxisRange (float newLowY, float newHighY);

    /** Set the colour of the trace line.
     *
     *  @param newColour  the new trace colour
     */
    void setTraceColour (Colour newColour);

    /** Set the background colour.
     *
     *  @param newColour  the new background colour
     */
    void setBackgroundColour (Colour newColour);

    //==========================================================================
    //      Utility Functions
    //==========================================================================
    /** Get the x axis value which corresponds to a given x position in the component.
     *  
     *  @param xPos  the x position in the component to convert to an axis value
     */
    float xToAxisValue (float xPos);

    /** Get the x position in the component which corresponds to a given x axis value.
     *  
     *  @param axisValue  the x axis value to convert to an x position in the component
     */
    float axisValueToX (float axisValue);

    /** Get the y axis value which corresponds to a given y position in the component.
     *  
     *  @param yPos  the y position in the component to convert to an axis value
     */
    float yToAxisValue (float yPos);

    /** Get the y position in the component which corresponds to a given y axis value.
     *  
     *  @param axisValue  the y axis value to convert to a y position in the component
     */
    float axisValueToY (float axisValue);

    //==========================================================================
    //      Function to Plot
    //==========================================================================
    /** Get an array of values which will be used as the x values when plotting the graph. */
    Array <float> getInputValues();

    /** Provide an array of values which will be used as the y values when plotting the graph. 
     *
     *  @param newOutputValues  an array of values to plot on the graph - each element should be the corresponding y value to the x value in the same element of the array returned by getInputValues()
     */
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
