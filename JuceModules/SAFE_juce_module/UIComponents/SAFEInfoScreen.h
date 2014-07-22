#ifndef __SAFEINFOSCREEN__
#define __SAFEINFOSCREEN__

//==========================================================================
//      Component with all the Info
//==========================================================================
class SAFEInfo : public Component
{
public:
    //==========================================================================
    //      Constructor and Destructor
    //==========================================================================
    SAFEInfo();
    ~SAFEInfo();

    //==========================================================================
    //      Paint the Thing
    //==========================================================================
    void paint (Graphics& g);

    //==========================================================================
    //      Set Version Number
    //==========================================================================
    void setMostRecentVersion (float versionNumber);

private:
    Image currentImage, updateImage;

    float version;
};

//==========================================================================
//      A Sliding Screen to House the Info
//==========================================================================
class SAFEInfoScreen : public SAFEExtraScreen
{
public:
    //==========================================================================
    //      Constructor and Destructor
    //==========================================================================
    SAFEInfoScreen();
    ~SAFEInfoScreen();

    // button made accessible so listeners can easily be added
    SAFEButton closeButton;

    Viewport viewport;
    SAFEInfo info;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SAFEInfoScreen)
};

#endif // __SAFEINFOSCREEN__
