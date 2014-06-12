//==========================================================================
//      Constructor and Destructor
//==========================================================================
FunctionGraph::FunctionGraph()
    : axisLabelWidth (30),
      lineSeparation (0.2f),
      lowX (0), highX (1),
      lowY (0), highY (1),
      traceColour (Colour (0xaa00ff00)),
      backgroundColour (0xff222222)
{
    setSize (200, 200);

    float width = (float) getWidth();

    for (float xPos = 2.5f + axisLabelWidth; xPos < width - 2.5f; ++xPos)
    {
        inputValues.add (xToAxisValue (xPos));
        outputValues.add (xToAxisValue (xPos));
    }
}

FunctionGraph::~FunctionGraph()
{
}

//==========================================================================
//      Paint
//==========================================================================
void FunctionGraph::paint (Graphics& g)
{
    // get size info
    float width = (float) getWidth();
    float height = (float) getHeight();
    
    g.fillAll (backgroundColour);

    // paint the display background
    g.setGradientFill (ColourGradient (Colour (0xff232338), width / 2, height / 2, Colour (0xff21222a), 2.5f, height / 2, true));
    g.fillRoundedRectangle (2.5f + axisLabelWidth, 2.5f, width - 5 - axisLabelWidth, height - 5 - axisLabelWidth, 10.000f);

    // paint the trace
    tracePath.clear();

    if (inputValues.size() == outputValues.size())
    {
        int valueIndex = 0;
        float xPos = axisValueToX (inputValues [valueIndex]);
        float yPos = axisValueToY (outputValues [valueIndex]);
        tracePath.startNewSubPath (xPos, yPos); 

        for (valueIndex = 1; valueIndex < inputValues.size(); ++valueIndex)
        {
            xPos = axisValueToX (inputValues [valueIndex]);
            yPos = axisValueToY (outputValues [valueIndex]);
            tracePath.lineTo (xPos, yPos); 
        }

        g.saveState();
        g.reduceClipRegion (Rectangle <int> (5 + axisLabelWidth, 5, (int) width - 5 - axisLabelWidth, (int) height - 5 - axisLabelWidth));
        g.setColour (traceColour);
        g.strokePath (tracePath, PathStrokeType (3.0f));
        g.restoreState();
    }

    // paint the display grid lines
    gridPath.clear();

    g.setColour (Colours::white);

    float firstXValue = ceil (lowX / lineSeparation) * lineSeparation;

    for (float xValue = firstXValue; xValue < highX; xValue += lineSeparation)
    {
        float xCoord = axisValueToX (xValue);
        gridPath.startNewSubPath (xCoord, 2.5f);
        gridPath.lineTo (xCoord, height - 2.5f - axisLabelWidth);

        g.drawText (String (xValue), (int) xCoord - axisLabelWidth / 2, (int) height - axisLabelWidth, axisLabelWidth, 20, Justification::centred, false);
    }

    float firstYValue = ceil (lowY / lineSeparation) * lineSeparation;

    for (float yValue = firstYValue; yValue < highY; yValue += lineSeparation)
    {
        float yCoord = axisValueToY (yValue);
        gridPath.startNewSubPath (2.5f + axisLabelWidth, yCoord);
        gridPath.lineTo (width - 2.5f, yCoord);

        g.drawText (String (yValue), 0, (int) yCoord - 10, axisLabelWidth, 20, Justification::centred, false);
    }

    g.saveState();
    g.reduceClipRegion (Rectangle <int> (5 + axisLabelWidth, 5, (int) width - 5 - axisLabelWidth, (int) height - 5 - axisLabelWidth));
    g.setColour (Colour (0x60ffffff));   
    g.strokePath (gridPath, PathStrokeType (1.0f));
    g.restoreState();

    // draw the boarder
    g.setColour (Colours::black);
    g.drawRoundedRectangle (2.5f + axisLabelWidth, 2.5f, width - 5 - axisLabelWidth, height - 5 -axisLabelWidth, 10.000f, 5.000f);
}

void FunctionGraph::resized()
{
    float width = (float) getWidth();

    inputValues.clear();
    outputValues.clear();

    for (float xPos = 2.5f + axisLabelWidth; xPos < width - 2.5f; ++xPos)
    {
        inputValues.add (xToAxisValue (xPos));
        outputValues.add (xToAxisValue (xPos));
    }
}

//==========================================================================
//      Change Appearance
//==========================================================================
void FunctionGraph::setLineSeparation (float newValue)
{
    lineSeparation = newValue;
    repaint();
}

void FunctionGraph::setXAxisRange (float newLowX, float newHighX)
{
    lowX = newLowX;
    highX = newHighX;
    repaint();

    float width = (float) getWidth();

    inputValues.clear();
    outputValues.clear();

    for (float xPos = 2.5f + axisLabelWidth; xPos < width - 2.5f; ++xPos)
    {
        inputValues.add (xToAxisValue (xPos));
        outputValues.add (xToAxisValue (xPos));
    }
}

void FunctionGraph::setYAxisRange (float newLowY, float newHighY)
{
    lowY = newLowY;
    highY = newHighY;
    repaint();
}

void FunctionGraph::setTraceColour (Colour newColour)
{
    traceColour = newColour;
    repaint();
}

void FunctionGraph::setBackgroundColour (Colour newColour)
{
    backgroundColour = newColour;
    repaint();
}

//==========================================================================
//      Utility Functions
//==========================================================================
float FunctionGraph::xToAxisValue (float xPos)
{
    float width = (float) getWidth() - 5;
    return (xPos - 2.5f - axisLabelWidth) * (highX - lowX) / (width - axisLabelWidth) + lowX;
}

float FunctionGraph::axisValueToX (float axisValue)
{
    float width = (float) getWidth() - 5;
    return (axisValue - lowX) * (width - axisLabelWidth) / (highX - lowX) + 2.5f + axisLabelWidth;
}

float FunctionGraph::yToAxisValue (float yPos)
{
    float height = (float) getHeight() - 5;
    return - (yPos - 2.5f) * (highY - lowY) / (height - axisLabelWidth) + highY;
}

float FunctionGraph::axisValueToY (float axisValue)
{
    float height = (float) getHeight() - 5;
    return - (axisValue - highY) * (height - axisLabelWidth) / (highY - lowY) + 2.5f;
}


//==========================================================================
//      Function to Plot
//==========================================================================
Array <float> FunctionGraph::getInputValues()
{
    return inputValues;
}

void FunctionGraph::setOutputValues(Array <float> newOutputValues)
{
    outputValues = newOutputValues;
    repaint();
}
