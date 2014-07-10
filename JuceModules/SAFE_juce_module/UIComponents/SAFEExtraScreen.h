#ifndef __SAFEEXTRASCREEN__
#define __SAFEEXTRASCREEN__

class SAFEExtraScreen : public Component
{
public:
    //==========================================================================
    //      Constructor and Destructor
    //==========================================================================
    SAFEExtraScreen();
    ~SAFEExtraScreen();

    //==========================================================================
    //      Paint the Thing
    //==========================================================================
    void paint (Graphics& g);

protected:
    Label titleLabel;
};

#endif // __SAFEEXTRASCREEN__
