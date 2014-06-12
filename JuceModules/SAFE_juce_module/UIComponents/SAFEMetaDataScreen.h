#ifndef __SAFEMETADATASCREEN__
#define __SAFEMETADATASCREEN__

struct SAFEMetaData
{
    String genre, instrument, location, experience, language, age;
};

class SAFEMetaDataScreen : public Component
{
public:
    //==========================================================================
    //      Constructor and Destructor
    //==========================================================================
    SAFEMetaDataScreen();
    ~SAFEMetaDataScreen();

    //==========================================================================
    //      Paint the Thing
    //==========================================================================
    void paint (Graphics& g);

    //==========================================================================
    //      Get the Data
    //==========================================================================
    SAFEMetaData getMetaData();

    // button made accessible so listeners can easily be added
    SAFEButton submitButton;

private:
    // the various boxes and whatnot
    TextEditor genreBox, instrumentBox, locationBox, languageBox, experienceBox, ageBox;
    Label titleLabel, genreLabel, instrumentLabel, locationLabel, languageLabel, experienceLabel, ageLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SAFEMetaDataScreen)
};

#endif // __SAFEMETADATASCREEN__

