#ifndef __SAFEBUTTON__
#define __SAFEBUTTON__

/**
 *  An extension of the JUCE Button for the SAFE UI.
 *
 */
class SAFEButton : public Button
{
public:
    //==========================================================================
    //      Constructor and Destructor
    //==========================================================================
    /** Create a new button.
     *
     *  @param buttonName  the component name
     */
    SAFEButton(const String& buttonName);

    /** Destructor */
    ~SAFEButton();

    //==========================================================================
    //      Mode Enumeration
    //==========================================================================
    /** An enum type for the various button modes. */
    enum ButtonMode
    {
        Record, /**< Start recording button */
        Recording, /**< Button during recording */
        Save, /**< Save button */
        Load, /**< Load button */
        MetaData, /**< Meta data button */
        LocalFile, /**< Local file saving symbol button */
        GlobalFile, /**< Global file saving symbol button */
        Refresh, /**< Refresh button */
        Close, /**< Close button */
        info, /**< Info button */
        infoWarning /**< Red info button */
    };

    //==========================================================================
    //      Paint
    //==========================================================================
    /** Draw the button.
     *
     *  Inherited from Button.
     */
    void paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown);

    //==========================================================================
    //      Change Mode
    //==========================================================================
    /** Change the button mode.
     *
     *  @param newMode  the new button mode
     */
    void setMode (ButtonMode newMode);

private:
    ButtonMode currentMode;

    // images for the various modes and conditions
    Image recordImage, recordMouseOverImage, recordMouseDownImage;
    Image recordingImage;
    Image saveImage, saveMouseOverImage, saveMouseDownImage;
    Image loadImage, loadMouseOverImage, loadMouseDownImage;
    Image metaDataImage, metaDataMouseOverImage, metaDataMouseDownImage;
    Image localFileImage, localFileMouseOverImage, localFileMouseDownImage;
    Image globalFileImage, globalFileMouseOverImage, globalFileMouseDownImage;
    Image infoImage, infoMouseOverImage;
    Image infoWarningImage, infoWarningMouseOverImage;

    void drawRefreshSymbol (Graphics& g);
    void drawCloseSymbol (Graphics& g);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SAFEButton)
};

#endif // __SAFEBUTTON__
