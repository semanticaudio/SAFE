//==========================================================================
//      Constructor and Destructor
//==========================================================================
SAFEAudioProcessorEditor::SAFEAudioProcessorEditor (SAFEAudioProcessor* ownerFilter)
    : AudioProcessorEditor (ownerFilter),
      recordButton ("Record"),
      loadButton ("Load"),
      metaDataButton ("Meta Data"),
      fileAccessButton ("File Access"),
      parameters (ownerFilter->getParameterArray()),
      versionUpdateLabel ("", "This plug-in is out of date!\n\n"
                              "A newer version is available to download at\n"
                              "www.semanticaudio.co.uk")
{
    // set it to use our look and feel
    lookAndFeel = new SAFELookAndFeel;
    setLookAndFeel (lookAndFeel);

    // options for descriptor box
    descriptorBox.setWantsKeyboardFocus (true);
    descriptorBox.setMultiLine (true);
    descriptorBox.setColour (TextEditor::backgroundColourId, SAFEColours::textEditorGrey);
    descriptorBox.setInputRestrictions (256);

    recordButton.setMode (SAFEButton::Save);
    recordButton.addListener (this);
    
    if (ownerFilter->isRecording())
    {
        recordButton.setMode (SAFEButton::Recording);
        recordButton.setEnabled (false);
    }

    loadButton.setMode (SAFEButton::Load);
    loadButton.addListener (this);
    
    numParameters = parameters.size();

    // get parameter ranges and stuff from the plugin
    for (int n = 0; n < numParameters; ++n)
    {
        SAFESlider* currentSlider = sliders.add (new SAFESlider);
        
        SAFEParameter* currentParameter = parameters [n];
        float currentMinValue = currentParameter->getMinValue();
        float currentMaxValue = currentParameter->getMaxValue();
        float currentDefaultValue = currentParameter->getDefaultValue();
        float currentSkewFactor = currentParameter->getSkewFactor();
        String currentUnits = currentParameter->getUnits();
        float currentUIScaleFactor = currentParameter->getUIScaleFactor();

        currentSlider->setRange (currentMinValue, currentMaxValue, 0.01);
        currentSlider->setDefaultValue (currentDefaultValue);
        currentSlider->setSkewFactor (currentSkewFactor);
        currentSlider->setTextValueSuffix (currentUnits);
        currentSlider->setTextBoxScaleFactor (currentUIScaleFactor);
        currentSlider->addListener (this);
    }

    // stuff for the meta data screen
    metaDataXPos = -390;
    metaDataYPos = 0;
    addAndMakeVisible (&metaDataScreen);
    metaDataScreen.setBounds (metaDataXPos, metaDataYPos, 390, 295);
    metaDataScreen.setAlwaysOnTop (true);
    metaDataScreen.setEnabled (false);
    metaDataScreen.submitButton.addListener (this);

    metaDataButton.setMode (SAFEButton::MetaData);
    metaDataButton.addListener (this);

    File documentsDirectory (File::getSpecialLocation (File::userDocumentsDirectory));

    File dataDirectory (documentsDirectory.getChildFile ("SAFEPluginData"));
    
    metaDataFile = dataDirectory.getChildFile ("SAFEMetaData.xml");

    if (metaDataFile.exists())
    {
        XmlDocument metaDataDocument (metaDataFile);
        metaDataElement = metaDataDocument.getDocumentElement();

        SAFEMetaData metaData;
        metaData.location = metaDataElement->getStringAttribute ("Location", "");
        metaData.experience = metaDataElement->getStringAttribute ("Experience", "");
        metaData.age = metaDataElement->getStringAttribute ("Age", "");
        metaData.language = metaDataElement->getStringAttribute ("Language", "");

        metaDataScreen.setMetaData (metaData);
    }
    else
    {
        metaDataElement = new XmlElement ("MetaData");
    }

    // file access button settings
    if (canReachServer())
    {
        fileAccessButton.setMode (SAFEButton::GlobalFile);
        fileAccessButtonPressed = true;
    }
    else
    {
        fileAccessButton.setMode (SAFEButton::LocalFile);
        fileAccessButtonPressed = false;
    }

    fileAccessButton.addListener (this);

    availableDescriptorList.setTextWhenNothingSelected ("Available Descriptors");
    availableDescriptorList.setColour (ComboBox::backgroundColourId, SAFEColours::textEditorGrey);
    availableDescriptorList.addListener (this);
    availableDescriptorList.setColour (ComboBox::arrowColourId, Colours::black);
    updateAvailableDescriptorList();

    warningVisible = false;
    warningFlagged = false;
    flaggedWarningID = NoWarning;

    addChildComponent (&versionUpdateLabel);
    versionUpdateLabel.setColour (Label::backgroundColourId, SAFEColours::red);
    versionUpdateLabel.setFont (Font (18));
    versionUpdateLabel.setJustificationType (Justification::centred);

    URL versionCheck ("http://193.60.133.151/SAFE/mostRecentVersion.txt");
    float mostRecentVersion = versionCheck.readEntireTextStream().getFloatValue();
    versionUpdateLabel.setVisible (mostRecentVersion > JucePlugin_Version);

    // start timer to update sliders
    startTimer (parameterUpdateTimer, 100);
    startTimer (meterTimer, 50);
}

SAFEAudioProcessorEditor::~SAFEAudioProcessorEditor()
{
    SAFEMetaData metaData = metaDataScreen.getMetaData();

    metaDataElement->setAttribute ("Location", metaData.location);
    metaDataElement->setAttribute ("Experience", metaData.experience);
    metaDataElement->setAttribute ("Age", metaData.age);
    metaDataElement->setAttribute ("Language", metaData.language);

    metaDataElement->writeToFile (metaDataFile, "");
}

//==========================================================================
//      User Interaction Listeners
//==========================================================================
void SAFEAudioProcessorEditor::buttonClicked (Button* button)
{
    SAFEAudioProcessor* ourProcessor = getProcessor();
    
    String descriptorBoxContent = descriptorBox.getText();
    
    SAFEMetaData metaData = metaDataScreen.getMetaData();
    
    // record / save button
    if (button == &recordButton)
    {
        if (descriptorBoxContent.containsNonWhitespaceChars())
        {
            if (fileAccessButtonPressed && ! canReachServer())
            {
                displayWarning (CannotReachServer);
                return;
            }

            if (ourProcessor->isPlaying())
            {
                ourProcessor->startRecording (descriptorBoxContent, metaData, fileAccessButtonPressed);
                recordButton.setEnabled (false);
                recordButton.setMode (SAFEButton::Recording);
            }
            else
            {
                displayWarning (AudioNotPlaying);
            }
        }
        else
        {
            displayWarning (DescriptorBoxEmpty);
        }
    }
    // load button
    else if (button == &loadButton) 
    {
        if (fileAccessButtonPressed && ! canReachServer())
        {
            displayWarning (CannotReachServer);
            return;
        }

        if (! (ourProcessor->isRecording()))
        {
            if (descriptorBoxContent.containsNonWhitespaceChars())
            {
                // select param settings from either local/global db...
                WarningID warning;
                
                if (fileAccessButtonPressed) //select between the local of global file based on usr:global/local button
                {
                    warning = ourProcessor->getServerData (descriptorBoxContent);
                }
                else
                {
                    warning = ourProcessor->loadSemanticData (descriptorBoxContent);
                }
                
                // load the descriptor and warn if it's not found...
                if (warning != NoWarning)
                {
                    displayWarning (warning);
                }
            }
            else
            {
                displayWarning (DescriptorBoxEmpty);
            }
        }
        else
        {
            displayWarning (LoadingDisabled);
        }
    }
    // meta data button
    else if (button == &metaDataButton)
    {
        // disable all the components
        int numComponents = getNumChildComponents();

        for (int component = 0; component < numComponents; ++component)
        {
            getChildComponent (component)->setEnabled (false);
        }

        metaDataScreen.setEnabled (true);

        // animate the meta data screen
        Rectangle <int> metaDataPosition = metaDataScreen.getBoundsInParent();
        metaDataPosition.setX (metaDataXPos);

        animator.animateComponent (&metaDataScreen, metaDataPosition, 0xff, 1000, false, 0, 0);
    }
    // submit button
    else if (button == &metaDataScreen.submitButton)
    {
        // enable all the components
        int numComponents = getNumChildComponents();

        for (int component = 0; component < numComponents; ++component)
        {
            getChildComponent (component)->setEnabled (true);
        }

        metaDataScreen.setEnabled (false);

        // animate the meta data screen
        Rectangle <int> metaDataPosition = metaDataScreen.getBoundsInParent();
        metaDataPosition.setX (-390);

        animator.animateComponent (&metaDataScreen, metaDataPosition, 0xff, 1000, false, 0, 0);
    }
    // file access button
    else if (button == &fileAccessButton)
    {
        if (fileAccessButtonPressed)
        {
            fileAccessButton.setMode (SAFEButton::LocalFile);
            fileAccessButtonPressed = false;
        }
        else
        {
            fileAccessButton.setMode (SAFEButton::GlobalFile);
            fileAccessButtonPressed = true;
        }

        updateAvailableDescriptorList();
    }
}

void SAFEAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
    // set the plugin's parameters
    SAFEAudioProcessor* ourProcessor = getProcessor();

    for (int n = 0; n < numParameters; ++n)
    {
        if (slider == sliders [n]->getSliderPointer())
        {
            ourProcessor->setScaledParameterNotifyingHost (n, (float) sliders [n]->getValue());
        }
    }

    sliderUpdate (slider);
}

void SAFEAudioProcessorEditor::comboBoxChanged (ComboBox *comboBoxThatChanged)
{
    SAFEAudioProcessor* ourProcessor = getProcessor();

    if (fileAccessButtonPressed && ! canReachServer())
    {
        displayWarning (CannotReachServer);
        return;
    }

    if (! (ourProcessor->isRecording()))
    {
        // select param settings from either local/global db...
        WarningID warning;
        
        if (fileAccessButtonPressed) //select between the local of global file based on usr:global/local button
        {
            warning = ourProcessor->getServerData (comboBoxThatChanged->getText());
        }
        else
        {
            warning = ourProcessor->loadSemanticData (comboBoxThatChanged->getText());
        }
        
        // load the descriptor and warn if it's not found...
        if (warning != NoWarning)
        {
            displayWarning (warning);
        }
    }
    else
    {
        displayWarning (LoadingDisabled);
    }
}

//==========================================================================
//      Timers
//==========================================================================
void SAFEAudioProcessorEditor::timerCallback (int timerID)
{
    if (timerID == parameterUpdateTimer)
    {
        // update sliders to new values
        SAFEAudioProcessor* ourProcessor = getProcessor();

        if (ourProcessor->isReadyToSave())
        {
            recordButton.setMode (SAFEButton::Save);

            if (! isTimerRunning (warningTimer))
            {
                recordButton.setEnabled (true);
            }
        }

        if (warningFlagged)
        {
            displayWarning (flaggedWarningID, 1000);
            warningFlagged = false;
        }

        for (int n = 0; n < numParameters; ++n)
        {
            sliders [n]->setValue (ourProcessor->getScaledParameter (n), dontSendNotification);
        }

        updateUI();
    }
    else if (timerID == warningTimer)
    {
        // show warnings
        if (warningVisible)
        {
            warningVisible = false;
        }
        else
        {
            stopTimer (warningTimer);
            descriptorBox.setColour (TextEditor::textColourId, Colours::black);
            descriptorBox.setText (savedDescriptorBoxContent);
            descriptorBox.setReadOnly (false);

            recordButton.setEnabled (true);
            loadButton.setEnabled (true);
        }
    }
    else if (timerID == meterTimer)
    {
        updateMeters();
    }
}

//==========================================================================
//      Externally Flag Warning
//==========================================================================
void SAFEAudioProcessorEditor::flagWarning (WarningID id)
{
    warningFlagged = true;
    flaggedWarningID = id;
}

//==========================================================================
//      Set Meta Data Screen Position
//==========================================================================
void SAFEAudioProcessorEditor::setMetaDataScreenPosition (int x, int y)
{
    metaDataXPos = x;
    metaDataYPos = y;
    
    Rectangle <int> metaDataPosition = metaDataScreen.getBoundsInParent();
    metaDataPosition.setY (metaDataYPos);
    metaDataScreen.setBounds (metaDataPosition);
}

//==========================================================================
//      Show Warning Messages in Text Box
//==========================================================================
void SAFEAudioProcessorEditor::displayWarning (WarningID id, int durationInMilliseconds)
{
    if (! warningVisible)
    {
        String warningMessage;

        switch (id)
        {
            case LoadingDisabled:
                warningMessage = "You cannot load parameters until the current ones have been saved.";
                break;

            case AudioNotPlaying:
                warningMessage = "Audio must be playing and plug-in must be enabled in order to save!";
                break;

            case ParameterChange:
                warningMessage = "Recording stopped because a parameter was changed.";
                break;

            case AnalysisThreadBusy:
                warningMessage = "Too busy at the moment, try again in a bit :D";
                break;

            case AudioNotProcessed:
                warningMessage = "You need to actually process the audio numbskull!";
                break;

            case DescriptorNotOnServer:
                warningMessage = "Descriptor not found on server!";
                break;

            case DescriptorNotInFile:
                warningMessage = "Descriptor not found in local file!";
                break;

            case DescriptorBoxEmpty:
                warningMessage = "You need to write something in the box first fool!";
                break;

            case CannotReachServer:
                warningMessage = "Can't reach the server, check your internet connection";
                break;
        }

        recordButton.setEnabled (false);
        loadButton.setEnabled (false);

        savedDescriptorBoxContent = descriptorBox.getText();
        descriptorBox.setColour (TextEditor::textColourId, Colours::indianred);
        descriptorBox.setText (warningMessage);
        descriptorBox.setReadOnly (true);
        warningVisible = true;
        startTimer (warningTimer, durationInMilliseconds);
    }
}

//==========================================================================
//      Update Available Descriptors
//==========================================================================
void SAFEAudioProcessorEditor::updateAvailableDescriptorList()
{
    StringArray descriptors;

    if (fileAccessButtonPressed)
    {
        URL descriptorURL ("http://193.60.133.151/SAFE/getDescriptors.php");
        descriptorURL = descriptorURL.withParameter ("PluginName", JucePlugin_Name);
        
        String allDescriptors = descriptorURL.readEntireTextStream();
        descriptors.addTokens (allDescriptors, true);
    }
    else
    {
        SAFEAudioProcessor* ourProcessor = getProcessor();
        XmlElement* localSemanticData = ourProcessor->getSemanticDataElement();

        forEachXmlChildElement (*localSemanticData, entry)
        {
            for (int i = 0; i < entry->getNumAttributes(); ++i)
            {
                descriptors.add (entry->getStringAttribute (String ("Descriptor") + String (i)));
            }
        }
    }

    descriptors.removeEmptyStrings();
    descriptors.removeDuplicates (true);
    descriptors.sort (true);
    
    availableDescriptorList.clear (dontSendNotification);
    availableDescriptorList.addItemList (descriptors, 1);
}

//==========================================================================
//      Test Connection to Server
//==========================================================================
bool SAFEAudioProcessorEditor::canReachServer()
{
    URL serverConnectionTest ("http://193.60.133.151/SAFE/testConnection.txt");
    String testString (serverConnectionTest.readEntireTextStream());

    return testString.contains ("Hi There!");
}
