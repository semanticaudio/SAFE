//==========================================================================
//      Constructor and Destructor
//==========================================================================
SAFEExtraScreen::SAFEExtraScreen()
{
    setSize (390, 295);

    // add the main title
    titleLabel.setFont (Font (22));
    titleLabel.setBounds (15, 15, 360, 25);
    titleLabel.setJustificationType (Justification::centred);
}

SAFEExtraScreen::~SAFEExtraScreen()
{
}

//==========================================================================
//      Paint the Thing
//==========================================================================
void SAFEExtraScreen::paint (Graphics& g)
{
    int width = getWidth();
    int height = getHeight();

    g.setColour (SAFEColours::metaDataGrey);
    g.fillRoundedRectangle (2.0f, 2.0f, (float) (width - 4.0f), (float) (height - 4.0f), 20.0f);

    g.setColour (Colour (0xff000000));
    g.drawRoundedRectangle (2.0f, 2.0f, (float) (width - 4.0f), (float) height - 4.0f, 20.0f, 4.0f);
}
