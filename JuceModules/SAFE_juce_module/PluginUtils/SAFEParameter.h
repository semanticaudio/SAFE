#ifndef __SAFEPARAMETER__
#define __SAFEPARAMETER__

class SAFEParameter
{
public:
    //==========================================================================
    //      Constructor and Destructor
    //==========================================================================
    SAFEParameter (String nameInit, float& valueRef, float initialValue = 1, float minValueInit = 0, float maxValueInit = 1, String unitsInit = String::empty, float skewFactorInit = 1, bool convertDBToGainValue = false);
    ~SAFEParameter();
    
    //==========================================================================
    //      Getters and Setters
    //==========================================================================
    void setBaseValue (float newValue);
    void setScaledValue (float newValue);
    
    float getBaseValue() const;   
    float getScaledValue() const;
    float getGainValue() const;
    float getDefaultValue() const;
    
    float getMinValue() const;
    float getMaxValue() const;
    float getSkewFactor() const;
    
    const String getName() const;
    const String getUnits() const;
    
private:
    float baseValue, minValue, maxValue, defaultValue, skewFactor, scaledValue, gainValue;
    float& outputValue;
    
    String name, units;
    
    bool convertToGain;
    
    void updateOutputValue();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SAFEParameter)
};

#endif  // __SAFEPARAMETER__
