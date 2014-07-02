#ifndef __SAFEDESCRIPTORLOADSCREEN__
#define __SAFEDESCRIPTORLOADSCREEN__

class SAFEDescriptorLoadScreen : public Component,
                                 public ListBoxModel,
                                 public Button::Listener
{
public:
    //==========================================================================
    //      Constructor and Destructor
    //==========================================================================
    SAFEDescriptorLoadScreen();
    ~SAFEDescriptorLoadScreen();

    //==========================================================================
    //      Paint the Thing
    //==========================================================================
    void paint (Graphics& g);

    //==========================================================================
    //      List Box Model Stuff
    //==========================================================================
    int getNumRows();
    void paintListBoxItem (int rowNumber, Graphics &g, int width, int height, bool rowIsSelected);

    //==========================================================================
    //      Get Descriptors
    //==========================================================================
    void updateDescriptors (bool fromServer, XmlElement* localSemanticDataElement);

    //==========================================================================
    //      Descriptor Search
    //==========================================================================
    void buttonClicked (Button *buttonThatWasClicked);

    // button made accessible so listeners can easily be added
    TextButton closeButton, loadButton;

private:
    // the various boxes and whatnot
    TextEditor searchBox;
    StringArray allDescriptors, searchedDescriptors;
    ListBox descriptorBox;
    Label titleLabel;

    TextButton searchButton;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SAFEDescriptorLoadScreen)
};

#endif // __SAFEDESCRIPTORLOADSCREEN__
