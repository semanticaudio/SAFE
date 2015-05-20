#ifndef __SAFEMETADATASCREEN__
#define __SAFEMETADATASCREEN__

/**
 *  A struct to hold the users meta data.
 */
struct SAFEMetaData
{
    String genre, instrument, location, experience, language, age;
};

/** 
 *  A dialogue box for entering meta data.
 */
class SAFEMetaDataScreen : public SAFEExtraScreen
{
public:
    //==========================================================================
    //      Constructor and Destructor
    //==========================================================================
    /** Create a new meta data screen. */
    SAFEMetaDataScreen();

    /** Destructor */
    ~SAFEMetaDataScreen();

    //==========================================================================
    //      Get the Data
    //==========================================================================
    /** Get the meta data currently written in the text boxes. */
    SAFEMetaData getMetaData();

    //==========================================================================
    //      Set the Data
    //==========================================================================
    /** Set the meta data in the text boxes.
     *
     *  @param metaData  the new meta data.
     */
    void setMetaData (const SAFEMetaData& metaData);

    // button made accessible so listeners can easily be added
    SAFEButton submitButton; /**< The submit button.
                              *   Made public so listeners can be added easily.
                              */

private:
    // the various boxes and whatnot
    TextEditor genreBox, instrumentBox, locationBox, languageBox, experienceBox, ageBox;
    Label genreLabel, instrumentLabel, locationLabel, languageLabel, experienceLabel, ageLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SAFEMetaDataScreen)
};

#endif // __SAFEMETADATASCREEN__

