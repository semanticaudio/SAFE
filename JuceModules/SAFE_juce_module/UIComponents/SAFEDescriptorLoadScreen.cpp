//==========================================================================
//      Constructor and Destructor
//==========================================================================
SAFEDescriptorLoadScreen::SAFEDescriptorLoadScreen()
    : titleLabel ("", "Available Descriptors"),
      searchButton ("Search"),
      closeButton ("Close"),
      loadButton ("Load")
{
    setSize (390, 295);

    // add the main title
    addAndMakeVisible (&titleLabel);
    titleLabel.setFont (Font (22));
    titleLabel.setBounds (15, 15, 360, 25);
    titleLabel.setJustificationType (Justification::centred);

    // add the search text box
    addAndMakeVisible (&searchBox);
    searchBox.setBounds (20, 55, 310, 25);
    searchBox.setColour (TextEditor::backgroundColourId, SAFEColours::textEditorGrey);

    // add the search button
    addAndMakeVisible (&searchButton);
    searchButton.setBounds (345, 55, 25, 25);
    searchButton.addListener (this);

    updateDescriptors();
    descriptorBox.setModel (this);
    addAndMakeVisible (&descriptorBox);
    descriptorBox.setBounds (20, 90, 350, 160);

    addAndMakeVisible (&closeButton);
    closeButton.setBounds (20, 260, 170, 25);

    addAndMakeVisible (&loadButton);
    loadButton.setBounds (200, 260, 170, 25);
}

SAFEDescriptorLoadScreen::~SAFEDescriptorLoadScreen()
{
}

//==========================================================================
//      Paint the Thing
//==========================================================================
void SAFEDescriptorLoadScreen::paint (Graphics& g)
{
    int width = getWidth();
    int height = getHeight();

    g.setColour (SAFEColours::metaDataGrey);
    g.fillRoundedRectangle (2.0f, 2.0f, (float) (width - 4.0f), (float) (height - 4.0f), 20.0f);

    g.setColour (Colour (0xff000000));
    g.drawRoundedRectangle (2.0f, 2.0f, (float) (width - 4.0f), (float) height - 4.0f, 20.0f, 4.0f);
}

//==========================================================================
//      List Box Model Stuff
//==========================================================================
int SAFEDescriptorLoadScreen::getNumRows()
{
    return searchedDescriptors.size();
}

void SAFEDescriptorLoadScreen::paintListBoxItem (int rowNumber, Graphics &g, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
    {
        g.fillAll (Colours::lightblue);
    }
    else
    {
        g.fillAll (SAFEColours::textEditorGrey);
    }

    g.setColour (Colours::black);
    g.setFont (height * 0.7f);

    g.drawText (searchedDescriptors [rowNumber], 5, 0, width, height, Justification::centredLeft, true);
}

//==========================================================================
//      Descriptor Search
//==========================================================================
void SAFEDescriptorLoadScreen::buttonClicked (Button *buttonThatWasClicked)
{
    if (buttonThatWasClicked == &searchButton)
    {
    }
}

//==========================================================================
//      Get Descriptors
//==========================================================================
void SAFEDescriptorLoadScreen::updateDescriptors()
{
    URL descriptorURL ("http://193.60.133.151/SAFE/getDescriptors.php");
    descriptorURL = descriptorURL.withParameter ("PluginName", JucePlugin_Name);
    
    String loadableDescriptors = descriptorURL.readEntireTextStream();
    allDescriptors.addTokens (loadableDescriptors, true);
    allDescriptors.removeEmptyStrings();
    allDescriptors.removeDuplicates (true);
    allDescriptors.sort (true);

    searchedDescriptors = allDescriptors;
}
