#ifndef __SAFEAUDIOPROCESSOREDITOR__
#define __SAFEAUDIOPROCESSOREDITOR__

/**
 *  An extension of the AudioProcessorEditor class for use in SAFE plugins.
 */
class SAFEAudioProcessorEditor : public AudioProcessorEditor,
                                 public Button::Listener,
                                 public Slider::Listener,
                                 public MultiTimer
{
public:
    //==========================================================================
    //      Constructor and Destructor
    //==========================================================================
    /** Create a new editor.
     *
     *  @param ownerFilter  a pointer to the processor this editor will control
     */
    SAFEAudioProcessorEditor (SAFEAudioProcessor* ownerFilter);

    /** Destructor */
    virtual ~SAFEAudioProcessorEditor();

    //==========================================================================
    //      User Interaction Listeners
    //==========================================================================
    /** Implementation of function inherited from Button::Listener. */
    void buttonClicked (Button* button);

    /** Implementation of function inherited from Slider::Listener. */
    void sliderValueChanged (Slider* slider);

    /** A function called when a slider has been moved.
     *
     *  As the Slider::Lister callback is already implemented to send parameter
     *  updates to the processor this function will be called whenever a slider is moved.
     *
     *  If you need your editor to do anything when a slider is moved override this 
     *  function and do what you will in its body.
     */
    virtual void sliderUpdate (Slider*) {}

    //==========================================================================
    //      GUI Update Timer
    //==========================================================================
    /** Timer callbacks for UI and metering updates.
     *  
     *  As ever we probably should be inheriting privately from MultiTimer.
     */
    void timerCallback (int timerID);

    /** A function called to update the parameter values in the user interface.
     *
     *  Parameter sliders should update themselves automatically when parameter values are
     *  change through other means.
     *
     *  If your editor should react in any other way to updates in parameter
     *  values override this function and have your way with it.
     */
    virtual void updateUI() {};

    /** A function called to update the meter levels in the user interface.
     *
     *  This function will get called periodically in order for you to update the
     *  values shown on meters in your UI.
     *
     *  If you have level meters override this function and put your meter update 
     *  code in it.
     */
    virtual void updateMeters() {};

    //==========================================================================
    //      Externally Flag Warning
    //==========================================================================
    /** Tell the editor that it should display a warning.
     *
     *  @param id  that id of the warning that should be displayed
     */
    void flagWarning (WarningID id);

protected:
    TextEditor descriptorBox; /**< The text editor descriptors should be written in. */
    SAFEButton recordButton; /**< The button for saving descriptors. */
    SAFEButton loadButton; /**< The button for loading descriptors. */
    SAFEButton metaDataButton; /**< The button for bringing up the meta data dialogue box. */
    SAFEButton fileAccessButton; /**< The button for changing between local and 
                                      global saving. */
    SAFEButton infoButton; /**< The button for displaying the usage instructions. */
    OwnedArray <SAFESlider> sliders; /**< An array containing the parameter sliders.
                                      *   
                                      *   These sliders are automatically linked to
                                      *   the corresponding parameter in the processor.
                                      *   Your job is to position them where ever you
                                      *   see fit in your UI.
                                      */

    //==========================================================================
    //      Set Position of Extra Screens
    //==========================================================================
    /** Set the position at which dialogue boxes should appear on the UI.
     *
     *  @param x  the x position of the top left corner of a dialogue box
     *  @param y  the y position of the top left corner of a dialogue box
     */
    void setExtraScreenPosition (int x, int y);

private:
    enum Timers
    {
        parameterUpdateTimer,
        warningTimer,
        meterTimer
    };

    enum ExtraScreenLocation
    {
        Left,
        Right,
        Top,
        Bottom
    };

    bool warningVisible;
    String savedDescriptorBoxContent;

    bool fileAccessButtonPressed;

    //==========================================================================
    //      Extra Screen Bits
    //==========================================================================
    ComponentAnimator animator;

    SAFEMetaDataScreen metaDataScreen;
    File metaDataFile;
    ScopedPointer <XmlElement> metaDataElement;

    SAFEDescriptorLoadScreen descriptorLoadScreen;

    SAFEInfoScreen infoScreen;

    int extraScreenXPos, extraScreenYPos;
    bool extraScreenVisible;

    void showExtraScreen (SAFEExtraScreen& screenToShow);
    void hideExtraScreen (SAFEExtraScreen& screenToHide, ExtraScreenLocation direction);

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
