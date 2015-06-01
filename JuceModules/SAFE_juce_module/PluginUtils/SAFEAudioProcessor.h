#ifndef __SAFEAUDIOPROCESSOR__
#define __SAFEAUDIOPROCESSOR__

/**
 *  An extension of the AudioProcessor class for use in SAFE plug-ins.
 *
 *  When you are making a SAFE plug-in you will start by writing a new class which
 *  inherits from this class. This class handles all the audio analysis, saving and 
 *  loading of descriptors and general plug-in things leaving you to concentrate
 *  on the audio processing.
 *
 *  Lots of the pure virtual functions from AudioProcessor are given basic implementations
 *  here. This means you only need to bother writing a few important functions in
 *  order to get a plug-in up and running. In most cases it should be fine if you override
 *  these simple implementations. Where it is important you don't override a function
 *  it should be marked final. If you are using a decent C++11 compiler this should avoid any 
 *  problems. If you are still cludging along with some archaic compiler take heed 
 *  of which functions are marked final in this documentation. Then DON'T OVERRIDE THEM.
 */
class SAFEAudioProcessor : public AudioProcessor,
                           public Timer
{
private:
    //==========================================================================
    //      A Class to Put the Analysis on a Separate Thread
    //==========================================================================
    class AnalysisThread : public Thread
    {
    public:
        //==========================================================================
        //      Constructor and Destructor
        //==========================================================================
        AnalysisThread (SAFEAudioProcessor* processorInit);
        ~AnalysisThread();
        
        //==========================================================================
        //      The Thread Callback
        //==========================================================================
        void run();

        //==========================================================================
        //      Set Some Parameters
        //==========================================================================
        void setParameters (String newDescriptors, SAFEMetaData newMetaData, bool newSendToServer);

    private:
        SAFEAudioProcessor* processor;

        String descriptors;
        SAFEMetaData metaData;
        bool sendToServer;

        //==========================================================================
        //      Lock to ensure only one plug-in saves at a time
        //==========================================================================
        static SpinLock mutex;
    };

    ScopedPointer <AnalysisThread> analysisThread;

public:
    //==========================================================================
    //      Constructor and Destructor
    //==========================================================================
    /** Create a new audio processor. **/
    SAFEAudioProcessor();

    /** Destructor */
    virtual ~SAFEAudioProcessor();
    
    //==========================================================================
    //      Parameter Info Methods
    //==========================================================================
    /** Implementation of function from AudioProcessor. */
    const String getName() const;

    /** Implementation of function from AudioProcessor. */
    int getNumParameters() override final;

    /** Implementation of function from AudioProcessor. */
    float getParameter (int index) override final;

    /** Implementation of function from AudioProcessor. */
    void setParameter (int index, float newValue) override final;
    
    /** Returns the value of a parameter in its actual range.
     *
     *  @param index  the index of the parameter to get the value of
     */
    float getScaledParameter (int index);

    /** Set the value of a parameter with a value in its actual range.
     *
     *  @param index     the index of the parameter to set
     *  @param newValue  the new value for the parameter - within the range
     *                   of parameters the value can take
     *
     *  This function will not notify the host of a parameter update. If you wish
     *  the set the value of a parameter you should use setScaledParameterNotifyingHost().
     */
    void setScaledParameter (int index, float newValue);

    /** Set the value of a parameter with a value in its actual range and notify the
     *  host of a parameter update.
     *
     *  @param index     the index of the parameter to set
     *  @param newValue  the new value for the parameter - within the range
     *                   of parameters the value can take
     */
    void setScaledParameterNotifyingHost (int index, float newValue);

    /** Get the value of a parameter converted from decibels to a gain factor
     *
     *  @param index  the index of the parameter to get the value of
     */
    float getGainParameter (int index);
    
    /** Called when a parameter value has been set.
     *
     *  Override this function po perform any extra calculations you need to do after 
     *  the value of a parameter has been set. Perhaps you need to calculate some filter
     *  coefficients of something.
     *
     *  @param index  the index of the parameter which was set  
     */
    virtual void parameterUpdateCalculations (int /*index*/) {};

    /** Implementation of function from AudioProcessor. */
    const String getParameterName (int index) override final;

    /** Implementation of function from AudioProcessor. */
    const String getParameterText (int index) override final;
    
    /** Returns an read only reference to the plug-ins array of parameters.
     *  
     *  The SAFEAudioProcessorEditor class uses this to get the information needed
     *  to generate its sliders.
     */
    const OwnedArray <SAFEParameter>& getParameterArray();

    //==========================================================================
    //      Other Plugin Info
    //==========================================================================
    /** Implementation of function from AudioProcessor. */
    const String getInputChannelName (int channelIndex) const;

    /** Implementation of function from AudioProcessor. */
    const String getOutputChannelName (int channelIndex) const;

    /** Implementation of function from AudioProcessor. */
    bool isInputChannelStereoPair (int index) const;

    /** Implementation of function from AudioProcessor. */
    bool isOutputChannelStereoPair (int index) const;

    /** Implementation of function from AudioProcessor. */
    bool acceptsMidi() const;

    /** Implementation of function from AudioProcessor. */
    bool producesMidi() const;

    /** Implementation of function from AudioProcessor. */
    bool silenceInProducesSilenceOut() const;

    /** Implementation of function from AudioProcessor. */
    double getTailLengthSeconds() const;

    //==========================================================================
    //      Program Stuff
    //==========================================================================
    /** Implementation of function from AudioProcessor. */
    int getNumPrograms();
    
    /** Implementation of function from AudioProcessor. */
    int getCurrentProgram();
    
    /** Implementation of function from AudioProcessor. */
    void setCurrentProgram (int index);

    /** Implementation of function from AudioProcessor. */
    const String getProgramName (int index);

    /** Implementation of function from AudioProcessor. */
    void changeProgramName (int index, const String& newName);

    //==========================================================================
    //      Saving and Loading Patches
    //==========================================================================
    /** Implementation of function from AudioProcessor. */
    void getStateInformation (MemoryBlock& destData);

    /** Implementation of function from AudioProcessor. */
    void setStateInformation (const void* data, int sizeInBytes);

    //==========================================================================
    //      Semantic Data Parsing
    //==========================================================================
    /** Returns a pointer to the XmlElement from the file the plug-in saves its descriptors
     *  to. */
    XmlElement* getSemanticDataElement();

    /** Load a descriptor from a local file.
     *
     *  @param descriptor  the descriptor to load.
     */
    WarningID loadSemanticData (const String& descriptor);

    /** Load a descriptor from the server
     *
     *  @param descriptor  the descriptor to load.
     */
    WarningID getServerData (const String& descriptor);

    //==========================================================================
    //      Analysis Thread
    //==========================================================================
    /** Returns true if the plug-in is currently analysing some audio. */
    bool isThreadRunning();
    
    //==========================================================================
    //      Process Block
    //==========================================================================
    /** Implementation of function from AudioProcessor. */
    void prepareToPlay (double sampleRate, int samplesPerBlock) final;

    /** Do any preparation your plug-in needs to do here.
     *
     *  prepareToPlay() has already been implemented to do some useful things.
     *  Any preparation you need should be done in this function which will get
     *  called by prepareToPlay().
     */
    virtual void pluginPreparation (double sampleRate, int samplesPerBlock) = 0;

    /** Implementation of function from AudioProcessor. */
    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages) final;

    /** The plug-ins processing goes here. 
     *
     *  processBlock() is implemented by SAFE to take cake of all the analysis and parameter
     *  smoothing. processBlock() will pass the audio to this function for you to process it.
     *
     *  All the same rules that apply to processBlock() apply here. We don't want any
     *  cracks and pops now do we.
     */
    virtual void pluginProcessing (AudioSampleBuffer& buffer, MidiBuffer& midiMessages) = 0;

    //==========================================================================
    //      Playing & Recording Info
    //==========================================================================
    /** Returns true if the host is currently playing audio. */
    bool isPlaying();

    /** Starts recording audio for analysis.
     *
     *  Returns true if the plug-in started recording. If this returns false it means the 
     *  plug-in was already recording audio from a previous call to this function.
     *  
     *  You should not need to call this function. It is called by the editor when the record
     *  button is pressed.
     */
    bool startRecording (const String& descriptors, const SAFEMetaData& metaData, bool newSendToServer);

    /** Returns true if the plug-in is currently recording audio. */
    bool isRecording();

    /** Returns true if the plug-in has finished recording and is ready to save. */
    bool isReadyToSave();

    /** Returns the size of the analysis frames in samples. 
     *
     *  Override this function to change the frame size.
     */
    virtual int getAnalysisFrameSize();

    /** Returns the step size between analysis frames in samples.
     *
     *  Override this function to change the step size.
     */
    virtual int getAnalysisStepSize();

    /** Returns the time the audio is analysed for in milliseconds.
     *
     *  Override this function to change the analysis time.
     */
    virtual int getAnalysisTime();

protected:
    //==========================================================================
    //      Methods to Create New Parameters
    //==========================================================================
    /** Add a new parameter to the plug-in.
     *
     *  This function is used to add parameters to your plug-in. It should be called during 
     *  your plug-in's constructor. Calling it at any other time could be pretty 
     *  dangerous because plug-ins shouldn't change their number of parameters willy nilly.
     *
     *  @param name                  the name of the parameter
     *  @param valueRef              a reference to a variable which will hold this
     *                               parameters value - give your plug-in a member variable
     *                               which will hold the value of this parameter and pass 
     *                               in here, that way you can access the value of the 
     *                               parameter without having to faff about calling function
     *  @param initialValue          the default value for this parameter
     *  @param minValue              the minimum value for this parameter
     *  @param maxValue              the maximum value for this parameter
     *  @param units                 a string to display after this parameters value
     *  @param skewFactor            the skew factor for this parameter - see
     *                               the documentation of skew factor in the JUCE 
     *                               slider class
     *  @param convertDBToGainValue  if true the parameters value will be treated as though
     *                               it were a gain in decibels and valueRef will be 
     *                               converted to a gain which can be applied to a signal
     *  @param interpolationTime     the time (in milliseconds) it takes for a parameter 
     *                               to change to a new value which was set
     *  @param UIScaleFactor         a factor to scale the parameter value by when
     *                               it is displayed in a user interface
     */
    void addParameter (String name, float& valueRef, float initialValue = 1, float minValue = 0, float maxValue = 1, String units = String::empty, float skewFactor = 1, bool convertDBToGainValue = false, double interpolationTime = 100, float UIScaleFactor = 1);

    /** Add a new parameter to the plugin.
     *
     *  The same as addParameter() but automatically sets convertDBToGainValue to true.
     */
    void addDBParameter (String name, float& valueRef, float initialValue = 1, float minValue = 0, float maxValue = 1, String units = String::empty, float skewFactor = 1, double interpolationTimeInit = 100, float UIScaleFactor = 1);

    //==========================================================================
    //      Play Head Stuff
    //==========================================================================
    AudioPlayHead::CurrentPositionInfo playHead; /**< The audio play head. */

    /** Update the play head. */
    void updatePlayHead(); 

    bool recording; /**< Recording flag, is true when the plug-in is recording audio. */
    bool readyToSave; /**< Ready to save flag, is true when the plug-in has finished recording
                           and is ready to save data. */

    //==========================================================================
    //      Multiple Channel Stuff
    //==========================================================================
    double fs;
    int numInputs; /**< The number of audio inputs. */
    int numOutputs; /**< The number of audio outputs. */

    /** Display a warning message on the currently active editor.
     *
     *  @param warning  the warning to display
     */
    void sendWarningToEditor (WarningID warning);

private:
    bool localRecording;

    OwnedArray <SAFEParameter> parameters;
    Array <float> parametersToSave;

    String descriptorsToSave;
    SAFEMetaData metaDataToSave;
    bool sendToServer;

    //==========================================================================
    //      Semantic Data File Stuff
    //==========================================================================
    File semanticDataFile;
    ScopedPointer <XmlElement> semanticDataElement;
    void updateSemanticDataElement();

    int numAnalysisFrames, currentUnprocessedAnalysisFrame, currentProcessedAnalysisFrame;
    int numSamplesToRecord;
    AudioSampleBuffer unprocessedBuffer, processedBuffer;
    int unprocessedTap, processedTap;
    int unprocessedSamplesToRecord, processedSamplesToRecord;

    SAFEFeatureExtractor unprocessedFeatureExtractor, processedFeatureExtractor;

    double controlRate;
    int controlBlockSize;
    int remainingControlBlockSamples;
    MidiBuffer midiControlBlock;

    #if JUCE_LINUX
    SharedResourcePointer <CurlHolder> curl;
    #endif

    //==========================================================================
    //      Recording Tests
    //==========================================================================
    void cacheCurrentParameters();
    bool haveParametersChanged();
    void timerCallback();
    void resetRecording();

    //==========================================================================
    //      Internal Semantic Data Parsing
    //==========================================================================

    /** Set up a local file for the descriptors to be saved to.
     *
     *  This should get put in the user's Documents directory in a new
     *  directory called SAFEPluginData.
     */
    void initialiseSemanticDataFile();

    /** Populate an XmlElement with the latest set of audio feature data.
     *
     *  @param element   a pointer to the XmlElement to populate
     *  @param metaData  the user's meta data to save alongside the feature data
     */
    WarningID populateXmlElementWithSemanticData (XmlElement* element, const SAFEMetaData& metaData);

    /** Save semantic data locally.
     *  
     *  @param newDescriptors  the descriptors to save
     *  @param metaData        the users meta data to save
     */
    WarningID saveSemanticData (const String& newDescriptors, const SAFEMetaData& metaData);

    /** Save semantic data to the server.
     *  
     *  @param newDescriptors  the descriptors to save
     *  @param metaData        the users meta data to save
     */
    WarningID sendDataToServer (const String& newDescriptors, const SAFEMetaData& metaData);

    /** Starts the thread which analysis the audio. */
    WarningID startAnalysisThread();

    //==========================================================================
    //      Buffer Playing Audio For Analysis
    //==========================================================================
    /** Records samples into the unprocessed recording buffer.
     *
     *  @param buffer  the samples to record
     *
     *  This is called in processBlock(), there should never really be a good reason for 
     *  you to call it yourself. Perhaps it should be private.
     */
    void recordUnprocessedSamples (AudioSampleBuffer& buffer);

    /** Records samples into the processed recording buffer.
     *
     *  @param buffer  the samples to record
     *
     *  This is called in processBlock(), there should never really be a good reason for 
     *  you to call it yourself. Perhaps it should be private.
     */
    void recordProcessedSamples (AudioSampleBuffer& buffer);

    //==========================================================================
    //      Analyse Buffered Audio
    //==========================================================================
    /** Analyses the samples in the recording buffers. */
    WarningID analyseRecordedSamples();

    //==========================================================================
    //      Make String ok for use in XML
    //==========================================================================
    String makeXmlString (String input);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SAFEAudioProcessor);
};

#endif  // __SAFEAUDIOPROCESSOR__
