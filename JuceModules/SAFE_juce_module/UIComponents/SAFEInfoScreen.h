#ifndef __SAFEINFOSCREEN__
#define __SAFEINFOSCREEN__

//==========================================================================
//      Component with all the Info
//==========================================================================
/**
 *  The usage instruction for the SAFE plug-ins.
 */
class SAFEInfo : public Component
{
public:
    //==========================================================================
    //      Constructor and Destructor
    //==========================================================================
    /** Create a new info screen. */
    SAFEInfo();

    /** Destructor */
    ~SAFEInfo();

    //==========================================================================
    //      Paint the Thing
    //==========================================================================
    /** Draw the component. */
    void paint (Graphics& g);

    //==========================================================================
    //      Set Version Number
    //==========================================================================
    /** Set the most recent version number of the plug-ins. 
     *
     *  If the version number of the currently running plug-in is
     *  less than this number a notification to update the plug-in will be displayed.
     *
     *  @param versionNumber  the most recent version number of the SAFE plug-ins
     */
    void setMostRecentVersion (float versionNumber);

private:
    Image currentImage, updateImage;

    float version;
};

//==========================================================================
//      A Sliding Screen to House the Info
//==========================================================================
/**
 *  A SAFEExraScreen to show the plug-in usage instructions.
 */
class SAFEInfoScreen : public SAFEExtraScreen
{
public:
    //==========================================================================
    //      Constructor and Destructor
    //==========================================================================
    /** Create a new info screen. */
    SAFEInfoScreen();

    /** Destructor */
    ~SAFEInfoScreen();

    // button made accessible so listeners can easily be added
    SAFEButton closeButton; /**< The close button.
                             *   Made public so listeners can be added easily.
                             */

    Viewport viewport; /**< The Viewport where the instructions are shown. */
    SAFEInfo info; /**< The plug-in usage instructions. */

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SAFEInfoScreen)
};

#endif // __SAFEINFOSCREEN__
