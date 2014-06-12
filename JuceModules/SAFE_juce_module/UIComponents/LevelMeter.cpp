//==========================================================================
//      Constructor and Destructor
//==========================================================================
LevelMeter::LevelMeter()
    : LevelMeterTimer (1)
{
    setMeterDecayTime (1000);
}

LevelMeter::~LevelMeter()
{
}

//==========================================================================
//      Paint
//==========================================================================
void LevelMeter::paint (Graphics& g)
{
    int height = getHeight();
    int width = getWidth();

    if (height >= width)
    {
        isVertical = true;
    }
    if (height < width)
    {
        isVertical = false;
    }

    g.fillAll (Colours::black);

    int position = (int) gainToPosition ((float) getMeterLevel (0));
    
    g.setColour (Colours::yellow);

    if (isVertical)
    {
        g.fillRect (0, position, width, height - position);
    }
    else
    {
        g.fillRect (0, 0, position, height);
    }
}

//==========================================================================
//      Utility Functions
//==========================================================================
float LevelMeter::positionToGain (float position)
{
    float proportionOfDimension = 0;

    if (isVertical)
    {
        proportionOfDimension = position / getHeight();
    }
    else
    {
        proportionOfDimension = 1 - position / getWidth();
    }

    if (proportionOfDimension <= 0.48f)
    {
        return - 20.0f * proportionOfDimension / 0.48f;
    }
    else if (proportionOfDimension <= 0.84f)
    {
        return - 20.0f - 30.0f * (proportionOfDimension - 0.48f) / (0.84f - 0.48f);
    }
    else if (proportionOfDimension <= 0.96f)
    {
        return - 50.0f - 20.0f * (proportionOfDimension - 0.84f) / (0.96f - 0.84f);
    }
    else
    {
        return - 70.0f - 30.0f * (proportionOfDimension - 0.96f) / (1.0f - 0.96f);
    }
}

float LevelMeter::gainToPosition (float gain)
{
    float proportionOfDimension = 0;

    if (gain >= (- 20.0f))
    {
        proportionOfDimension  = gain * 0.48f / (- 20.0f);
    }
    else if (gain >= - 50.0f)
    {
        proportionOfDimension = (gain + 20.0f) * (0.84f - 0.48f) / (- 30.0f) + 0.48f;
    }
    else if (gain >= - 70.0f)
    {
        proportionOfDimension = (gain + 50.0f) * (0.96f - 0.84f) / (- 20.0f) + 0.84f;
    }
    else
    {
        proportionOfDimension = (gain + 70.0f) * (1.0f - 0.96f) / (- 30.0f) + 0.96f;
    }

    if (isVertical)
    {
        return proportionOfDimension * getHeight();
    }
    else
    {
        return (1 - proportionOfDimension) / getWidth();
    }
}

//==========================================================================
//      Metering
//==========================================================================
void LevelMeter::setLevel (float level)
{
    setMeterLevel (0, level);
}

void LevelMeter::meterLevelChanged()
{
    repaint();
}
