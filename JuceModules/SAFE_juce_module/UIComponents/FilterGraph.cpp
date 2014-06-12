//==========================================================================
//      Control Dot Class
//==========================================================================
//==========================================================================
//      Constructor and Destructor
//==========================================================================
FilterGraph::FilterControl::FilterControl()
    : q (2),
      baseQ (pow ((q - minQ) / (maxQ - minQ), qSkewFactor)),
      qSkewFactor (1),
      minQ (1),
      maxQ (1),
      notifyListeners (true)
{
    addAndMakeVisible (&dot);
    dot.setBounds (0, 0, 10, 10);
    dot.addComponentListener (this);

    setInterceptsMouseClicks (false, true);
}

FilterGraph::FilterControl::~FilterControl()
{
}

//==========================================================================
//      Control Settings
//==========================================================================
void FilterGraph::FilterControl::setParameters (float freq, float gain, float qFactor)
{
    float x = freqToX (freq) - 5;
    float y = gainToY (gain) - 5;

    int width = getWidth();
    int height = getHeight();

    if (x > width - 10)
    {
        x = width - 10.0f;
    }

    if (y > height - 10)
    {
        y = height - 10.0f;
    }

    q = qFactor;
    float qRange = maxQ - minQ;
    float proportion = q - minQ;
    baseQ = pow ((proportion / qRange), qSkewFactor);  

    notifyListeners = false;
    dot.setBounds ((int) x, (int) y, 10, 10);
    notifyListeners = true;
}

void FilterGraph::FilterControl::setLimits (float newMinFreq, float newMaxFreq, float newMinGain, float newMaxGain)
{
    minFreq = newMinFreq;
    maxFreq = newMaxFreq;

    minGain = newMinGain;
    maxGain = newMaxGain;
}

float FilterGraph::FilterControl::getFrequency()
{
    Rectangle <int> bounds = dot.getBounds();
    float x = bounds.getX() + 5.0f;

    int minX = (int) freqToX (minFreq);
    int maxX = (int) freqToX (maxFreq);

    if (x <= minX)
    {
        return minFreq;
    }
    else if (x >= maxX - 1)
    {
        return maxFreq;
    }
    else
    {
        return xToFreq (x);
    }
}

float FilterGraph::FilterControl::getGain()
{
    Rectangle <int> bounds = dot.getBounds();
    float y = bounds.getY() + 5.0f;
    
    int minY = (int) gainToY (maxGain);
    int maxY = (int) gainToY (minGain);

    if (y <= minY)
    {
        return maxGain;
    }
    else if (y >= maxY - 1)
    {
        return minGain;
    }
    else
    {
        return yToGain (y);
    }
}

void FilterGraph::FilterControl::setQLimits (float newMinQ, float newMaxQ, float newQSkewFactor)
{
    minQ = newMinQ;
    maxQ = newMaxQ;
    qSkewFactor = newQSkewFactor;
    
    float qRange = maxQ - minQ;
    float proportion = q - minQ;
    baseQ = pow ((proportion / qRange), qSkewFactor);  
}

float FilterGraph::FilterControl::getQ()
{
    return q;
}

void FilterGraph::FilterControl::setColour (Colour newColour)
{
    dot.colour = newColour;
}

Colour FilterGraph::FilterControl::getColour()
{
    return dot.colour;
}

//==========================================================================
//      Filter Control Callbacks
//==========================================================================
void FilterGraph::FilterControl::componentMovedOrResized (Component& /*component*/, bool /*wasMoved*/, bool /*wasResized*/)
{
    FilterGraph* parent = static_cast <FilterGraph*> (getParentComponent());

    parent->filterControlChanged (this, notifyListeners);
}

void FilterGraph::FilterControl::updateQ (float changeAmount)
{
    baseQ += changeAmount / 4;

    if (baseQ > 1)
    {
        baseQ = 1;
    }
    else if (baseQ < 0)
    {
        baseQ = 0;
    }

    float qRange = maxQ - minQ;
    q = qRange * pow (baseQ, (1 / qSkewFactor)) + minQ;

    FilterGraph* parent = static_cast <FilterGraph*> (getParentComponent());

    parent->filterControlChanged (this, notifyListeners);
}

//==========================================================================
//      Mouse Over Bits
//==========================================================================
void FilterGraph::FilterControl::mouseOverDot()
{
    FilterGraph* parent = static_cast <FilterGraph*> (getParentComponent());
    parent->paintSingleFilter (this);
}

void FilterGraph::FilterControl::mouseOffDot()
{
    FilterGraph* parent = static_cast <FilterGraph*> (getParentComponent());
    parent->hideSingleFilter (true);
}

//==========================================================================
//      Utility Functions
//==========================================================================
float FilterGraph::FilterControl::freqToX (float freq)
{
    Rectangle <int> bounds = getBoundsInParent();
    float x = (float) bounds.getX();

    FilterGraph* parent = static_cast <FilterGraph*> (getParentComponent());

    float parentX = parent->freqToX (freq);

    return parentX - x;
}

float FilterGraph::FilterControl::xToFreq (float xPos)
{
    Rectangle <int> bounds = getBoundsInParent();
    float x = (float) bounds.getX();

    FilterGraph* parent = static_cast <FilterGraph*> (getParentComponent());

    float freq = parent->xToFreq (xPos + x);

    return freq;
}

float FilterGraph::FilterControl::gainToY (float gain)
{
    Rectangle <int> bounds = getBoundsInParent();
    float y = (float) bounds.getY();

    FilterGraph* parent = static_cast <FilterGraph*> (getParentComponent());

    float parentY = parent->gainToY (gain);

    return parentY - y;
}

float FilterGraph::FilterControl::yToGain (float yPos)
{
    Rectangle <int> bounds = getBoundsInParent();
    float y = (float) bounds.getY();

    FilterGraph* parent = static_cast <FilterGraph*> (getParentComponent());

    float gain = parent->yToGain (yPos + y);

    return gain;
}

//==========================================================================
//      The Dot Itself
//==========================================================================
//==========================================================================
//      Constructor and Destructor
//==========================================================================
FilterGraph::FilterControl::DraggableDot::DraggableDot()
    : colour (Colours::blue)
{
    bounds.setMinimumOnscreenAmounts (0xffffff, 0xffffff, 0xffffff, 0xffffff);
}

FilterGraph::FilterControl::DraggableDot::~DraggableDot()
{
}

//==========================================================================
//      Mouse Stuff
//==========================================================================
void FilterGraph::FilterControl::DraggableDot::mouseEnter (const MouseEvent& /*e*/)
{
    FilterControl* parent = static_cast <FilterControl*> (getParentComponent());
    parent->mouseOverDot();
}

void FilterGraph::FilterControl::DraggableDot::mouseExit (const MouseEvent& /*e*/)
{
    FilterControl* parent = static_cast <FilterControl*> (getParentComponent());
    parent->mouseOffDot();
}

void FilterGraph::FilterControl::DraggableDot::mouseDown (const MouseEvent& e)
{
    dragger.startDraggingComponent (this, e);
}

void FilterGraph::FilterControl::DraggableDot::mouseDrag (const MouseEvent& e)
{
    dragger.dragComponent (this, e, &bounds);
}

void FilterGraph::FilterControl::DraggableDot::mouseWheelMove (const MouseEvent& /*e*/, const MouseWheelDetails& w)
{
    FilterControl* parent = static_cast <FilterControl*> (getParentComponent());
    parent->updateQ (w.deltaY);
}

//==========================================================================
//      Paint
//==========================================================================
void FilterGraph::FilterControl::DraggableDot::paint (Graphics& g)
{
    int width = getWidth();
    int height = getHeight();

    g.setColour (colour);
    g.fillEllipse (0.0f, 0.0f, (float) width, (float) height);
}

//==========================================================================
//      The Graph Itself
//==========================================================================
//==========================================================================
//      Constructor and Destructor
//==========================================================================
FilterGraph::FilterGraph (int numFiltersInit)
    : externalShowSingleFilter (false),
      showSingleFilter (false),
      filterToShow (0),
      numHorizontalLines (7),
      lowFreq (20),
      highFreq (20000),
      maxDecibels (6),
      maxPhase (1),
      traceColour (Colour (0xaa00ff00)),
      traceType (Magnitude),
      fs (44100),
      numFilters (numFiltersInit)
{    
    setSize (500, 300);

    for (int n = 0; n < numFilters; ++n)
    {
        filterArray.add (new FilterInfo);
        controlArray.add (new FilterControl);

        addAndMakeVisible (controlArray [n]);
    }
}

FilterGraph::~FilterGraph()
{
    listenerArray.clear (false);
}

//==========================================================================
//      Paint
//==========================================================================
void FilterGraph::paintSingleFilter (FilterControl* control)
{
    int filterNum = 0;

    for (int n = 0; n < numFilters; ++n)
    {
        if (control == controlArray [n])
        {
            filterNum = n;
        }  
    }

    showSingleFilter = true;
    filterToShow = filterNum;

    repaint();
}

void FilterGraph::paintSingleFilter (int filterNum)
{
    externalShowSingleFilter = true;
    showSingleFilter = true;
    filterToShow = filterNum;

    repaint();
}

void FilterGraph::hideSingleFilter (bool internallyCalled)
{
    if (internallyCalled)
    {
        showSingleFilter = false;
        repaint();
    }
    else if (externalShowSingleFilter)
    {
        externalShowSingleFilter = false;
        showSingleFilter = false;
        repaint();
    }
}

void FilterGraph::paint (Graphics& g)
{
    // get size info
    float width = (float) getWidth();
    float height = (float) getHeight();
    
    // paint the display background
    g.setGradientFill (ColourGradient (Colour (0xff232338), width / 2, height / 2, Colour (0xff21222a), 2.5f, height / 2, true));
    g.fillRoundedRectangle (2.5f, 2.5f, width - 5, height - 5, 10.000f);

    // paint single filter
    if (showSingleFilter)
    {
        singleFilterPath.clear();

        if (traceType == Magnitude)
        {
            float traceMagnitude = (float) filterArray [filterToShow]->getResponse (lowFreq).magnitudeValue;
            traceMagnitude = Decibels::gainToDecibels (traceMagnitude);
            singleFilterPath.startNewSubPath (2.5f, gainToY (traceMagnitude));

            for (float xPos = 3.5; xPos < (width - 2.5); xPos += 1)
            {
                float freq = xToFreq (xPos);
                traceMagnitude = (float) filterArray [filterToShow]->getResponse (freq).magnitudeValue;
                traceMagnitude = Decibels::gainToDecibels (traceMagnitude);
                singleFilterPath.lineTo (xPos, gainToY (traceMagnitude));
            }
        }

        if (traceType == Phase)
        {
            float tracePhase = (float) filterArray [filterToShow]->getResponse (lowFreq).phaseValue;
            singleFilterPath.startNewSubPath (2.5f, phaseToY (tracePhase / float_Pi));
            float previousPhase = tracePhase;
            float unwrapSummand = 0;

            for (float xPos = 3.5; xPos < (width - 2.5); xPos += 1)
            {
                float freq = xToFreq (xPos);
                tracePhase = (float) filterArray [filterToShow]->getResponse (freq).phaseValue;

                if (fabs (tracePhase - previousPhase) > 5)
                {
                    unwrapSummand += float_Pi * 2;
                }

                previousPhase = tracePhase;

                singleFilterPath.lineTo (xPos, phaseToY ((tracePhase + unwrapSummand) / float_Pi));
            }
        }

        g.setColour (controlArray [filterToShow]->getColour().withAlpha ((uint8) 0xaa));
        g.strokePath (singleFilterPath, PathStrokeType (3.0f));
    }

    // paint the only trace at the moment
    tracePath.clear();
    
    if (traceType == Magnitude)
    {
        float traceMagnitude = getMagnitudeResponse (lowFreq);
        tracePath.startNewSubPath (2.5f, gainToY (traceMagnitude));

        for (float xPos = 3.5; xPos < (width - 2.5); xPos += 1)
        {
            float freq = xToFreq (xPos);
            traceMagnitude = getMagnitudeResponse (freq);
            tracePath.lineTo (xPos, gainToY (traceMagnitude));
        }
    }
    
    if (traceType == Phase)
    {
        float tracePhase = getPhaseResponse (lowFreq);
        tracePath.startNewSubPath (2.5f, phaseToY (tracePhase / float_Pi));
        float previousPhase = tracePhase;
        float unwrapSummand = 0;

        for (float xPos = 3.5; xPos < (width - 2.5); xPos += 1)
        {
            float freq = xToFreq (xPos);
            tracePhase = getPhaseResponse (freq);

            if (fabs (tracePhase - previousPhase) > 5)
            {
                unwrapSummand += float_Pi * 2;
            }

            previousPhase = tracePhase;

            tracePath.lineTo (xPos, phaseToY ((tracePhase + unwrapSummand) / float_Pi));
        }
    }
    
    g.setColour (traceColour);
    g.strokePath (tracePath, PathStrokeType (3.0f));
    
    // paint the display grid lines
    g.setColour (Colour (0xaaffffff));
    String axisLabel;
    if (traceType == Magnitude)  axisLabel = String (maxDecibels, 1) + "dB";
    else if (traceType == Phase) 
    {
        axisLabel = String (CharPointer_UTF8 ("\xcf\x80")) + "rad";
        axisLabel = (maxPhase == 1) ? axisLabel : String (maxPhase, 1) + axisLabel;
    }
    
    g.setFont (Font ("Arial Rounded MT", 12.0f, Font::plain));
    g.drawText (axisLabel, 6, (int) gainToY (maxDecibels) - 13, 45, 12, Justification::centred, false);
    g.drawText (String ("-") + axisLabel, 6, (int) gainToY (- maxDecibels) + 1, 45, 12, Justification::centred, false);
    
    if(showSingleFilter && ! externalShowSingleFilter)
    {
        g.setColour (Colours::white);
        g.drawText (String ("Scroll over a filter to control its Q"), 590, (int) gainToY (maxDecibels) - 13, 180, 12, Justification::left, false);
    }
    
    gridPath.clear();
    for (int lineNum = 1; lineNum < numHorizontalLines + 1; ++lineNum)
    {
        float yPos = lineNum * (height - 5) / (numHorizontalLines + 1) + 2.5f;
        gridPath.startNewSubPath (2.5f, yPos);
        gridPath.lineTo (width - 2.5f, yPos);
    }
    
    float order = (float) pow (10, floor (log10 (lowFreq)));
    float rounded = order * (floor(lowFreq / order) + 1);
    for (float freq = rounded; freq < highFreq; freq += (float) pow (10, floor (log10 (freq))))
    {
        float xPos = freqToX (freq);
        gridPath.startNewSubPath (xPos, 2.5f);
        gridPath.lineTo (xPos, height - 2.5f);
    }
    g.excludeClipRegion (Rectangle <int> (6, (int) gainToY (maxDecibels) - 13, 45, 12));
    g.excludeClipRegion (Rectangle <int> (6, (int) gainToY (- maxDecibels) + 1, 45, 12));
    
    if(showSingleFilter && ! externalShowSingleFilter)
    {
        g.excludeClipRegion (Rectangle <int> (590, (int) gainToY (maxDecibels) - 13, 180, 12));
    }
    
    g.setColour (Colour (0x60ffffff));   
    g.strokePath (gridPath, PathStrokeType (1.0f));
}

void FilterGraph::paintOverChildren (Graphics& g)
{
    // get size info
    float width = (float) getWidth();
    float height = (float) getHeight();

    // draw the boarder
    g.setColour (Colours::black);
    g.drawRoundedRectangle (2.5f, 2.5f, width - 5, height - 5, 10.000f, 5.000f);
}

void FilterGraph::resized()
{
}

//==========================================================================
//      Change Appearance
//==========================================================================
void FilterGraph::setNumHorizontalLines (int newValue)
{
    numHorizontalLines = newValue;
    repaint();
}

void FilterGraph::setFreqRange (float newLowFreq, float newHighFreq)
{
    lowFreq = fabs (newLowFreq + 0.1f);
    highFreq = fabs (newHighFreq);
    repaint();
}

void FilterGraph::setMaxDecibels (float newMaxDecibels)
{
    maxDecibels = newMaxDecibels;
    repaint();
}

void FilterGraph::setMaxPhase (float newMaxPhase)
{
    maxPhase = newMaxPhase;
    repaint();
}

void FilterGraph::setTraceColour (Colour newColour)
{
    traceColour = newColour;
    repaint();
}

void FilterGraph::setTraceType (TraceType newTraceType)
{
    traceType = newTraceType;
    repaint();
}

//==========================================================================
//      Set Filters
//==========================================================================
void FilterGraph::setFilterGain (int filterNum, double gain)
{
    filterArray [filterNum]->setGain (gain);
    repaint();
}

void FilterGraph::setFilter (int filterNum, double sampleRate, IIRCoefficients newCoefficients)
{
    filterArray [filterNum]->setSampleRate (sampleRate);
    filterArray [filterNum]->setCoefficients (newCoefficients);
    
    repaint();
}

void FilterGraph::setFilter (int filterNum, double sampleRate, Array <double> numCoeffs, Array <double> denCoeffs)
{
    filterArray [filterNum]->setSampleRate (sampleRate);
    filterArray [filterNum]->setCoefficients (numCoeffs, denCoeffs);
    
    repaint();
}

//==========================================================================
//      Set Control Dots
//==========================================================================
void FilterGraph::setControlLimits (int filterNum, float minFreq, float maxFreq, float minGain, float maxGain)
{
    FilterControl* control = controlArray [filterNum];
    
    int x = (int) freqToX (minFreq) - 5;
    int y = (int) gainToY (maxGain) - 5;
    int width = (int) freqToX (maxFreq) - x + 5;
    int height = (int) gainToY (minGain) - y + 5;
    control->setBounds (x, y, width, height);
    control->setLimits (minFreq, maxFreq, minGain, maxGain);
}

void FilterGraph::setControlQLimits (int filterNum, float minQ, float maxQ, float qSkewFactor)
{
    FilterControl* control = controlArray [filterNum];
    control->setQLimits (minQ, maxQ, qSkewFactor);
}

void FilterGraph::setControlParameters (int filterNum, float freq, float gain, float qFactor)
{
    FilterControl* control = controlArray [filterNum];
    control->setParameters (freq, gain, qFactor);
}

float FilterGraph::getControlFrequency (int controlNum)
{
    return controlArray [controlNum]->getFrequency();
}

float FilterGraph::getControlGain (int controlNum)
{
    return controlArray [controlNum]->getGain();
}

float FilterGraph::getControlQ (int controlNum)
{
    return controlArray [controlNum]->getQ();
}

void FilterGraph::setControlColour (int filterNum, Colour newColour)
{
    FilterControl* control = controlArray [filterNum];
    control->setColour (newColour);
}

//==========================================================================
//      Utility Functions
//==========================================================================
float FilterGraph::xToFreq (float xPos)
{
    float width = (float) getWidth();
    return lowFreq * pow ((highFreq / lowFreq), ((xPos - 2.5f) / (width - 5.0f)));
}

float FilterGraph::freqToX (float freq)
{
    float width = (float) getWidth();
    return (width - 5) * (log (freq / lowFreq) / log (highFreq / lowFreq)) + 2.5f;
}

float FilterGraph::yToGain (float yPos)
{
    float height = (float) getHeight();
    float offsetYPos = yPos - height / 2;

    float scaleFactor = - (numHorizontalLines + 1) * maxDecibels * 2 / ((numHorizontalLines - 1) * (height - 5));

    return offsetYPos * scaleFactor;
}

float FilterGraph::gainToY (float gain)
{
    float height = (float) getHeight();

    float scaleFactor = - (numHorizontalLines - 1) * (height - 5) / ((numHorizontalLines + 1) * maxDecibels * 2);

    float scaledGain = scaleFactor * gain;

    return scaledGain + height / 2;
}

float FilterGraph::yToPhase (float yPos)
{
    float height = (float) getHeight();
    float offsetYPos = yPos - height / 2;

    float scaleFactor = - (numHorizontalLines + 1) * maxPhase * 2 / ((numHorizontalLines - 1) * (height - 5));

    return offsetYPos * scaleFactor;
}

float FilterGraph::phaseToY (float phase)
{
    float height = (float) getHeight();

    float scaleFactor = - (numHorizontalLines - 1) * (height - 5) / ((numHorizontalLines + 1) * maxPhase * 2);

    float scaledPhase = scaleFactor * phase;

    return scaledPhase + height / 2;
}

float FilterGraph::getMagnitudeResponse (float freq)
{
    float magnitudeResponse = 0;

    if (numFilters > 0)
    {
        magnitudeResponse = (float) (filterArray [0]->getResponse (freq).magnitudeValue);
        
        for (int i = 1; i < numFilters; ++i)
        {
            magnitudeResponse *= (float) (filterArray [i]->getResponse (freq).magnitudeValue);
        }

        magnitudeResponse = Decibels::gainToDecibels (magnitudeResponse);
    }

    return magnitudeResponse;
}

float FilterGraph::getPhaseResponse (float freq)
{
    float phaseResponse = 0;

    if (numFilters > 0)
    {
        phaseResponse = (float) (filterArray [0]->getResponse (freq).phaseValue);
        
        for (int i = 1; i < numFilters; ++i)
        {
            phaseResponse += (float) (filterArray [i]->getResponse (freq).phaseValue);
        }
    }

    return phaseResponse;
}

//==========================================================================
//      Listener Stuff
//==========================================================================
void FilterGraph::addListener (Listener* newListener)
{
    listenerArray.add (newListener);
}

void FilterGraph::removeListener (Listener* newListener)
{
    listenerArray.removeObject (newListener);
}

void FilterGraph::filterControlChanged (FilterControl* control, bool notifyListeners)
{
    if (notifyListeners)
    {
        int controlNum = 0;

        for (int n = 0; n < numFilters; ++n)
        {
            if (control == controlArray [n])
            {
                controlNum = n;
            }  
        }

        for (int i = 0; i < listenerArray.size(); ++i)
        {
            listenerArray [i]->filterControlChanged (controlNum);
        }
    }
}

void FilterGraph::mouseMove (const MouseEvent& /*event*/)
{
    Point <int> mousePos =  getMouseXYRelative();
    float xPos = (float) mousePos.getX();
    float freq = xToFreq (xPos);
    
    if (traceType == Magnitude)
    {
        float magnitude = (float) (filterArray [0]->getResponse (freq).magnitudeValue);
    
        for (int i = 1; i < numFilters; ++i)
        {
            magnitude *= (float) (filterArray [i]->getResponse (freq).magnitudeValue);
        }
    
        magnitude = Decibels::gainToDecibels (magnitude);
    
        setTooltip (String (freq, 1) + "Hz, " + String (magnitude, 1) + "dB");
    }
    
    if (traceType == Phase)
    {
        float phase = (float) (filterArray [0]->getResponse (freq).phaseValue);
    
        for (int i = 1; i < numFilters; ++i)
        {
            phase += (float) (filterArray [i]->getResponse (freq).phaseValue);
        }
        
        phase /= float_Pi;
    
        setTooltip (String (freq, 1) + "Hz, " + String (phase, 2) + String (CharPointer_UTF8 ("\xcf\x80")) + "rad");
    }
}
