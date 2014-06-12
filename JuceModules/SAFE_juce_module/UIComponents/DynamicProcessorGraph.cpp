//==========================================================================
//      Bounding Component for Meter Dot
//==========================================================================
//==========================================================================
//      Constructor and Destructor
//==========================================================================
DynamicProcessorGraph::DynamicsMeter::DynamicsMeter()
{
    addAndMakeVisible (&dot);
}

DynamicProcessorGraph::DynamicsMeter::~DynamicsMeter()
{
}

//==========================================================================
//      Set Dot Position
//==========================================================================
void DynamicProcessorGraph::DynamicsMeter::setDotPosition (int xPos, int yPos)
{
    dot.setBounds (xPos, yPos, 10, 10);
}

//==========================================================================
//      Dot Component
//==========================================================================
//==========================================================================
//      Constructor and Destructor
//==========================================================================
DynamicProcessorGraph::DynamicsMeter::Dot::Dot()
    : colour (Colours::orange)
{
}

DynamicProcessorGraph::DynamicsMeter::Dot::~Dot()
{
}

//==========================================================================
//      Paint
//==========================================================================
void DynamicProcessorGraph::DynamicsMeter::Dot::paint (Graphics& g)
{
    float width = (float) getWidth();
    float height = (float) getHeight();

    g.setColour (colour);
    g.fillEllipse (0, 0, width, height);
}

//==========================================================================
//      The Graph Itself
//==========================================================================
//==========================================================================
//      Constructor and Destructor
//==========================================================================
DynamicProcessorGraph::DynamicProcessorGraph()
    : LevelMeterTimer (2)
{
    int width = getWidth();
    int height = getHeight();

    addAndMakeVisible (&meterArea);
    meterArea.setBounds (axisLabelWidth + 5, 5, width - 10 - axisLabelWidth, height - 10 - axisLabelWidth);

    setMeterRiseTime (10);
    setMeterDecayTime (50);
}

DynamicProcessorGraph::~DynamicProcessorGraph()
{
}

//==========================================================================
//      Paint
//==========================================================================
void DynamicProcessorGraph::paint (Graphics& g)
{
    FunctionGraph::paint (g);

    // get size info
    float width = (float) getWidth();
    float height = (float) getHeight();

    // paint the liner in out line
    linePath.clear();

    int valueIndex = 0;
    float xPos = axisValueToX (inputValues [valueIndex]);
    float yPos = axisValueToY (inputValues [valueIndex]);
    linePath.startNewSubPath (xPos, yPos); 

    for (valueIndex = 1; valueIndex < inputValues.size(); ++valueIndex)
    {
        xPos = axisValueToX (inputValues [valueIndex]);
        yPos = axisValueToY (inputValues [valueIndex]);
        linePath.lineTo (xPos, yPos); 
    }

    g.saveState();
    g.reduceClipRegion (Rectangle <int> (5 + axisLabelWidth, 5, (int) width - 5 - axisLabelWidth, (int) height - 5 - axisLabelWidth));

    g.setColour (Colour (0x60ffffff));   
    g.strokePath (linePath, PathStrokeType (1.0f));

    g.restoreState();
}

void DynamicProcessorGraph::paintOverChildren (Graphics& g)
{
    // get size info
    float width = (float) getWidth();
    float height = (float) getHeight();

    // draw the boarder
    g.setColour (Colours::black);
    g.drawRoundedRectangle (2.5f + axisLabelWidth, 2.5f, width - 5 - axisLabelWidth, height - 5 -axisLabelWidth, 10.000f, 5.000f);
}

void DynamicProcessorGraph::resized()
{
    int width = getWidth();
    int height = getHeight();

    meterArea.setBounds (axisLabelWidth + 2, 2, width - 5 - axisLabelWidth, height - 5 - axisLabelWidth);
}

//==========================================================================
//      Metering
//==========================================================================
void DynamicProcessorGraph::setMeterLevels (float inputLevel, float outputLevel)
{
    setMeterLevel (0, inputLevel);
    setMeterLevel (1, outputLevel);
}

void DynamicProcessorGraph::meterLevelChanged()
{
    xPos = (int) axisValueToX ((float) getMeterLevel (0)) - 5;
    yPos = (int) axisValueToY ((float) getMeterLevel (1)) - 5;

    meterArea.setDotPosition ((int) (xPos - axisLabelWidth - 2), (int) (yPos - 2));
}
