#ifndef __SAFEColours__
#define __SAFEColours__

/**
 *  Much like the JUCE Colours class but with the SAFE colours in it.
 */
class SAFEColours
{
public:
    static const Colour green, /**< The colour of the green dial. */
                        yellow, /**< The colour of the yellow dial. */
                        red, /**< The colour of the red dial. */
                        textEditorGrey, /**< The colour of the text editor background. */
                        metaDataGrey, /**< The colour of the dialogue boxes. */
                        buttonGrey, /**< The colour of the button bodies. */
                        mouseOverButtonGrey; /**< The colour of the button 
                                                  bodies when moused over. */

private:
    SAFEColours();

    JUCE_DECLARE_NON_COPYABLE (SAFEColours);
};

#endif // __SAFEColours__
