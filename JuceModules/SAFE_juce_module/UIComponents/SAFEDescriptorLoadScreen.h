#ifndef __SAFEDESCRIPTORLOADSCREEN__
#define __SAFEDESCRIPTORLOADSCREEN__

/**
 *  The dialogue box for loading descriptors in the plug-ins.
 */
class SAFEDescriptorLoadScreen : public SAFEExtraScreen,
                                 public ListBoxModel,
                                 public Button::Listener,
                                 public TextEditor::Listener,
                                 public KeyListener
{
public:
    //==========================================================================
    //      Constructor and Destructor
    //==========================================================================
    /** Create a new descriptor load screen. */
    SAFEDescriptorLoadScreen();

    /** Destructor */
    ~SAFEDescriptorLoadScreen();

    //==========================================================================
    //      List Box Model Stuff
    //==========================================================================
    /** Implementation of function from ListBoxModel */
    int getNumRows();

    /** Implementation of function from ListBoxModel */
    void paintListBoxItem (int rowNumber, Graphics &g, int width, int height, bool rowIsSelected);

    /** Implementation of function from ListBoxModel */
    void listBoxItemDoubleClicked (int row, const MouseEvent& e);

    //==========================================================================
    //      Get Descriptors
    //==========================================================================
    /** Update the list of descriptors.
     *
     *  @param fromServer                if true the descriptor list will be populated
     *                                   by the descriptors on the server otherwise the
     *                                   local descriptor file will be used
     *  @param localSemanticDataElement  a pointer to the XMLElement from the local descriptor 
     *                                   file
     */
    void updateDescriptors (bool fromServer, const XmlElement* localSemanticDataElement);

    /** Returns the currently selected descriptor. */
    String getSelectedDescriptor();

    //==========================================================================
    //      Descriptor Search
    //==========================================================================
    /** Implementation of function from Button::Listener */
    void buttonClicked (Button *buttonThatWasClicked);

    /** Implementation of function from TextEditor::Listener */
    void textEditorTextChanged (TextEditor&);

    /** Implementation of function from TextEditor::Listener */
    void textEditorReturnKeyPressed (TextEditor&);

    //==========================================================================
    //      Key Listener Stuff
    //==========================================================================
    /** Implementation of function from KeyListener */
    bool keyPressed (const KeyPress &key, Component *originatingComponent);


    // button made accessible so listeners can easily be added
    SAFEButton loadButton; /**< The load button. 
                            *   Made public so listeners can be added easily.
                            */
    SAFEButton closeButton; /**< The close button.
                             *   Made public so listeners can be added easily.
                             */

private:
    // the various boxes and whatnot
    TextEditor searchBox;
    StringArray allDescriptors, searchedDescriptors;
    ListBox descriptorBox;

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
