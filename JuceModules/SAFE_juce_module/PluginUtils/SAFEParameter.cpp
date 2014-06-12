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
    
    updateOutputValue();
}

void SAFEParameter::setScaledValue (float newScaledValue)
{
    scaledValue = newScaledValue;
    
    gainValue = Decibels::decibelsToGain (scaledValue);
    
    float range = maxValue - minValue;
    float proportion = scaledValue - minValue;
    baseValue = pow ((proportion / range), skewFactor);  
    
    updateOutputValue();
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

void SAFEParameter::updateOutputValue()
{
    if (convertToGain)
        outputValue = gainValue;
    else
        outputValue = scaledValue;
}
