#ifndef __XYSLIDER__
#define __XYSLIDER__

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
    XYSlider();
    ~XYSlider();

    void paint (Graphics &g);
    void paintOverChildren (Graphics &g);

    double getXValue();
    double getYValue();

    void setXValue (double newValue);
    void setYValue (double newValue);

    void setRange (double newMinX, double newMaxX, double newMinY, double newMaxY);

    //==========================================================================
    //      Listener Stuff
    //==========================================================================
    class Listener
    {
    public:
        virtual ~Listener() {};

        virtual void xySliderMoved (XYSlider*) = 0;
    };

    void addListener (Listener* newListener);
    void removeListener (Listener* listener);

    void controlChanged (bool notifyListeners);

private:
    double minX, minY, maxX, maxY;

    OwnedArray <Listener> listenerArray;
};

#endif // __XYSLIDER__
