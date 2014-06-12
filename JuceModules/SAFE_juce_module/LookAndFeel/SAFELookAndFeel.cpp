#ifndef IMAGE_DIRECTORY
#define IMAGE_DIRECTORY File::getSpecialLocation (File::currentExecutableFile)
#endif

//==========================================================================
//      Constructor and Destructor
//==========================================================================
SAFELookAndFeel::SAFELookAndFeel()
{
    File imageDirectory (IMAGE_DIRECTORY);

    // load the slider images
//    File greenSliderFile = imageDirectory.getChildFile ("Images/knob_green.png");
//    greenSliderImage = ImageCache::getFromFile (greenSliderFile);
//    File redSliderFile = imageDirectory.getChildFile ("Images/knob_red.png");
//    redSliderImage = ImageCache::getFromFile (redSliderFile);
//    File yellowSliderFile = imageDirectory.getChildFile ("Images/knob_yellow.png");
//    yellowSliderImage = ImageCache::getFromFile (yellowSliderFile);

    greenSliderImage    = ImageCache::getFromMemory(SAFEImages::knob_green_png, SAFEImages::knob_green_pngSize);
    redSliderImage      = ImageCache::getFromMemory(SAFEImages::knob_red_png, SAFEImages::knob_red_pngSize);
    yellowSliderImage   = ImageCache::getFromMemory(SAFEImages::knob_yellow_png, SAFEImages::knob_yellow_pngSize);
}

SAFELookAndFeel::~SAFELookAndFeel()
{
}

//==========================================================================
//      Draw Knob
//==========================================================================
void SAFELookAndFeel::drawRotarySlider (Graphics &g, int /*x*/, int /*y*/, int width, int height, float sliderPosProportional, float /*rotaryStartAngle*/, float /*rotaryEndAngle*/, Slider &slider)
{
    // select the relevant image
    Image imageToDraw;
    Colour sliderColour = slider.findColour (Slider::rotarySliderFillColourId);

    if (sliderColour == SAFEColours::green)
    {
        imageToDraw = greenSliderImage;
    }
    else if (sliderColour == SAFEColours::red)
    {
        imageToDraw = redSliderImage;
    }
    else
    {
        imageToDraw = yellowSliderImage;
    }

    // apply a transform to the image and draw it
    AffineTransform transform;

    int imageWidth = imageToDraw.getWidth();
    int imageHeight = imageToDraw.getHeight();
    
    float widthFactor = (float) width / imageWidth;
    float heightFactor = (float) height / imageHeight;

    float minFactor = 1;
    float xTranslate = 0;
    float yTranslate = 0;

    // if the given area is not square the slider will scale to the shortest dimension
    if (widthFactor >= heightFactor)
    {
        minFactor = heightFactor;
        xTranslate = (width - imageWidth * minFactor) / 2;
    }
    else
    {
        minFactor = widthFactor;
        yTranslate = (height - imageHeight * minFactor) / 2;
    }

    transform = transform.scaled (minFactor);
    transform = transform.translated (xTranslate, yTranslate);

    float sliderRange = 3.0f * float_Pi / 2.0f;
    float sliderAngle = sliderPosProportional * sliderRange - 3.0f * float_Pi / 4.0f;
    transform = transform.rotated (sliderAngle, (float) (width / 2.0f), (float) (height / 2.0f));
    
    g.drawImageTransformed (imageToDraw, transform);
}

//==========================================================================
//      Draw Text Editor
//==========================================================================
void SAFELookAndFeel::fillTextEditorBackground (Graphics &g, int width, int height, TextEditor &textEditor)
{
    g.setColour (textEditor.findColour (TextEditor::backgroundColourId));

    g.fillRect (0, 0, width, height);
}

void SAFELookAndFeel::drawTextEditorOutline (Graphics &g, int width, int height, TextEditor& /*textEditor*/)
{
    g.setColour (Colour (0xff000000));

    g.drawRect (0, 0, width, height, 1);
}
