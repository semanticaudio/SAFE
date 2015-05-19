#ifndef __XYSLIDER__
#define __XYSLIDER__

/** 
 *  A two dimensional slider component
 */
class XYSlider : public Component
{
private:
    //==========================================================================
    //      Control Dot Class
    //==========================================================================
    class ControlDot : public Component,
                       public ComponentListener
    {
    public:
        //==========================================================================
        //      Constructor and Destructor
        //==========================================================================
        ControlDot();
        ~ControlDot();

        //==========================================================================
        //      Control Settings
        //==========================================================================
        float getX();
        float getY();
        void setX (int newX);
        void setY (int newY);

        void parentSizeChanged();

        //==========================================================================
        //      Control Callbacks
        //==========================================================================
        void componentMovedOrResized (Component& component, bool wasMoved, bool wasResized);

        //==========================================================================
        //      Mouse Over Bits
        //==========================================================================
        void mouseOverDot();
        void mouseOffDot();

    private:
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
            void mouseDown (const MouseEvent& e);
            void mouseDrag (const MouseEvent& e);

            //==========================================================================
            //      Paint
            //==========================================================================
            void paint (Graphics& g);

            Colour colour;
        };

        DraggableDot dot;

        bool notifyListeners;
    };

    ControlDot controlDot;

public:
    /** Create a new XY slider. */
    XYSlider();
    
    /** Destructor */
    ~XYSlider();

    /** Draw the component. */
    void paint (Graphics &g);

    /** Draw the component's border. */
    void paintOverChildren (Graphics &g);

    /** Get the current x value of the slider. */
    double getXValue();

    /** Get the current y value of the slider. */
    double getYValue();

    /** Set the x value of the slider.
     *
     *  @param newValue  the new x value  
     */
    void setXValue (double newValue);

    /** Set the y value of the slider.
     *
     *  @param newValue  the new y value  
     */
    void setYValue (double newValue);

    /** Set the slier range
     *
     *  @param newMinX  the minimum x value
     *  @param newMaxX  the maximum x value
     *  @param newMinY  the minimum y value
     *  @param newMaxY  the maximum y value
     */
    void setRange (double newMinX, double newMaxX, double newMinY, double newMaxY);

    //==========================================================================
    //      Listener Stuff
    //==========================================================================
    /** A class to listen for changes in an XYSlider. */
    class Listener
    {
    public:
        /** Destructor */
        virtual ~Listener() {};

        /** Callback function called when an XYSlider is moved. */
        virtual void xySliderMoved (XYSlider*) = 0;
    };

    /** Add a listener to be notified of changes in this slider
     *
     *  @param newListener  a pointer to the listener to add
     */
    void addListener (Listener* newListener);

    /** Remove a listener
     *
     *  @param listener  a pointer to the listener to remove
     */
    void removeListener (Listener* listener);

    /** Notify listeners of a change depending on the argument.
     *
     *  @param notifyListeners  if true notify listeners of a slider movement
     */
    void controlChanged (bool notifyListeners);

private:
    double minX, minY, maxX, maxY;

    OwnedArray <Listener> listenerArray;
};

#endif // __XYSLIDER__
