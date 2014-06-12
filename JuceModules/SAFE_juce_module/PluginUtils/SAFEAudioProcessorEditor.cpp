//==========================================================================
//      Constructor and Destructor
//==========================================================================
SAFEAudioProcessorEditor::SAFEAudioProcessorEditor (SAFEAudioProcessor* ownerFilter)
    : AudioProcessorEditor (ownerFilter),
      recordButton ("Record"),
      loadButton ("Load"),
      metaDataButton ("Meta Data"),
      fileAccessButton ("File Access"),
      parameters (ownerFilter->getParameterArray())
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

        currentSlider->setRange (currentMinValue, currentMaxValue, 0.01);
        currentSlider->setDefaultValue (currentDefaultValue);
        currentSlider->setSkewFactor (currentSkewFactor);
        currentSlider->setTextValueSuffix (currentUnits);
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

    warningVisible = false;
    warningFlagged = false;
    flaggedWarningID = NoWarning;

    // start timer to update sliders
    startTimer (parameterUpdateTimer, 100);
    startTimer (meterTimer, 50);
}

SAFEAudioProcessorEditor::~SAFEAudioProcessorEditor()
{
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
//      Test Connection to Server
//==========================================================================
bool SAFEAudioProcessorEditor::canReachServer()
{
    URL serverConnectionTest ("http://193.60.133.151/SAFE/testConnection.txt");
    String testString (serverConnectionTest.readEntireTextStream());

    return testString.contains ("Hi There!");
}
