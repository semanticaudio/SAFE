#ifndef __SAFEEXTRASCREEN__
#define __SAFEEXTRASCREEN__

/**
 *  A base class for the dialogue boxes from the SAFE UI.
 */
class SAFEExtraScreen : public Component
{
public:
    //==========================================================================
    //      Constructor and Destructor
    //==========================================================================
    /** Create a new extra screen. */
    SAFEExtraScreen();

    /** Destructor */
    ~SAFEExtraScreen();

    //==========================================================================
    //      Paint the Thing
    //==========================================================================
    /** Draw the component. */
    void paint (Graphics& g);

protected:
    Label titleLabel; /**< The title of the dialogue box. 
                       *   Should probably be a parameter of the constructor
                       *   but hey we live and learn.
                       */
};

#endif // __SAFEEXTRASCREEN__
