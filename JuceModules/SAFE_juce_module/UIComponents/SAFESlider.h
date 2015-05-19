#ifndef __SAFESLIDER__
#define __SAFESLIDER__

/**
 * An extension of the JUCE slider for the SAFE UI.
 *
 */
class SAFESlider : public Component,
                   public Slider::Listener
{
public:
    //==========================================================================
    //      Constructor and Destructor
    //==========================================================================
    /** Creates a SAFE slider. */
    SAFESlider();

    /** Destructor */
    ~SAFESlider();

    //==========================================================================
    //      Set Component Sizes
    //==========================================================================
    /** Sets the size of the slider. 
     *
     *  @param width   the new slider width
     *  @param height  the new slider height
     */
    void setSliderSize (int width, int height);

    /** Sets the size of the text box.
     *
     *  @param width   the new text box width
     *  @param height  the new text box height
     */
    void setTextBoxSize (int width, int height);

    /** Sets the colour of the silder.
     *  
     *  @param newColour  the new slider colour
     *
     *  Currently the slider body can only be one of the following:
     *      SAFEColours::green,
     *      SAFEColours::red,
     *      SAFEColours::yellow
     *
     *  The body will default to SAFEColours::yellow if an different colour is given.
     *  The text box will be the colour passed to this function.
     */
    void setColour (Colour newColour);

    /** Sets the font for the text box 
     *  
     *  @param newFont  the new text box font.
     */
    void setTextBoxFont (Font newFont);

    //==========================================================================
    //      Slider Forwarding Stuff
    //==========================================================================
    /** Changes the slider's current value.
     *
     *  This will trigger a callback to Slider::Listener::sliderValueChanged() for any listeners
     *  that are registered, and will synchronously call the valueChanged()
     *  method in case subclasses want to handle it.
     *
     *  @param newValue         the new value to set - this will be restricted by the
     *                          minimum and maximum range, and will be snapped to the
     *                          nearest interval if one has been set
     *  @param notification     can be one of the NotificationType values, to request
     *                          a synchronous or asynchronous call to the valueChanged() method
     *                          of any Slider::Listeners that are registered.
     */
    void setValue (double newValue, NotificationType notification);

    /** Returns the sliders current value. */
    double getValue();

    /** Sets the default value for the slider. 
     *  
     *  @param newValue  the dew default value for the slider
     *
     *  When the reset button is pressed the slider will be set to the default value. 
     */
    void setDefaultValue (double newValue);


    /** Sets the limits that the slider's value can take.
     *  @param newMinimum   the lowest value allowed
     *  @param newMaximum   the highest value allowed
     *  @param newInterval  the steps in which the value is allowed to increase - if this
     *                      is not zero, the value will always be (newMinimum +
     *                      (newInterval * an integer)).
     */
    void setRange (double newMinimum, double newMaximum, double newInterval);

    /** Sets the skew factor for the slider.
     *
     *  @param factor  the new skew factor
     *
     *  You may want to use a range of values on the slider where more accuracy
     *  is required towards one end of the range, so this will logarithmically
     *  spread the values across the length of the slider.
     *  If the factor is < 1.0, the lower end of the range will fill more of the
     *  slider's length; if the factor is > 1.0, the upper end of the range
     *  will be expanded instead. A factor of 1.0 doesn't skew it at all.
     */
    void setSkewFactor (double factor);

    /** Sets a suffix to append to the end of the numeric value when it's displayed as
      * a string.
      *
      * @param suffix  the new suffix
      */
    void setTextValueSuffix (const String &suffix);

    /** Add a listener to be called when the slider value changes
     *
     *  @param listener  a pointer to the listener object to be added
     */
    void addListener (Slider::Listener* listener);

    /** Detect whether the mouse is currently over or dragging on the slider body. */
    bool isMouseOverOrDraggingSlider();

    //==========================================================================
    //      Get a Pointer to the Slider Itself
    //==========================================================================
    /** Returns a pointer to the slider component inside this component. */
    const Slider* getSliderPointer() const;

    //==========================================================================
    //      Set the Label Text
    //==========================================================================
    /** Sets the text to be displayed on the slider body.
     *  
     *  @param newText  the new slider text
     */
    void setText (String newText);
    
    //==========================================================================
    //      Slider Moved
    //==========================================================================
    /** Callback function called when the slider is moved. */
    void sliderValueChanged (Slider* movedSlider);    

    //==========================================================================
    //      Reset
    //==========================================================================
    /** Reset the slider to its default value.
     *
     *  @see setDefaultValue
     */
    void reset();    

    //==========================================================================
    //      Resize
    //==========================================================================
    /** Function called when the size of the slider is changed. */
    void resized();

    //==========================================================================
    //      Text Box Settings
    //==========================================================================
    /** If set to true the slider will try its best to display its value using SI prefixes
     *
     *  Rather than displaying 1000Hz it would display 1kHz.
     */
    void setUseSIPrefixes (bool newValue);

    /** Set the number of decimal places to be displayed in the sliders text box
     *
     *  @param newNumDecimalPlaces  the number of decimal places to display
     */
    void setNumDecimalPlaces (int newNumDecimalPlaces);

    /** Set a scale factor to be applied to the value shown in the text box
     *
     *  @param newScaleFactor  the new scale factor to be applied
     */
    void setTextBoxScaleFactor (double newScaleFactor);

private:
    Label label;

    double defaultValue;

    Slider slider;
    int sliderWidth, sliderHeight;

    //==========================================================================
    //      Text Box Class
    //==========================================================================
    class SAFESliderTextBox : public Label
    {
    public:
        //==========================================================================
        //      Constructor and Destructor
        //==========================================================================
        SAFESliderTextBox(const String& componentName, const String& labelText);
        ~SAFESliderTextBox();

        //==========================================================================
        //      Mouse Callbacks
        //==========================================================================
        void mouseEnter (const MouseEvent& e);
        void mouseExit (const MouseEvent& e);
        void mouseDown (const MouseEvent& e);

        //==========================================================================
        //      Set Text
        //==========================================================================
        void setText (String newText);

    private:
        bool isChangable;
        String content;
    };

    SAFESliderTextBox textBox;
    int textBoxWidth, textBoxHeight;
    String units;
    int numDecimalPlaces;

    double textBoxScaleFactor;

    bool useSIPrefixes;

    //==========================================================================
    //      Make the Value String Look Pretty
    //==========================================================================
    String makeSIValueString (double value);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SAFESlider)
};

#endif // __SAFESLIDER__
