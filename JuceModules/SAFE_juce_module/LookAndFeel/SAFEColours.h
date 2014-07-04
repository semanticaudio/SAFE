#ifndef __SAFEColours__
#define __SAFEColours__

// much like the juce Colours class but with our colours in it
class SAFEColours
{
public:
    static const Colour green,
                        yellow,
                        red,
                        textEditorGrey,
                        metaDataGrey,
                        buttonGrey,
                        mouseOverButtonGrey;

private:
    SAFEColours();

    JUCE_DECLARE_NON_COPYABLE (SAFEColours);
};

#endif // __SAFEColours__
