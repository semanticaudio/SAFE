#ifndef __SAFEBUTTON__
#define __SAFEBUTTON__

class SAFEButton : public Button
{
public:
    //==========================================================================
    //      Constructor and Destructor
    //==========================================================================
    SAFEButton(const String& buttonName);
    ~SAFEButton();

    //==========================================================================
    //      Mode Enumeration
    //==========================================================================
    enum ButtonMode
    {
        Record,
        Recording,
        Save,
        Load,
        MetaData,
        LocalFile,
        GlobalFile
    };

    //==========================================================================
    //      Paint
    //==========================================================================
    void paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown);

    //==========================================================================
    //      Change Mode
    //==========================================================================
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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SAFEButton)
};

#endif // __SAFEBUTTON__
