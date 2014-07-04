//==========================================================================
//      Constructor and Destructor
//==========================================================================
SAFEDescriptorLoadScreen::SAFEDescriptorLoadScreen()
    : titleLabel ("", "Available Descriptors"),
      refreshButton ("Refresh List"),
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
    searchBox.setBounds (20, 55, 315, 25);
    searchBox.setColour (TextEditor::backgroundColourId, SAFEColours::textEditorGrey);
    searchBox.addListener (this);
    searchBox.addKeyListener (this);

    // add the search button
    addAndMakeVisible (&refreshButton);
    refreshButton.setMode (SAFEButton::Refresh);
    refreshButton.setBounds (345, 55, 25, 25);
    refreshButton.addListener (this);

    descriptorBox.setModel (this);
    addAndMakeVisible (&descriptorBox);
    descriptorBox.setColour (ListBox::backgroundColourId, SAFEColours::textEditorGrey);
    descriptorBox.setBounds (20, 90, 350, 160);
    descriptorBox.addKeyListener (this);

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

void SAFEDescriptorLoadScreen::listBoxItemDoubleClicked (int row, const MouseEvent& e)
{
    loadButton.triggerClick();
}

//==========================================================================
//      Get Descriptors
//==========================================================================
void SAFEDescriptorLoadScreen::updateDescriptors (bool fromServer, const XmlElement* localSemanticDataElement)
{
    getDataFromServer = fromServer;
    localSemanticData = localSemanticDataElement;

    allDescriptors.clear();

    if (fromServer)
    {
        URL descriptorURL ("http://193.60.133.151/SAFE/getDescriptors.php");
        descriptorURL = descriptorURL.withParameter ("PluginName", JucePlugin_Name);

        String loadableDescriptors = descriptorURL.readEntireTextStream();
        loadableDescriptors = loadableDescriptors.removeCharacters ("()[]{}<>");
        allDescriptors.addTokens (loadableDescriptors, true);
    }
    else if (localSemanticDataElement)
    {
        forEachXmlChildElement (*localSemanticDataElement, entry)
        {
            for (int i = 0; i < entry->getNumAttributes(); ++i)
            {
                allDescriptors.add (entry->getStringAttribute (String ("Descriptor") + String (i)));
            }
        }
    }
    
    allDescriptors.removeEmptyStrings();
    allDescriptors.removeDuplicates (true);
    allDescriptors.sort (true);

    searchedDescriptors = allDescriptors;

    searchBox.clear();

    descriptorBox.updateContent();
}

String SAFEDescriptorLoadScreen::getSelectedDescriptor()
{
    int selectedRow = descriptorBox.getSelectedRow();
    return searchedDescriptors [selectedRow];
}

//==========================================================================
//      Descriptor Search
//==========================================================================
void SAFEDescriptorLoadScreen::buttonClicked (Button *buttonThatWasClicked)
{
    if (buttonThatWasClicked == &refreshButton)
    {
        updateDescriptors (getDataFromServer, localSemanticData);
    }
}

void SAFEDescriptorLoadScreen::textEditorTextChanged (TextEditor&)
{
    searchDescriptors();
}

void SAFEDescriptorLoadScreen::textEditorReturnKeyPressed (TextEditor&)
{
    if (descriptorBox.getNumSelectedRows() != 0)
    {
        loadButton.triggerClick();
    }
    else if (searchedDescriptors.size() == 1)
    {
        descriptorBox.selectRow (0);
        loadButton.triggerClick();
    }
}

//==========================================================================
//      Key Listener Stuff
//==========================================================================
bool SAFEDescriptorLoadScreen::keyPressed (const KeyPress &key, Component *originatingComponent)
{
    if (key.isKeyCode (KeyPress::downKey) || key.isKeyCode (KeyPress::upKey))
    {
        descriptorBox.keyPressed (key);
        return true;
    }
    else
    {
        return false;
    }
}

//==========================================================================
//      Descriptor Search
//==========================================================================
void SAFEDescriptorLoadScreen::searchDescriptors()
{
    String searchTerm = searchBox.getText();

    if (searchTerm == previousSearchTerm)
    {
        return;
    }

    StringArray descriptorsToSearch;

    if (searchTerm.startsWithIgnoreCase (previousSearchTerm))
    {
        descriptorsToSearch = searchedDescriptors;
    }
    else
    {
        descriptorsToSearch = allDescriptors;
    }

    searchedDescriptors.clear();

    int lowerBound = 0;
    int upperBound = descriptorsToSearch.size() - 1;
    int firstMatchIndex = -1;

    while (lowerBound <= upperBound)
    {
        int testIndex = (upperBound + lowerBound) / 2;
        String testString = descriptorsToSearch [testIndex];
        int testResult = searchTerm.compareIgnoreCase (testString);
        
        if (testString.startsWithIgnoreCase (searchTerm))
        {
            firstMatchIndex = testIndex;
            break;
        }
        else if (testResult < 0)
        {
            upperBound = testIndex - 1;
        }
        else if (testResult > 0)
        {
            lowerBound = testIndex + 1;
        }
    }

    if (firstMatchIndex >= 0)
    {
        bool searchUp = true, searchDown = true;
        int lowerResult = firstMatchIndex;
        int upperResult = firstMatchIndex;

        while (searchUp || searchDown)
        {
            if (searchDown)
            {
                if (lowerResult > 0)
                {
                    String testString = descriptorsToSearch [--lowerResult];

                    if (! testString.startsWithIgnoreCase (searchTerm))
                    {
                        searchDown = false;
                        ++lowerResult;
                    }
                }
                else
                {
                    searchDown = false;
                }
            }

            if (searchUp)
            {
                if (++upperResult < descriptorsToSearch.size())
                {
                    String testString = descriptorsToSearch [upperResult];

                    if (! testString.startsWithIgnoreCase (searchTerm))
                    {
                        searchUp = false;
                    }
                }
                else
                {
                    searchUp = false;
                }
            }
        }

        searchedDescriptors.addArray (descriptorsToSearch, lowerResult, upperResult - lowerResult);
    }

    descriptorBox.updateContent();
    descriptorBox.repaint();
    previousSearchTerm = searchTerm;
}
