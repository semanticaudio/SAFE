#ifndef __SAFEAUDIOPROCESSOR__
#define __SAFEAUDIOPROCESSOR__

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
    };

    ScopedPointer <AnalysisThread> analysisThread;

public:
    //==========================================================================
    //      Constructor and Destructor
    //==========================================================================
    SAFEAudioProcessor();
    virtual ~SAFEAudioProcessor();
    
    //==========================================================================
    //      Parameter Info Methods
    //==========================================================================
    const String getName() const;

    int getNumParameters();

    float getParameter (int index);
    void setParameter (int index, float newValue);
    
    float getScaledParameter (int index);
    void setScaledParameter (int index, float newValue);
    void setScaledParameterNotifyingHost (int index, float newValue);

    float getGainParameter (int index);
    
    virtual void parameterUpdateCalculations (int /*index*/) {};

    const String getParameterName (int index) override;
    const String getParameterText (int index);
    
    const OwnedArray <SAFEParameter>& getParameterArray();

    //==========================================================================
    //      Other Plugin Info
    //==========================================================================
    const String getInputChannelName (int channelIndex) const;
    const String getOutputChannelName (int channelIndex) const;
    bool isInputChannelStereoPair (int index) const;
    bool isOutputChannelStereoPair (int index) const;

    bool acceptsMidi() const;
    bool producesMidi() const;
    bool silenceInProducesSilenceOut() const;
    double getTailLengthSeconds() const;

    //==========================================================================
    //      Program Stuff
    //==========================================================================
    int getNumPrograms();
    int getCurrentProgram();
    void setCurrentProgram (int index);
    const String getProgramName (int index);
    void changeProgramName (int index, const String& newName);

    //==========================================================================
    //      Saving and Loading Patches
    //==========================================================================
    void getStateInformation (MemoryBlock& destData);
    void setStateInformation (const void* data, int sizeInBytes);

    //==========================================================================
    //      Semantic Data Parsing
    //==========================================================================
    void initialiseSemanticDataFile();
    WarningID populateXmlElementWithSemanticData (XmlElement* element, const SAFEMetaData& metaData);
    WarningID saveSemanticData (const String& newDescriptors, const SAFEMetaData& metaData);
    WarningID loadSemanticData (const String& descriptor);
    WarningID sendDataToServer (const String& newDescriptors, const SAFEMetaData& metaData);
    WarningID getServerData (const String& descriptor);

    //==========================================================================
    //      Analysis Thread
    //==========================================================================
    WarningID startAnalysisThread();
    void sendWarningToEditor (WarningID warning);
    
    //==========================================================================
    //      Process Block
    //==========================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock);
    virtual void pluginPreparation (double sampleRate, int samplesPerBlock) = 0;
    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages);
    virtual void pluginProcessing (AudioSampleBuffer& buffer, MidiBuffer& midiMessages) = 0;

    //==========================================================================
    //      Playing & Recording Info
    //==========================================================================
    bool isPlaying();
    void startRecording (const String& descriptors, const SAFEMetaData& metaData, bool newSendToServer);
    bool isRecording();
    bool isReadyToSave();

protected:
    //==========================================================================
    //      Methods to Create New Parameters
    //==========================================================================
    void addParameter (String name, float& valueRef, float initialValue = 1, float minValue = 0, float maxValue = 1, String units = String::empty, float skewFactor = 1, bool convertDBToGainValue = false);

    void addDBParameter (String name, float& valueRef, float initialValue = 1, float minValue = 0, float maxValue = 1, String units = String::empty, float skewFactor = 1);

    //==========================================================================
    //      Buffer Playing Audio For Analysis
    //==========================================================================
    void recordUnprocessedSamples (AudioSampleBuffer& buffer);
    void recordProcessedSamples (AudioSampleBuffer& buffer);

    //==========================================================================
    //      Analyse Buffered Audio
    //==========================================================================
    WarningID analyseRecordedSamples();

    //==========================================================================
    //      Play Head Stuff
    //==========================================================================
    AudioPlayHead::CurrentPositionInfo playHead;
    void updatePlayHead();

    bool recording;
    bool readyToSave;

    //==========================================================================
    //      Multiple Channel Stuff
    //==========================================================================
    int numInputs;
    int numOutputs;

private:
    bool localRecording;

    OwnedArray <SAFEParameter> parameters;
    Array <float> parametersToSave;

    String descriptorsToSave;
    SAFEMetaData metaDataToSave;
    bool sendToServer;

    File semanticDataFile;
    ScopedPointer <XmlElement> semanticDataElement;

    static const int analysisTime = 5000;
    static const int analysisFrameLength = 4096;
    int numAnalysisFrames, currentUnprocessedAnalysisFrame, currentProcessedAnalysisFrame;
    int numSamplesToRecord;
    OwnedArray <Array <double> > unprocessedBuffer, processedBuffer;
    int unprocessedTap, processedTap;

    OwnedArray <SAFEFeatureExtractor> unprocessedFeatureExtractors, processedFeatureExtractors;

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
    //      Make String ok for use in XML
    //==========================================================================
    String makeXmlString (String input);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SAFEAudioProcessor);
};

#endif  // __SAFEAUDIOPROCESSOR__
