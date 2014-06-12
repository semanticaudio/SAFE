#ifndef __FILTERGRAPH__
#define __FILTERGRAPH__

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
    FilterGraph (int numFilters);
    ~FilterGraph();
    
    enum TraceType
    {
        Magnitude,
        Phase
    };
    
    //==========================================================================
    //      Paint
    //==========================================================================
    void paintSingleFilter (FilterControl* control);
    void paintSingleFilter (int filterNum);
    void hideSingleFilter (bool internallyCalled = false);
    void paint (Graphics&);
    void paintOverChildren (Graphics&);
    void resized();
    
    //==========================================================================
    //      Change Appearance
    //==========================================================================
    void setNumHorizontalLines (int newValue);
    void setFreqRange (float newLowFreq, float newHighFreq);
    void setMaxDecibels (float newMaxDecibels);
    void setMaxPhase (float newMaxPhase);
    void setTraceColour (Colour newColour);
    void setTraceType (TraceType newTraceType);
    
    //==========================================================================
    //      Set Filters
    //==========================================================================
    void setFilterGain (int filterNum, double gain);
    void setFilter (int filterNum, double sampleRate, IIRCoefficients newCoefficients);
    void setFilter (int filterNum, double sampleRate, Array <double> numCoeffs, Array <double> denCoeffs);
    
    //==========================================================================
    //      Set Control Dots
    //==========================================================================
    void setControlLimits (int filterNum, float minFreq, float maxFreq, float minGain, float maxGain);
    void setControlQLimits (int filterNum, float minQ, float maxQ, float qSkewFactor = 1);
    void setControlParameters (int filterNum, float freq, float gain, float qFactor);
    float getControlFrequency (int controlNum);
    float getControlGain (int controlNum);
    float getControlQ (int controlNum);
    void setControlColour (int filterNum, Colour newColour);

    //==========================================================================
    //      Utility Functions
    //==========================================================================
    float xToFreq (float xPos);
    float freqToX (float freq);

    float yToGain (float yPos);
    float gainToY (float gain);

    float yToPhase (float yPos);
    float phaseToY (float phase);

    float getMagnitudeResponse (float freq);
    float getPhaseResponse (float freq);
    
    //==========================================================================
    //      Listener Stuff
    //==========================================================================
    class Listener
    {
    public:
        virtual ~Listener() {};

        virtual void filterControlChanged (int controlNum) = 0;
    };

    void addListener (Listener* newListener);
    void removeListener (Listener* listener);

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
