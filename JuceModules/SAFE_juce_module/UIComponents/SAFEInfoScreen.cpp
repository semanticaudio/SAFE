//==========================================================================
//      Component with all the Info
//==========================================================================
//==========================================================================
//      Constructor and Destructor
//==========================================================================
SAFEInfo::SAFEInfo()
{
}

SAFEInfo::~SAFEInfo()
{
}

//==========================================================================
//      Paint the Thing
//==========================================================================
void SAFEInfo::paint (Graphics& g)
{
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
    closeButton.setBounds (10, 10, 25, 25);
}

SAFEInfoScreen::~SAFEInfoScreen()
{
}
