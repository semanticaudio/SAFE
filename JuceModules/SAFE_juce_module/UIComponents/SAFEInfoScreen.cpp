//==========================================================================
//      Component with all the Info
//==========================================================================
//==========================================================================
//      Constructor and Destructor
//==========================================================================
SAFEInfo::SAFEInfo()
    : metaDataButton ("")
{
    setSize (330, 400);

    addChildComponent (&versionUpdateLabel);
    versionUpdateLabel.setColour (Label::backgroundColourId, SAFEColours::red);
    versionUpdateLabel.setJustificationType (Justification::centred);
    versionUpdateLabel.setBounds (0, 0, 330, 80);

    addAndMakeVisible (&text1);
    text1.setBounds (0, 0, 330, 300);
    text1.setText ("Once you have used this plug-in to get your"
                  " audio sounding how you want it, you can save the plug-in setting."
		          " This is just a simple task of writing some descriptors in the"
                  " text box and hitting save.\n\n"
		          "As part of the saving process the plug-in has to analyse the"
                  " audio you are working on. In order to do this the audio needs"
                  " to be playing when you hit save. Once you hit save the save"
                  " button will notify you that the plug-in is recording the"
                  " features of your audio. During this time you should not change"
                  " any of the plug-in parameters. If you do the plug-in will tell"
                  " you off and cancel the saving process.\n\n"
		          "To aid in the analysis of the data there is also an option to enter"
                  " additional information about the audio you are working on and"
                  " yourself. You can do this by clicking the additional information"
                  " button which looks like this:"
                  , dontSendNotification);

    addAndMakeVisible (&metaDataButton);
    metaDataButton.setMode (SAFEButton::MetaData);
    metaDataButton.setEnabled (false);
    metaDataButton.setBounds (160, 300, 30, 30);

    addAndMakeVisible (&text2);
    text2.setBounds (0, 335, 330, 65);
    text2.setText ("This will present you with a data entry screen which you can use"
                   " to enter extra information.\n\n"
                   , dontSendNotification);
}

SAFEInfo::~SAFEInfo()
{
}

//==========================================================================
//      Paint the Thing
//==========================================================================
void SAFEInfo::paint (Graphics& g)
{
    g.fillAll (SAFEColours::metaDataGrey);
}

//==========================================================================
//      Set Version Number
//==========================================================================
void SAFEInfo::setMostRecentVersion (float versionNumber)
{
    if (JucePlugin_Version < versionNumber)
    {
        setSize (330, 480);

        versionUpdateLabel.setText (String ("A newer version of this plug-in is available.\n"
                                            "This is version ") 
                                    + JucePlugin_VersionString
                                    + String (", the most recent version is ")
                                    + String (versionNumber) + String (".\n")
                                    + String ("Go to www.semanticaudio.co.uk to download the "
                                              "newer version.")
                                    , dontSendNotification);
        versionUpdateLabel.setVisible (true);

        text1.setTopLeftPosition (0, 80);
        metaDataButton.setTopLeftPosition (160, 380);
        text2.setTopLeftPosition (0, 415);
    }
    else
    {
        setSize (330, 400);

        text1.setTopLeftPosition (0, 0);
        metaDataButton.setTopLeftPosition (160, 300);
        text2.setTopLeftPosition (0, 335);
    }
}

//==========================================================================
//      A Sliding Screen to House the Info
//==========================================================================
//==========================================================================
//      Constructor and Destructor
//==========================================================================
SAFEInfoScreen::SAFEInfoScreen()
    : closeButton ("")
{
    setSize (390, 295);

    // add the main title
    addAndMakeVisible (&titleLabel);
    titleLabel.setText ("Instructions", dontSendNotification);

    addAndMakeVisible (&closeButton);
    closeButton.setMode (SAFEButton::Close);
    closeButton.setBounds (350, 15, 25, 25);

    addAndMakeVisible (&viewport);
    viewport.setViewedComponent (&info);
    viewport.setBounds (20, 55, 350, 230);
}

SAFEInfoScreen::~SAFEInfoScreen()
{
}
