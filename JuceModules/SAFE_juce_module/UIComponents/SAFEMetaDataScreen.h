#ifndef __SAFEMETADATASCREEN__
#define __SAFEMETADATASCREEN__

struct SAFEMetaData
{
    String genre, instrument, location, experience, language, age;
};

class SAFEMetaDataScreen : public SAFEExtraScreen
{
public:
    //==========================================================================
    //      Constructor and Destructor
    //==========================================================================
    SAFEMetaDataScreen();
    ~SAFEMetaDataScreen();

    //==========================================================================
    //      Get the Data
    //==========================================================================
    SAFEMetaData getMetaData();

    //==========================================================================
    //      Set the Data
    //==========================================================================
    void setMetaData (const SAFEMetaData& metaData);

    // button made accessible so listeners can easily be added
    SAFEButton submitButton;

private:
    // the various boxes and whatnot
    TextEditor genreBox, instrumentBox, locationBox, languageBox, experienceBox, ageBox;
    Label genreLabel, instrumentLabel, locationLabel, languageLabel, experienceLabel, ageLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SAFEMetaDataScreen)
};

#endif // __SAFEMETADATASCREEN__

