//==========================================================================
//      Constructor and Destructor
//==========================================================================
XYSlider::ControlDot::ControlDot()
    : notifyListeners (true)
{
    addAndMakeVisible (&dot);
    dot.setBounds (0, 0, 10, 10);
    dot.addComponentListener (this);

    setInterceptsMouseClicks (false, true);
}

XYSlider::ControlDot::~ControlDot()
{
}

//==========================================================================
//      Control Settings
//==========================================================================
float XYSlider::ControlDot::getX()
{
    Rectangle <int> bounds = dot.getBounds();
    return bounds.getX() + 5.0f;
}

float XYSlider::ControlDot::getY()
{
    Rectangle <int> bounds = dot.getBounds();
    return bounds.getY() + 5.0f;
}

void XYSlider::ControlDot::setX (int newX)
{
    notifyListeners = false;
    dot.setTopLeftPosition (newX, dot.getY());
    notifyListeners = true;
}

void XYSlider::ControlDot::setY (int newY)
{
    notifyListeners = false;
    dot.setTopLeftPosition (dot.getX(), newY);
    notifyListeners = true;
}

void XYSlider::ControlDot::parentSizeChanged()
{
    setBounds (5.0f, 5.0f, getParentWidth() - 10.0f, getParentHeight() - 10.0f);
}

//==========================================================================
//      Control Callbacks
//==========================================================================
void XYSlider::ControlDot::componentMovedOrResized (Component& component, bool wasMoved, bool wasResized)
{
    XYSlider* parent = static_cast <XYSlider*> (getParentComponent());

    parent->controlChanged (notifyListeners);
}

//==========================================================================
//      Mouse Over Bits
//==========================================================================
void XYSlider::ControlDot::mouseOverDot()
{
}

void XYSlider::ControlDot::mouseOffDot()
{
}

//==========================================================================
//      Constructor and Destructor
//==========================================================================
XYSlider::ControlDot::DraggableDot::DraggableDot()
{
    bounds.setMinimumOnscreenAmounts (0xffffff, 0xffffff, 0xffffff, 0xffffff);
}

XYSlider::ControlDot::DraggableDot::~DraggableDot()
{
}

//==========================================================================
//      Mouse Stuff
//==========================================================================
void XYSlider::ControlDot::DraggableDot::mouseDown (const MouseEvent& e)
{
    dragger.startDraggingComponent (this, e);
}

void XYSlider::ControlDot::DraggableDot::mouseDrag (const MouseEvent& e)
{
    dragger.dragComponent (this, e, &bounds);
}

//==========================================================================
//      Paint
//==========================================================================
void XYSlider::ControlDot::DraggableDot::paint (Graphics& g)
{
    int width = getWidth();
    int height = getHeight();

    g.setColour (Colours::yellow);
    g.fillEllipse (0.0f, 0.0f, (float) width, (float) height);
}

XYSlider::XYSlider()
{
    minX = minY = 0;
    maxX = maxY = 0;

    addAndMakeVisible (&controlDot);
}

XYSlider::~XYSlider()
{
    listenerArray.clear (false);
}

void XYSlider::paint (Graphics &g)
{
    // get size info
    float width = (float) getWidth();
    float height = (float) getHeight();
    
    // paint the display background
    g.setGradientFill (ColourGradient (Colour (0xff232338), width / 2, height / 2, Colour (0xff21222a), 2.5f, height / 2, true));
    g.fillRoundedRectangle (2.5f, 2.5f, width - 5, height - 5, 10.000f);
}

void XYSlider::paintOverChildren (Graphics &g)
{
    // get size info
    float width = (float) getWidth();
    float height = (float) getHeight();

    // draw the boarder
    g.setColour (Colours::black);
    g.drawRoundedRectangle (2.5f, 2.5f, width - 5, height - 5, 10.000f, 5.000f);
}

double XYSlider::getXValue()
{
    double proportionOfWidth = (controlDot.getX() - 5.0) / (controlDot.getWidth() - 10.0);
    double xRange = maxX - minX;

    return proportionOfWidth * xRange + minX;
}

double XYSlider::getYValue()
{
    double proportionOfHeight = 1 - (controlDot.getY() - 5.0) / (controlDot.getHeight() - 10.0);
    double yRange = maxY - minY;

    return proportionOfHeight * yRange + minY;
}

void XYSlider::setXValue (double newValue)
{
    double xRange = maxX - minX;
    double proportionOfWidth = (newValue - minX) / xRange;
    double x = proportionOfWidth * (controlDot.getWidth() - 10.0);

    controlDot.setX (x);
}

void XYSlider::setYValue (double newValue)
{
    double yRange = maxY - minY;
    double proportionOfHeight = (newValue - minY) / yRange;
    double y = (1 - proportionOfHeight) * (controlDot.getHeight() - 10.0f);

    controlDot.setY (y);
}

void XYSlider::setRange (double newMinX, double newMaxX, double newMinY, double newMaxY)
{
    minX = newMinX;
    maxX = newMaxX;
    minY = newMinY;
    maxY = newMaxY;
}

//==========================================================================
//      Listener Stuff
//==========================================================================
void XYSlider::addListener (Listener* newListener)
{
    listenerArray.add (newListener);
}

void XYSlider::removeListener (Listener* listener)
{
    listenerArray.removeObject (listener);
}

void XYSlider::controlChanged (bool notifyListeners)
{
    if (notifyListeners)
    {
        for (int i = 0; i < listenerArray.size(); ++i)
        {
            listenerArray [i]->xySliderMoved (this);
        }
    }
}
