#ifndef __SAFEDESCRIPTORLOADSCREEN__
#define __SAFEDESCRIPTORLOADSCREEN__

class SAFEDescriptorLoadScreen : public Component,
                                 public ListBoxModel,
                                 public Button::Listener,
                                 public TextEditor::Listener,
                                 public KeyListener
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
    void listBoxItemDoubleClicked (int row, const MouseEvent& e);

    //==========================================================================
    //      Get Descriptors
    //==========================================================================
    void updateDescriptors (bool fromServer, const XmlElement* localSemanticDataElement);
    String getSelectedDescriptor();

    //==========================================================================
    //      Descriptor Search
    //==========================================================================
    void buttonClicked (Button *buttonThatWasClicked);
    void textEditorTextChanged (TextEditor&);
    void textEditorReturnKeyPressed (TextEditor&);

    //==========================================================================
    //      Key Listener Stuff
    //==========================================================================
    bool keyPressed (const KeyPress &key, Component *originatingComponent);


    // button made accessible so listeners can easily be added
    SAFEButton loadButton, closeButton;

private:
    // the various boxes and whatnot
    TextEditor searchBox;
    StringArray allDescriptors, searchedDescriptors;
    ListBox descriptorBox;
    Label titleLabel;

    SAFEButton refreshButton;
    String previousSearchTerm;

    bool getDataFromServer;
    const XmlElement *localSemanticData;

    //==========================================================================
    //      Descriptor Search
    //==========================================================================
    void searchDescriptors();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SAFEDescriptorLoadScreen)
};

#endif // __SAFEDESCRIPTORLOADSCREEN__
