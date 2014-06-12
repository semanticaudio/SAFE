#ifndef __SAFESLIDER__
#define __SAFESLIDER__

class SAFESlider : public Component,
                   public Slider::Listener
{
public:
    //==========================================================================
    //      Constructor and Destructor
    //==========================================================================
    SAFESlider();
    ~SAFESlider();

    //==========================================================================
    //      Set Component Sizes
    //==========================================================================
    void setSliderSize (int width, int height);
    void setTextBoxSize (int width, int height);
    void setColour (Colour newColour);
    void setTextBoxFont (Font newFont);

    //==========================================================================
    //      Slider Forwarding Stuff
    //==========================================================================
    void setValue (double newValue, NotificationType notification);
    double getValue();
    void setDefaultValue (double newValue);
    void setRange (double newMinimum, double newMaximum, double newInterval);
    void setSkewFactor (double factor);
    void setTextValueSuffix (const String &suffix);
    void addListener (Slider::Listener* listener);
    bool isMouseOverOrDraggingSlider();

    //==========================================================================
    //      Get a Pointer to the Slider Itself
    //==========================================================================
    const Slider* getSliderPointer() const;

    //==========================================================================
    //      Set the Label Text
    //==========================================================================
    void setText (String newText);
    
    //==========================================================================
    //      Slider Moved
    //==========================================================================
    void sliderValueChanged (Slider* movedSlider);    

    //==========================================================================
    //      Reset
    //==========================================================================
    void reset();    

    //==========================================================================
    //      Resize
    //==========================================================================
    void resized();

    //==========================================================================
    //      Number of Decimal Places for Text Box Value
    //==========================================================================
    void setUseSIPrefixes (bool newValue);
    void setNumDecimalPlaces (int newNumDecimalPlaces);

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

    bool useSIPrefixes;

    //==========================================================================
    //      Make the Value String Look Pretty
    //==========================================================================
    String makeSIValueString (double value);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SAFESlider)
};

#endif // __SAFESLIDER__
