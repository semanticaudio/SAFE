#ifndef __SAFEAUDIOPROCESSOREDITOR__
#define __SAFEAUDIOPROCESSOREDITOR__

class SAFEAudioProcessorEditor : public AudioProcessorEditor,
                                 public Button::Listener,
                                 public Slider::Listener,
                                 public MultiTimer
{
public:
    //==========================================================================
    //      Constructor and Destructor
    //==========================================================================
    SAFEAudioProcessorEditor (SAFEAudioProcessor* ownerFilter);
    virtual ~SAFEAudioProcessorEditor();

    //==========================================================================
    //      User Interaction Listeners
    //==========================================================================
    void buttonClicked (Button* button);
    void sliderValueChanged (Slider* slider);
    virtual void sliderUpdate (Slider*) {}

    //==========================================================================
    //      GUI Update Timer
    //==========================================================================
    void timerCallback (int timerID);
    virtual void updateUI() {};
    virtual void updateMeters() {};

    //==========================================================================
    //      Externally Flag Warning
    //==========================================================================
    void flagWarning (WarningID id);

protected:
    TextEditor descriptorBox;
    SAFEButton recordButton, loadButton, metaDataButton, fileAccessButton;
    OwnedArray <SAFESlider> sliders;

    //==========================================================================
    //      Set Position of Extra Screens
    //==========================================================================
    void setExtraScreenPosition (int x, int y);

    Label versionUpdateLabel;

private:
    enum Timers
    {
        parameterUpdateTimer,
        warningTimer,
        meterTimer
    };

    bool warningVisible;
    String savedDescriptorBoxContent;

    bool fileAccessButtonPressed;

    ComponentAnimator animator;

    SAFEMetaDataScreen metaDataScreen;
    int metaDataXPos, metaDataYPos;
    File metaDataFile;
    ScopedPointer <XmlElement> metaDataElement;

    SAFEDescriptorLoadScreen descriptorLoadScreen;
    int loadScreenXPos, loadScreenYPos;

    bool extraScreenVisible;

    int numParameters;
    const OwnedArray <SAFEParameter>& parameters;

    ScopedPointer <SAFELookAndFeel> lookAndFeel;

    //==========================================================================
    //      Show Warning Messages in Text Box
    //==========================================================================
    bool warningFlagged;
    WarningID flaggedWarningID;
    void displayWarning (WarningID id, int durationInMilliseconds = 1000);

    //==========================================================================
    //      Test Connection to Server
    //==========================================================================
    bool canReachServer();

    SAFEAudioProcessor* getProcessor()
    {
        return static_cast <SAFEAudioProcessor*> (getAudioProcessor());
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SAFEAudioProcessorEditor);
};

#endif // __SAFEAUDIOPROCESSOREDITOR__
