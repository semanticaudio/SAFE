#ifndef __FILTERGRAPH__
#define __FILTERGRAPH__

/**
 *  A component to control an EQ using dots on a graph of the frequency response.
 */
class FilterGraph : public Component,
                    public SettableTooltipClient
{
private:
    //==========================================================================
    //      Control Dot Class
    //==========================================================================
    class FilterControl : public Component,
                          public ComponentListener
    {
    public:
        //==========================================================================
        //      Constructor and Destructor
        //==========================================================================
        FilterControl();
        ~FilterControl();

        //==========================================================================
        //      Control Settings
        //==========================================================================
        void setParameters (float frequency, float gain, float qFactor);
        void setLimits (float newMinFreq, float newMaxFreq, float newMinGain, float newMaxGain);
        float getFrequency();
        float getGain();

        void setQLimits (float newMinQ, float newMaxQ, float newQSkewFactor);
        float getQ();

        void setColour (Colour newColour);
        Colour getColour();

        //==========================================================================
        //      Filter Control Callbacks
        //==========================================================================
        void componentMovedOrResized (Component& component, bool wasMoved, bool wasResized);
        void updateQ (float changeAmount);

        //==========================================================================
        //      Mouse Over Bits
        //==========================================================================
        void mouseOverDot();
        void mouseOffDot();

    private:
        //==========================================================================
        //      Utility Functions
        //==========================================================================
        float freqToX (float freq);
        float xToFreq (float xPos);
        float gainToY (float gain);
        float yToGain (float yPos);

        //==========================================================================
        //      The Dot Itself
        //==========================================================================
        class DraggableDot : public Component
        {
        public:
            //==========================================================================
            //      Constructor and Destructor
            //==========================================================================
            DraggableDot();
            ~DraggableDot();

            //==========================================================================
            //      Mouse Stuff
            //==========================================================================
            ComponentDragger dragger;
            ComponentBoundsConstrainer bounds;
            void mouseEnter (const MouseEvent& e);
            void mouseExit (const MouseEvent& e);
            void mouseDown (const MouseEvent& e);
            void mouseDrag (const MouseEvent& e);
            void mouseWheelMove (const MouseEvent& e, const MouseWheelDetails& w);

            //==========================================================================
            //      Paint
            //==========================================================================
            void paint (Graphics& g);

            Colour colour;
        };

        DraggableDot dot;

        float q;
        float baseQ;
        float qSkewFactor;
        float minQ, maxQ;
        float minFreq, maxFreq;
        float minGain, maxGain;

        bool notifyListeners;
    };

public:
    //==========================================================================
    //      Constructor and Destructor
    //==========================================================================
    /** Create a new filter graph. 
     *
     *  @param numFilters  the number of separate filters which can be controlled
     */
    FilterGraph (int numFilters);
    ~FilterGraph();
    
    /** An enum type for the type of trace to plot. */
    enum TraceType
    {
        Magnitude, /**< Plot magnitude */
        Phase /**< Plot phase */
    };
    
    //==========================================================================
    //      Paint
    //==========================================================================
    /** Draw the response of a single filter as well as the combined response. 
     *
     *  @param control  the control dot corresponding to the filter to draw
     */
    void paintSingleFilter (FilterControl* control);

    /** Draw the response of a single filter as well as the combined response. 
     *
     *  @param filterNum  the index of the filter to draw - this must be
     *                    between 0 and (numFilters - 1)
     */
    void paintSingleFilter (int filterNum);

    /** Hide a single filter's response on the graph.
     *
     *  @param internallyCalled  should be set to true if this is called from
     *                           an object other than this
     */
    void hideSingleFilter (bool internallyCalled = false);

    /** Draw the component. */
    void paint (Graphics&);

    /** Draw the component's border. */
    void paintOverChildren (Graphics&);

    /** Function called when the size of the graph is changed. */
    void resized();
    
    //==========================================================================
    //      Change Appearance
    //==========================================================================
    /** Set the number of horizontal grid lines on the graph.
     *
     *  @param newValue  the number of lines to show
     */
    void setNumHorizontalLines (int newValue);

    /** Set the range for the x axis.
     *
     *  @param newLowFreq   the low end of the x axis
     *  @param newHighFreq  the high end of the y axis
     */
    void setFreqRange (float newLowFreq, float newHighFreq);

    /** Set the maximum decibel value on the y axis when in magnitude mode.
     *
     *  @param newMaxDecibels  the maximum value to display on the y axis - the y axis will 
     *                         range from -newMaxDecibels to newMaxDecibels
     */
    void setMaxDecibels (float newMaxDecibels);

    /** Set the maximum phase value on the y axis when in phase mode.
     *
     *  @param newMaxPhase  the maximum value to display on the y axis - the y axis will 
     *                         range from -newMaxPhase to newMaxPhase
     */
    void setMaxPhase (float newMaxPhase);

    /** Set the trace colour.
     *
     *  @param newColour  the new trace colour
     */
    void setTraceColour (Colour newColour);

    /** Set the property of the filters to plot.
     *
     *  @param newTraceType  the filter property to plot
     */
    void setTraceType (TraceType newTraceType);
    
    //==========================================================================
    //      Set Filters
    //==========================================================================
    /** Set the gain of one of the filters.
     *
     *  @param filterNum  the index of the filter to alter - this must be
     *                    between 0 and (numFilters - 1)
     *  @param gain       the gain value of the filter being altered
     */
    void setFilterGain (int filterNum, double gain);

    /** Set the coefficients of one of the filters.
     *
     *  @param filterNum        the index of the filter to alter - this must be
     *                          between 0 and (numFilters - 1)
     *  @param sampleRate       the new sample rate of the filter
     *  @param newCoefficients  the new filter coefficients
     */
    void setFilter (int filterNum, double sampleRate, IIRCoefficients newCoefficients);

    /** Set the coefficients of one of the filters.
     *
     *  @param filterNum   the index of the filter to alter - this must be
     *                     between 0 and (numFilters - 1)
     *  @param sampleRate  the new sample rate of the filter
     *  @param numCoeffs   the new numerator coefficients
     *  @param denCoeffs   the new denominator coefficients
     */
    void setFilter (int filterNum, double sampleRate, Array <double> numCoeffs, Array <double> denCoeffs);
    
    //==========================================================================
    //      Set Control Dots
    //==========================================================================
    /** Set the limits one of the filter controls can be dragged to.
     *
     *  @param filterNum  the index of the filter to alter - this must be
     *                    between 0 and (numFilters - 1)
     *  @param minFreq    the minimum frequency allowed to be set 
     *  @param maxFreq    the maximum frequency allowed to be set 
     *  @param minGain    the minimum gain allowed to be set 
     *  @param maxGain    the maximum gain allowed to be set 
     */
    void setControlLimits (int filterNum, float minFreq, float maxFreq, float minGain, float maxGain);

    /** Set the limits a filters q can be changed to.
     *
     *  @param filterNum    the index of the filter to alter - this must be
     *                      between 0 and (numfilters - 1)
     *  @param minQ         the minimum Q allowed to be set
     *  @param maxQ         the maximum Q allowed to be set
     *  @param qSkewFactor  the skew factor to apply to the setting of Q
     */
    void setControlQLimits (int filterNum, float minQ, float maxQ, float qSkewFactor = 1);

    /** Set the parameters of one of the filters.
     *
     *  @param filterNum  the index of the filter to alter - this must be
     *                    between 0 and (numfilters - 1)
     *  @param freq       the new centre frequency
     *  @param gain       the new gain
     *  @param qFactor    the new Q factor
     */
    void setControlParameters (int filterNum, float freq, float gain, float qFactor);

    /** Get the centre frequency of one of the filters.
     *
     *  @param controlNum  the index of the filter - this must be
     *                     between 0 and (numfilters - 1)
     */
    float getControlFrequency (int controlNum);

    /** Get the gain of one of the filters.
     *
     *  @param controlNum  the index of the filter - this must be
     *                     between 0 and (numfilters - 1)
     */
    float getControlGain (int controlNum);

    /** Get the Q factor of one of the filters.
     *
     *  @param controlNum  the index of the filter - this must be
     *                     between 0 and (numfilters - 1)
     */
    float getControlQ (int controlNum);

    /** Set the colour of one of the filter controls.
     *
     *  @param filterNum  the index of the filter to alter - this must be
     *                    between 0 and (numfilters - 1)
     *  @param newColour  the new colour
     */
    void setControlColour (int filterNum, Colour newColour);

    //==========================================================================
    //      Utility Functions
    //==========================================================================
    /** Get the frequency which corresponds to a given x position in the component.
     *  
     *  @param xPos  the x position in the component to convert to a frequency
     */
    float xToFreq (float xPos);

    /** Get the x position in the component which corresponds to a given frequency.
     *  
     *  @param freq  the frequency to convert to an x position in the component
     */
    float freqToX (float freq);

    /** Get the gain which corresponds to a given y position in the component.
     *  
     *  @param yPos  the y position in the component to convert to a gain
     */
    float yToGain (float yPos);

    /** Get the y position in the component which corresponds to a given gain
     *  
     *  @param gain  the gain to convert to a y position in the component
     */
    float gainToY (float gain);

    /** Get the phase which corresponds to a given y position in the component.
     *  
     *  @param yPos  the y position in the component to convert to a phase
     */
    float yToPhase (float yPos);

    /** Get the y position in the component which corresponds to a phase.
     *  
     *  @param phase  the phase to convert to a y position in the component
     */
    float phaseToY (float phase);

    /** Get the combined magnitude response of the filters as a given frequency.
     *
     *  @param freq  the frequency to get the response at
     */
    float getMagnitudeResponse (float freq);

    /** Get the combined phase response of the filters as a given frequency.
     *
     *  @param freq  the frequency to get the response at
     */
    float getPhaseResponse (float freq);
    
    //==========================================================================
    //      Listener Stuff
    //==========================================================================
    /** A class to listen for changes in a FilterGraph */
    class Listener
    {
    public:
        /** Destructor */
        virtual ~Listener() {};

        /** Callback function called when a one of the filter controls is moved.
         * 
         *  @param controlNum  the index of the filter control which was moved
         */
        virtual void filterControlChanged (int controlNum) = 0;
    };

    /** Add a listener to be notified of changes in this component
     *
     *  @param newListener  a pointer to the listener to add
     */
    void addListener (Listener* newListener);

    /** Remove a listener
     *
     *  @param listener  a pointer to the listener to remove
     */
    void removeListener (Listener* listener);

    /** Called when one of the filter controls is moved
     *  
     *  @param control          the filter control moved
     *  @param nofityListeners  if true the listeners will be notified of the change
     */
    void filterControlChanged (FilterControl* control, bool notifyListeners);

private:
    bool externalShowSingleFilter;
    bool showSingleFilter;
    int filterToShow;

    int numHorizontalLines;
    float lowFreq, highFreq;   
    float maxDecibels, maxPhase;
    Colour traceColour;
    TraceType traceType;

    double fs;
    int numFilters;
    
    void mouseMove (const MouseEvent &event);
    
    OwnedArray <FilterInfo> filterArray;
    OwnedArray <FilterControl> controlArray;
    OwnedArray <Listener> listenerArray;
    
    Path gridPath, tracePath, singleFilterPath;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterGraph)
};

#endif  // __FILTERGRAPH__
