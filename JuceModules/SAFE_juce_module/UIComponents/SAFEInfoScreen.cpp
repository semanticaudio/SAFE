//==========================================================================
//      Component with all the Info
//==========================================================================
//==========================================================================
//      Constructor and Destructor
//==========================================================================
SAFEInfo::SAFEInfo()
{
    setSize (330, 1686);

    currentImage = ImageCache::getFromMemory (SAFEImages::infopanecurrent_png, SAFEImages::infopanecurrent_pngSize);
    updateImage = ImageCache::getFromMemory (SAFEImages::infopaneupdate_png, SAFEImages::infopaneupdate_pngSize);
}

SAFEInfo::~SAFEInfo()
{
}

//==========================================================================
//      Paint the Thing
//==========================================================================
void SAFEInfo::paint (Graphics& g)
{
    Image imageToDraw;

    if (JucePlugin_Version < version)
    {
        imageToDraw = updateImage;
    }
    else
    {
        imageToDraw = currentImage;
    }

    int width = getWidth();
    int height = getHeight();

    int imageWidth = imageToDraw.getWidth();
    int imageHeight = imageToDraw.getHeight();

    g.drawImage (imageToDraw, 0, 0, width, height, 0, 0, imageWidth, imageHeight);
}

//==========================================================================
//      Set Version Number
//==========================================================================
void SAFEInfo::setMostRecentVersion (float versionNumber)
{
    version = versionNumber;
    repaint();
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
