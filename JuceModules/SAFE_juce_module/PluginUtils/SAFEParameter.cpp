//==========================================================================
//      Constructor and Destructor
//==========================================================================
SAFEParameter::SAFEParameter (String nameInit, float& valueRef, float initialValue, float minValueInit, float maxValueInit, String unitsInit, float skewFactorInit, bool convertDBToGainValue)
    : outputValue (valueRef),
      convertToGain (convertDBToGainValue)
{
    name = nameInit;
    minValue = minValueInit;
    maxValue = maxValueInit;
    defaultValue = initialValue;
    skewFactor = skewFactorInit;
    units = unitsInit;

    interpolating = false;
    initialised = false;
    sampleRate = 44100;
    controlRate = 64;
    interpolationTime = 100;
    updateBlockSizes();

    setScaledValue (defaultValue);
}

SAFEParameter::~SAFEParameter()
{
}

//==========================================================================
//      Getters and Setters
//==========================================================================
void SAFEParameter::setBaseValue (float newBaseValue)
{
    baseValue = newBaseValue;
    
    float range = maxValue - minValue;
    scaledValue = range * pow (baseValue, (1 / skewFactor)) + minValue;
    
    gainValue = Decibels::decibelsToGain (scaledValue);

    startInterpolating();
}

void SAFEParameter::setScaledValue (float newScaledValue)
{
    scaledValue = newScaledValue;
    
    gainValue = Decibels::decibelsToGain (scaledValue);
    
    float range = maxValue - minValue;
    float proportion = scaledValue - minValue;
    baseValue = pow ((proportion / range), skewFactor);  

    startInterpolating();
}

float SAFEParameter::getBaseValue() const
{
    return baseValue;
}

float SAFEParameter::getScaledValue() const
{
    return scaledValue;
}

float SAFEParameter::getGainValue() const
{
    if (convertToGain)
        return gainValue;
    else
        return scaledValue;
}

float SAFEParameter::getDefaultValue() const
{
    return defaultValue;
}

float SAFEParameter::getMinValue() const
{
    return minValue;
}

float SAFEParameter::getMaxValue() const
{
    return maxValue;
}

float SAFEParameter::getSkewFactor() const
{
    return skewFactor;
}

const String SAFEParameter::getName() const
{
    return name;
}

const String SAFEParameter::getUnits() const
{
    return units;
}

//==========================================================================
//      Smoothing Bits
//==========================================================================
void SAFEParameter::setSampleRate (double newSampleRate)
{
    sampleRate = newSampleRate;
    updateBlockSizes();
}

void SAFEParameter::setControlRate (double newControlRate)
{
    controlRate = newControlRate;
    updateBlockSizes();
}

void SAFEParameter::setInterpolationTime (double newInterpolationTime)
{
    interpolationTime = newInterpolationTime;
    updateBlockSizes();
}

bool SAFEParameter::isInterpolating() const
{
    return interpolating;
}

void SAFEParameter::smoothValues()
{
    if (interpolating)
    {
        smoothedValue += interpolationStep;
        ++currentControlBlock;

        if (currentControlBlock >= controlBlocksPerChange)
        {
            smoothedValue = baseValue;
            interpolating = false;
        }

        float range = maxValue - minValue;
        float smoothedScaledValue = range * pow (smoothedValue, (1 / skewFactor)) + minValue;

        if (convertToGain)
            outputValue = Decibels::decibelsToGain (smoothedScaledValue);
        else
            outputValue = smoothedScaledValue;
    }
    else
    {
        if (convertToGain)
            outputValue = gainValue;
        else
            outputValue = scaledValue;
    }
}

void SAFEParameter::updateBlockSizes()
{
    controlBlockSize = (int) (sampleRate / controlRate);
    interpolationBlockSize = (int) (interpolationTime * sampleRate / 1000.0);
    controlBlocksPerChange = (int) (interpolationBlockSize / controlBlockSize);
}

void SAFEParameter::startInterpolating()
{
    if (! initialised)
    {
        smoothedValue = baseValue;
        
        if (convertToGain)
            outputValue = gainValue;
        else
            outputValue = scaledValue;

        initialised = true;
    }
    else
    {
        interpolationStep = (baseValue - smoothedValue) / controlBlocksPerChange;
        currentControlBlock = 1;

        interpolating = ! (smoothedValue == baseValue);
    }
}
