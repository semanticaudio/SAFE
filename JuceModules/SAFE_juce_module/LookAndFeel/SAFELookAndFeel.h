#ifndef __SAFELOOKANDFEEL__
#define __SAFELOOKANDFEEL__

/**
 *  An extension of the JUCE LookAndFeel for use with the SAFE plug-ins.
 */
class SAFELookAndFeel : public LookAndFeel_V3
{
public:
    //==========================================================================
    //      Constructor and Destructor
    //==========================================================================
    /** Create a new look and feel. */
    SAFELookAndFeel();

    /** Destructor */
    ~SAFELookAndFeel();

    //==========================================================================
    //      Draw Knob
    //==========================================================================
    /** Reimplementation of the slider drawing function. */
    void drawRotarySlider (Graphics &g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, Slider &slider);

    //==========================================================================
    //      Draw Text Editor
    //==========================================================================
    /** Reimplementation of the text editor filling function. */
    void fillTextEditorBackground (Graphics &g, int width, int height, TextEditor &textEditor);

    /** Reimplementation of the text editor outline drawing function. */
    void drawTextEditorOutline (Graphics &g, int width, int height, TextEditor &textEditor);

private:
    Image greenSliderImage, redSliderImage, yellowSliderImage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SAFELookAndFeel)
};

#endif // __SAFELOOKANDFEEL__
