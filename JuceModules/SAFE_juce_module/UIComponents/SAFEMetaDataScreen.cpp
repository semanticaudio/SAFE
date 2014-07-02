//==========================================================================
//      Constructor and Destructor
//==========================================================================
SAFEMetaDataScreen::SAFEMetaDataScreen()
    : submitButton ("Submit"),
      titleLabel ("", "Additional Information"),
      genreLabel ("", "Genre:"),
      instrumentLabel ("", "Musical Instrument:"),
      locationLabel ("", "Current Location:"),
      languageLabel ("", "Primary Language:"),
      experienceLabel ("", "Production Experience:"),
      ageLabel ("", "Age")
{
    setSize (390, 295);

    // add the main title
    addAndMakeVisible (&titleLabel);
    titleLabel.setFont (Font (22));
    titleLabel.setBounds (15, 15, 360, 25);
    titleLabel.setJustificationType (Justification::centred);

    // add the genre text box
    addAndMakeVisible (&genreLabel);
    genreLabel.setBounds (15, 55, 150, 25);
    genreLabel.setJustificationType (Justification::right);
    addAndMakeVisible (&genreBox);
    genreBox.setBounds (180, 55, 150, 25);
    genreBox.setColour (TextEditor::backgroundColourId, SAFEColours::textEditorGrey);
    genreBox.setExplicitFocusOrder (1);
    genreBox.setInputRestrictions (256);

    // add the instrument text box
    addAndMakeVisible (&instrumentLabel);
    instrumentLabel.setBounds (15, 95, 150, 25);
    instrumentLabel.setJustificationType (Justification::right);
    addAndMakeVisible (&instrumentBox);
    instrumentBox.setBounds (180, 95, 150, 25);
    instrumentBox.setColour (TextEditor::backgroundColourId, SAFEColours::textEditorGrey);
    instrumentBox.setExplicitFocusOrder (2);
    instrumentBox.setInputRestrictions (256);

    // add the location text box
    addAndMakeVisible (&locationLabel);
    locationLabel.setBounds (15, 135, 150, 25);
    locationLabel.setJustificationType (Justification::right);
    addAndMakeVisible (&locationBox);
    locationBox.setBounds (180, 135, 150, 25);
    locationBox.setColour (TextEditor::backgroundColourId, SAFEColours::textEditorGrey);
    locationBox.setExplicitFocusOrder (3);
    locationBox.setInputRestrictions (256);

    // add the language text box
    addAndMakeVisible (&languageLabel);
    languageLabel.setBounds (15, 175, 150, 25);
    languageLabel.setJustificationType (Justification::right);
    addAndMakeVisible (&languageBox);
    languageBox.setBounds (180, 175, 150, 25);
    languageBox.setColour (TextEditor::backgroundColourId, SAFEColours::textEditorGrey);
    languageBox.setExplicitFocusOrder (4);
    languageBox.setInputRestrictions (256);

    // add the experience text box
    addAndMakeVisible (&experienceLabel);
    experienceLabel.setBounds (15, 215, 150, 25);
    experienceLabel.setJustificationType (Justification::right);
    addAndMakeVisible (&experienceBox);
    experienceBox.setBounds (180, 215, 150, 25);
    experienceBox.setColour (TextEditor::backgroundColourId, SAFEColours::textEditorGrey);
    experienceBox.setExplicitFocusOrder (5);
    experienceBox.setInputRestrictions (256);

    // add the age text box
    addAndMakeVisible (&ageLabel);
    ageLabel.setBounds (15, 255, 150, 25);
    ageLabel.setJustificationType (Justification::right);
    addAndMakeVisible (&ageBox);
    ageBox.setBounds (180, 255, 150, 25);
    ageBox.setColour (TextEditor::backgroundColourId, SAFEColours::textEditorGrey);
    ageBox.setExplicitFocusOrder (6);
    ageBox.setInputRestrictions (256);

    // add the submit button
    addAndMakeVisible (&submitButton);
    submitButton.setMode (SAFEButton::MetaData);
    submitButton.setBounds (345, 250, 30, 30);
    submitButton.setExplicitFocusOrder (7);
}

SAFEMetaDataScreen::~SAFEMetaDataScreen()
{
}

//==========================================================================
//      Paint the Thing
//==========================================================================
void SAFEMetaDataScreen::paint (Graphics& g)
{
    int width = getWidth();
    int height = getHeight();

    g.setColour (SAFEColours::metaDataGrey);
    g.fillRoundedRectangle (2.0f, 2.0f, (float) (width - 4.0f), (float) (height - 4.0f), 20.0f);

    g.setColour (Colour (0xff000000));
    g.drawRoundedRectangle (2.0f, 2.0f, (float) (width - 4.0f), (float) height - 4.0f, 20.0f, 4.0f);
}

//==========================================================================
//      Get the Data
//==========================================================================
SAFEMetaData SAFEMetaDataScreen::getMetaData()
{
    SAFEMetaData metaData;

    // put text box content in a nice neat struct
    metaData.genre = genreBox.getText();
    metaData.instrument = instrumentBox.getText();
    metaData.experience = experienceBox.getText();
    metaData.location = locationBox.getText();
    metaData.age = ageBox.getText();
    metaData.language = languageBox.getText();

    return metaData;
}

//==========================================================================
//      Set the Data
//==========================================================================
void SAFEMetaDataScreen::setMetaData (const SAFEMetaData& metaData)
{
    genreBox.setText (metaData.genre);
    instrumentBox.setText (metaData.instrument);
    experienceBox.setText (metaData.experience);
    locationBox.setText (metaData.location);
    ageBox.setText (metaData.age);
    languageBox.setText (metaData.language);
}
