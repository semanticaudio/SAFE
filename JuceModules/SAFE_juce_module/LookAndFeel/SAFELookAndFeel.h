#ifndef __SAFELOOKANDFEEL__
#define __SAFELOOKANDFEEL__

class SAFELookAndFeel : public LookAndFeel_V3
{
public:
    //==========================================================================
    //      Constructor and Destructor
    //==========================================================================
    SAFELookAndFeel();
    ~SAFELookAndFeel();

    //==========================================================================
    //      Draw Knob
    //==========================================================================
    void drawRotarySlider (Graphics &g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, Slider &slider);

    //==========================================================================
    //      Draw Text Editor
    //==========================================================================
    void fillTextEditorBackground (Graphics &g, int width, int height, TextEditor &textEditor);
    void drawTextEditorOutline (Graphics &g, int width, int height, TextEditor &textEditor);

private:
    Image greenSliderImage, redSliderImage, yellowSliderImage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SAFELookAndFeel)
};

#endif // __SAFELOOKANDFEEL__
