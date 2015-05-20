#ifndef __SAFEPARAMETER__
#define __SAFEPARAMETER__

/** 
 *  A class for handling SAFE plug-in parameters.
 */
class SAFEParameter
{
public:
    //==========================================================================
    //      Constructor and Destructor
    //==========================================================================
    /** Create a new plug-in parameter.
     *
     *  @param nameInit               the name of the parameter
     *  @param valueRef               a reference to a variable which will hold this
     *                                parameters value
     *  @param initialValue           the default value for this parameter
     *  @param minValueInit           the minimum value for this parameter
     *  @param maxValueInit           the maximum value for this parameter
     *  @param unitsInit              a string to display after this parameters value
     *  @param skewFactorInit         the skew factor for this parameter - see
     *                                the documentation of skew factor in the JUCE 
     *                                slider class
     *  @param convertDBToGainValue   if true the parameters value will be treated as though
     *                                it were a gain in decibels and valueRef will be 
     *                                converted to a gain which can be applied to a signal
     *  @param interpolationTimeInit  the time (in milliseconds) it takes for a parameter 
     *                                to change to a new value which was set
     *  @param UIScaleFactorInit      a factor to scale the parameter value by when
     *                                it is displayed in a user interface
     */
    SAFEParameter (String nameInit, float& valueRef, float initialValue = 1, float minValueInit = 0, float maxValueInit = 1, String unitsInit = String::empty, float skewFactorInit = 1, bool convertDBToGainValue = false, double interpolationTimeInit = 100, float UIScaleFactorInit = 1);
    ~SAFEParameter();
    
    //==========================================================================
    //      Getters and Setters
    //==========================================================================
    /** Set the value of the parameter in the range 0-1.
     *
     *  As JUCE plug-in accept all parameter value changes in the range 0-1 this function
     *  can be used to set the parameter value without having to do any conversion
     *  yourself.
     *
     *  @param newValue  the new parameter value in the range 0-1
     */
    void setBaseValue (float newValue);

    /** Set the value of the parameter.
     *
     *  @param newValue  the new parameter value in its actual range
     */
    void setScaledValue (float newValue);
    
    /** Returns the parameter value in the range 0-1 for passing to the plug-in's host. */
    float getBaseValue() const;   

    /** Returns the parameter value in its actual range. */
    float getScaledValue() const;

    /** Returns the parameter value scaled for use in the user interface. */
    float getUIScaledValue() const;

    /** Returns the parameter value converted from decibels to a gain factor.
     *
     *  Returns the same as getScaledValue() if convertDBToGain 
     *  was set to false in the constructor.
     */
    float getGainValue() const;

    /** Returns the default value of this parameter. */
    float getDefaultValue() const;
    
    /** Returns the minimum value of this parameter. */
    float getMinValue() const;

    /** Returns the maximum value of this parameter. */
    float getMaxValue() const;

    /** Returns the skew factor of this parameter. */
    float getSkewFactor() const;
    
    /** Returns the name of this parameter. */
    const String getName() const;

    /** Returns the units string for this parameter. */
    const String getUnits() const;

    /** Returns the UI scale factor for this parameter.
     *
     *  This is the factor the parameter value is scaled by for display in the user interface.
     */
    float getUIScaleFactor() const;

    //==========================================================================
    //      Smoothing Bits
    //==========================================================================
    /** Set the sample rate of the audio being processed.
     *  
     *  @param newSampleRate  the sample rate of the incoming audio
     */
    void setSampleRate (double newSampleRate);

    /** Set the rate at which parameter values are updated.
     *
     *  @param newControlRate  the rate at which parameter values are updated in Hz
     */
    void setControlRate (double newControlRate);

    /** Set the amout of time it takes for a parameter to change to its new value.
     *
     *  @param newInterpolationTime  the new interpolation time in milliseconds
     */
    void setInterpolationTime (double newInterpolationTime);

    /** Returns true if the parameter value is not yet equal to the value it was set to. */
    bool isInterpolating() const;

    /** Increment a parameter value towards the value it was set to.
     *
     *  This should be called after each group of
     *  \f$ \frac{\text{sample rate}}{\text{control rate}} \f$ samples has been processed.
     */
    void smoothValues();

private:
    float baseValue, minValue, maxValue, defaultValue, skewFactor, scaledValue, gainValue;
    float smoothedValue;

    double sampleRate, controlRate, interpolationTime;
    float interpolationStep;
    
    int controlBlockSize, interpolationBlockSize;
    int controlBlocksPerChange, currentControlBlock;

    bool interpolating, initialised;
    float& outputValue;
    
    String name, units;
    
    bool convertToGain;

    double UIScaleFactor;

    void updateBlockSizes();

    void startInterpolating();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SAFEParameter)
};

#endif  // __SAFEPARAMETER__
