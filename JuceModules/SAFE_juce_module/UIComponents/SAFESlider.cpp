//==========================================================================
//      Text Box Class
//==========================================================================
//==========================================================================
//      Constructor and Destructor
//==========================================================================
SAFESlider::SAFESliderTextBox::SAFESliderTextBox(const String& componentName, const String& labelText)
    : Label (componentName, labelText),
      isChangable (true)
{
}

SAFESlider::SAFESliderTextBox::~SAFESliderTextBox()
{
}

//==========================================================================
//      Mouse Callbacks
//==========================================================================
void SAFESlider::SAFESliderTextBox::mouseEnter (const MouseEvent& /*e*/)
{
    isChangable = false;
    Label::setText ("Reset", dontSendNotification);
}

void SAFESlider::SAFESliderTextBox::mouseExit (const MouseEvent& /*e*/)
{
    isChangable = true;
}

void SAFESlider::SAFESliderTextBox::mouseDown (const MouseEvent& /*e*/)
{
    SAFESlider* parent = static_cast <SAFESlider*> (getParentComponent());
    parent->reset();
}

//==========================================================================
//      Set Text
//==========================================================================
void SAFESlider::SAFESliderTextBox::setText (String newText)
{
    content = newText;

    if (isChangable)
    {
        Label::setText (content, dontSendNotification);
    }
}

//==========================================================================
//      The Slider Class Proper
//==========================================================================
//==========================================================================
//      Constructor and Destructor
//==========================================================================
SAFESlider::SAFESlider()
    : label ("", ""),
      defaultValue (0),
      slider (Slider::RotaryHorizontalVerticalDrag, Slider::NoTextBox),
      sliderWidth (80),
      sliderHeight (80),
      textBox ("", ""),
      textBoxWidth (50),
      textBoxHeight (15),
      numDecimalPlaces (1),
      textBoxScaleFactor (1),
      useSIPrefixes (true)
{
    setSize (80, 80);

    // add the slider
    addAndMakeVisible (&slider);
    slider.setBounds (0, 0, 80, 80);
    slider.addListener (this);

    // add the label
    addAndMakeVisible (&label);
    label.setBounds (0, 0, 80, 80);
    label.setInterceptsMouseClicks (false, false);
    label.setJustificationType (Justification::centred);

    // add the text box
    addAndMakeVisible (&textBox);
    textBox.setJustificationType (Justification::centred);
    textBox.setColour (Label::outlineColourId, Colours::black);
    textBox.addMouseListener (this, false);
}

SAFESlider::~SAFESlider()
{
}

//==========================================================================
//      Set Component Sizes
//==========================================================================
void SAFESlider::setSliderSize (int width, int height)
{
    sliderWidth = width;
    sliderHeight = height;
}

void SAFESlider::setTextBoxSize (int width, int height)
{
    textBoxWidth = width;
    textBoxHeight = height;
}

void SAFESlider::setColour (Colour newColour)
{
    slider.setColour (Slider::rotarySliderFillColourId, newColour);
    textBox.setColour (Label::backgroundColourId, newColour);
}

void SAFESlider::setTextBoxFont (Font newFont)
{
    textBox.setFont (newFont);
}

//==========================================================================
//      Slider Forwarding Stuff
//==========================================================================
void SAFESlider::setValue (double newValue, NotificationType notification)
{
    slider.setValue (newValue, notification);

    String valueString;

    if (units != String::empty && useSIPrefixes)
    {
        valueString = makeSIValueString (newValue * textBoxScaleFactor) + units;
    }
    else
    {
        valueString = String (newValue * textBoxScaleFactor, numDecimalPlaces);
    }

    textBox.setText (valueString);
}

void SAFESlider::setDefaultValue (double newValue)
{
    defaultValue = newValue;
}

double SAFESlider::getValue()
{
    return slider.getValue();
}

void SAFESlider::setRange (double newMinimum, double newMaximum, double newInterval)
{
    slider.setRange (newMinimum, newMaximum, newInterval);
}

void SAFESlider::setSkewFactor (double factor)
{
    slider.setSkewFactor (factor);
}

void SAFESlider::setTextValueSuffix (const String &suffix)
{
    units = suffix;
}

void SAFESlider::addListener (Slider::Listener* listener)
{
    slider.addListener (listener);
}

bool SAFESlider::isMouseOverOrDraggingSlider()
{
    return slider.isMouseOverOrDragging();
}

//==========================================================================
//      Get a Pointer to the Slider Itself
//==========================================================================
const Slider* SAFESlider::getSliderPointer() const
{
    return &slider;
}

//==========================================================================
//      Set the Label Text
//==========================================================================
void SAFESlider::setText (String newText)
{
    label.setText (newText, dontSendNotification);
}

//==========================================================================
//      Slider Moved
//==========================================================================
void SAFESlider::sliderValueChanged (Slider* /*movedSlider*/)
{
    double sliderValue = slider.getValue() * textBoxScaleFactor;

    String valueString;

    if (units != String::empty && useSIPrefixes)
    {
        valueString = makeSIValueString (sliderValue) + units;
    }
    else
    {
        valueString = String (sliderValue, numDecimalPlaces);
    }

    textBox.setText (valueString);
}

//==========================================================================
//      Reset
//==========================================================================
void SAFESlider::reset()
{
    slider.setValue (defaultValue);
}

//==========================================================================
//      Resize
//==========================================================================
void SAFESlider::resized()
{
    int width = getWidth();
    
    int sliderX = (width - sliderWidth) / 2;
    int sliderY = 0;

    slider.setBounds (sliderX, sliderY, sliderWidth, sliderHeight);
    label.setBounds (sliderX, sliderY, sliderWidth, sliderHeight);

    int textBoxX = (width - textBoxWidth) / 2;
    int textBoxY = (int) (sliderY + sliderHeight * 0.9);

    textBox.setBounds (textBoxX, textBoxY, textBoxWidth, textBoxHeight);
}

//==========================================================================
//      Text Box Settings
//==========================================================================
void SAFESlider::setNumDecimalPlaces (int newNumDecimalPlaces)
{
    numDecimalPlaces = newNumDecimalPlaces;
}

void SAFESlider::setTextBoxScaleFactor (double newScaleFactor)
{
    textBoxScaleFactor = newScaleFactor;
}

//==========================================================================
//      Make the Value String Look Pretty
//==========================================================================
void SAFESlider::setUseSIPrefixes (bool newValue)
{
    useSIPrefixes = newValue;
}

String SAFESlider::makeSIValueString (double value)
{
    int exponent = (int) (floor (log10 (fabs (value))));

    String prefix;

    if (exponent > -6 && exponent <= -3)
    {
        prefix = "m";
        value = value * 1000.0;
    }
    else if (exponent > -3 && exponent < 3)
    {
        prefix = "";
    }
    else if (exponent >= 3 && exponent < 6)
    {
        prefix = "k";
        value = value / 1000.0;
    }
    else if (exponent >= 6 && exponent < 9)
    {
        prefix = "M";
        value = value / 1000000.0;
    }

    return String (value, numDecimalPlaces) + prefix;
}
