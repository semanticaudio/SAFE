//==========================================================================
//      A Class to Put the Analysis on a Separate Thread
//==========================================================================
//==========================================================================
//      Constructor and Destructor
//==========================================================================
SAFEAudioProcessor::AnalysisThread::AnalysisThread (SAFEAudioProcessor* processorInit)
    : Thread ("AnalysisThread")
{
    processor = processorInit;
}

SAFEAudioProcessor::AnalysisThread::~AnalysisThread()
{
    stopThread (4000);
}

//==========================================================================
//      The Thread Callback
//==========================================================================
void SAFEAudioProcessor::AnalysisThread::run()
{
    WarningID warning;

    if (sendToServer)
    {
        warning = processor->sendDataToServer (descriptors, metaData);
    }
    else
    {
        warning = processor->saveSemanticData (descriptors, metaData);
    }

    if (warning != NoWarning)
    {
        processor->sendWarningToEditor (warning);
    }
}

//==========================================================================
//      Set Some Parameters
//==========================================================================
void SAFEAudioProcessor::AnalysisThread::setParameters (String newDescriptors, SAFEMetaData newMetaData, bool newSendToServer)
{
    descriptors = newDescriptors;
    metaData = newMetaData;
    sendToServer = newSendToServer;
}

//==========================================================================
//      The Processor Itself
//==========================================================================
//==========================================================================
//      Constructor and Destructor
//==========================================================================
SAFEAudioProcessor::SAFEAudioProcessor()
{
    // reset tap values
    unprocessedTap = processedTap = 0;

    // get the semantic data file set up
    initialiseSemanticDataFile();

    playHead.resetToDefault();

    recording = false;
    readyToSave = true;

    numInputs = 1;
    numOutputs = 1;

    analysisThread = new AnalysisThread (this);

    controlRate = 64;
    controlBlockSize = (int) (44100.0 / controlRate);
    remainingControlBlockSamples = 0;
}

SAFEAudioProcessor::~SAFEAudioProcessor()
{
}

//==========================================================================
//      Parameter Info Methods
//==========================================================================
const String SAFEAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int SAFEAudioProcessor::getNumParameters()
{
    return parameters.size();
}

float SAFEAudioProcessor::getParameter (int index)
{
    return parameters [index]->getBaseValue();
}

void SAFEAudioProcessor::setParameter (int index, float newValue)
{
    parameters [index]->setBaseValue (newValue);
    
    parameterUpdateCalculations (index);
}

float SAFEAudioProcessor::getScaledParameter (int index)
{
    return parameters [index]->getScaledValue();
}

void SAFEAudioProcessor::setScaledParameter (int index, float newValue)
{
    parameters [index]->setScaledValue (newValue);
    
    parameterUpdateCalculations (index);
}

void SAFEAudioProcessor::setScaledParameterNotifyingHost (int index, float newValue)
{
    setScaledParameter (index, newValue);
    float newBaseValue = parameters [index]->getBaseValue();
    sendParamChangeMessageToListeners (index, newBaseValue);
}

float SAFEAudioProcessor::getGainParameter (int index)
{
    return parameters [index]->getGainValue();
}

const String SAFEAudioProcessor::getParameterName (int index)
{
    return parameters [index]->getName();
}

const String SAFEAudioProcessor::getParameterText (int index)
{
    SAFEParameter* info = parameters [index];
    return String (info->getScaledValue(), 2) + info->getUnits();
}

const OwnedArray <SAFEParameter>& SAFEAudioProcessor::getParameterArray()
{
    return parameters;
}

//==========================================================================
//      Other Plugin Info
//==========================================================================
const String SAFEAudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String SAFEAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool SAFEAudioProcessor::isInputChannelStereoPair (int /*index*/) const
{
    return true;
}

bool SAFEAudioProcessor::isOutputChannelStereoPair (int /*index*/) const
{
    return true;
}

bool SAFEAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SAFEAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SAFEAudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double SAFEAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

//==========================================================================
//      Program Stuff
//==========================================================================
int SAFEAudioProcessor::getNumPrograms()
{
    return 0;
}

int SAFEAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SAFEAudioProcessor::setCurrentProgram (int /*index*/)
{
}

const String SAFEAudioProcessor::getProgramName (int /*index*/)
{
    return String::empty;
}

void SAFEAudioProcessor::changeProgramName (int /*index*/, const String& /*newName*/)
{
}

//==========================================================================
//      Saving and Loading Patches
//==========================================================================
void SAFEAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    XmlElement xml (makeXmlString(JucePlugin_Name + String ("Settings")));

    for (int parameterNum = 0; parameterNum < parameters.size(); ++parameterNum)
    {
        xml.setAttribute ("Parameter" + String (parameterNum), parameters [parameterNum]->getBaseValue());
    }

    copyXmlToBinary (xml, destData);
}

void SAFEAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState != nullptr)
    {
        if (xmlState->hasTagName (makeXmlString (JucePlugin_Name + String ("Settings"))))
        {
            for (int parameterNum = 0; parameterNum < parameters.size(); ++parameterNum)
            {
                setParameterNotifyingHost (parameterNum, (float) xmlState->getDoubleAttribute ("Parameter" + String (parameterNum), parameters [parameterNum]->getBaseValue()));
            }
        }
    }
}

//==========================================================================
//      Semantic Data Parsing
//==========================================================================
void SAFEAudioProcessor::initialiseSemanticDataFile()
{
    File documentsDirectory (File::getSpecialLocation (File::userDocumentsDirectory));

    File dataDirectory (documentsDirectory.getChildFile ("SAFEPluginData"));

    if (! dataDirectory.isDirectory())
    {
        dataDirectory.createDirectory();
    }

    semanticDataFile = dataDirectory.getChildFile (JucePlugin_Name + String ("Data.xml"));

    if (semanticDataFile.exists())
    {
        XmlDocument semanticDataDocument (semanticDataFile);
        semanticDataElement = semanticDataDocument.getDocumentElement();
    }
    else
    {
        String elementName (JucePlugin_Name + String ("Data"));
        semanticDataElement = new XmlElement (makeXmlString (elementName));
    }
}

WarningID SAFEAudioProcessor::populateXmlElementWithSemanticData (XmlElement* element, const SAFEMetaData& metaData)
{
    // analyse the buffered samples
    WarningID warning = analyseRecordedSamples();

    if (warning != NoWarning)
    {
        return warning;
    }

    // save the channel configuration
    XmlElement* configElement = element->createNewChildElement ("ChannelConfiguration");

    configElement->setAttribute ("Inputs", numInputs);
    configElement->setAttribute ("Outputs", numOutputs);

    // save the parameter settings
    XmlElement* parametersElement = element->createNewChildElement ("ParameterSettings");

    for (int parameterNum = 0; parameterNum < parameters.size(); ++parameterNum)
    {
        SAFEParameter* currentParameter = parameters [parameterNum];
        String xmlParameterName = makeXmlString (currentParameter->getName());
        float currentParameterValue = parametersToSave [parameterNum];

        parametersElement->setAttribute (xmlParameterName, currentParameterValue);
    }

    // allocate some memory for checksum data
    MemoryBlock channelChecksums;

    // save the unprocessed audio features
    XmlElement* unprocessedFeaturesElement = element->createNewChildElement ("UnprocessedAudioFeatures");
    
    for (int inputChannel = 0; inputChannel < numInputs; ++inputChannel)
    {
        String channelName = String ("Channel") + String (inputChannel);

        XmlElement* unprocessedChannelElement = unprocessedFeaturesElement->getChildByName (channelName);

        if (! unprocessedChannelElement)
        {
            unprocessedChannelElement = unprocessedFeaturesElement->createNewChildElement (channelName);
        }

        unprocessedFeatureExtractors [inputChannel]->addToXml (unprocessedChannelElement);

        channelChecksums.append (unprocessedFeatureExtractors [inputChannel]->getMD5Checksum().getData(), 16);
    }

    // save the processed audio features
    XmlElement* processedFeaturesElement = element->createNewChildElement ("ProcessedAudioFeatures");

    for (int outputChannel = 0; outputChannel < numOutputs; ++outputChannel)
    {
        String channelName = String ("Channel") + String (outputChannel);

        XmlElement* processedChannelElement = processedFeaturesElement->getChildByName (channelName);

        if (! processedChannelElement)
        {
            processedChannelElement = processedFeaturesElement->createNewChildElement (channelName);
        }

        processedFeatureExtractors [outputChannel]->addToXml (processedChannelElement);

        channelChecksums.append (processedFeatureExtractors [outputChannel]->getMD5Checksum().getData(), 16);
    }

    // save the meta data
    XmlElement* metaDataElement = element->createNewChildElement ("MetaData");

    metaDataElement->setAttribute ("Genre", metaData.genre);
    metaDataElement->setAttribute ("Instrument", metaData.instrument);
    metaDataElement->setAttribute ("Location", metaData.location);
    metaDataElement->setAttribute ("Experience", metaData.experience);
    metaDataElement->setAttribute ("Age", metaData.age);
    metaDataElement->setAttribute ("Language", metaData.language);

    // save a checksum of the audio features
    XmlElement* checksumElement = element->createNewChildElement ("Checksum");
    MD5 featureChecksum (channelChecksums);
    checksumElement->setAttribute ("Checksum", featureChecksum.toHexString());

    return warning;
}

WarningID SAFEAudioProcessor::saveSemanticData (const String& newDescriptors, const SAFEMetaData& metaData)
{
    // separate different descriptors
    StringArray descriptors;
    descriptors.addTokens (newDescriptors, " ,;", String::empty);
    int numDescriptors = descriptors.size();

    XmlElement* descriptorElement = semanticDataElement->createNewChildElement ("SemanticData");

    for (int descriptor = 0; descriptor < numDescriptors; ++descriptor)
    {
        String descriptorString = descriptors [descriptor];
        String descriptorName = String ("Descriptor") + String (descriptor);

        descriptorElement->setAttribute (descriptorName, descriptorString);
    }
    
    WarningID warning = populateXmlElementWithSemanticData (descriptorElement, metaData);

    if (warning != NoWarning)
    {
        return warning;
    }

    // save to file
    semanticDataElement->writeToFile (semanticDataFile, "");

    return warning;
}

WarningID SAFEAudioProcessor::loadSemanticData (const String& descriptor)
{
    StringArray descriptorArray;
    descriptorArray.addTokens (descriptor, " ,;", String::empty);

    if (descriptorArray.size() > 0)
    {
        String firstDescriptor = descriptorArray [0];

        if (firstDescriptor.containsNonWhitespaceChars())
        {
            // go through XML elements and look for the first one with the descriptor we want
            forEachXmlChildElement (*semanticDataElement, descriptorElement)
            {
                int numAttributes = descriptorElement->getNumAttributes();

                for (int attribute = 0; attribute < numAttributes; ++ attribute)
                {
                    String attributeValue = descriptorElement->getAttributeValue (attribute);

                    if (attributeValue == firstDescriptor)
                    {
                        XmlElement* parametersElement = descriptorElement->getChildByName ("ParameterSettings");

                        for (int parameterNum = 0; parameterNum < parameters.size(); ++parameterNum)
                        {
                            SAFEParameter* currentParameter = parameters [parameterNum];
                            String xmlParameterName = makeXmlString (currentParameter->getName());
                            float newParameterValue = (float) parametersElement->getDoubleAttribute (xmlParameterName);
                            
                            setScaledParameterNotifyingHost (parameterNum, newParameterValue);
                        }

                        return NoWarning;
                    }
                }
            }
        }
    }

    return DescriptorNotInFile;
}

WarningID SAFEAudioProcessor::sendDataToServer (const String& newDescriptors, const SAFEMetaData& metaData)
{
    XmlElement tempDataElement (JucePlugin_Name);

    XmlElement* descriptorElement = tempDataElement.createNewChildElement ("SemanticData");

    descriptorElement->setAttribute ("Descriptors", newDescriptors);

    WarningID warning = populateXmlElementWithSemanticData (descriptorElement, metaData);

    if (warning != NoWarning)
    {
        return warning;
    }
    
    // save to file
    File documentsDirectory (File::getSpecialLocation (File::userDocumentsDirectory));
    File dataDirectory (documentsDirectory.getChildFile ("SAFEPluginData"));

    File tempDataFile = dataDirectory.getChildFile ("tempData.xml");

    tempDataElement.writeToFile (tempDataFile, "");

    #if JUCE_LINUX
    CURLcode res;
 
    struct curl_httppost *formpost=NULL;
    struct curl_httppost *lastptr=NULL;
    struct curl_slist *headerlist=NULL;
    static const char buf[] = "Expect:";
 
    /* Fill in the file upload field */ 
    curl_formadd(&formpost,
                 &lastptr,
                 CURLFORM_COPYNAME, "DataFile",
                 CURLFORM_FILE, tempDataFile.getFullPathName().toRawUTF8(),
                 CURLFORM_END);
 
    /* Fill in the filename field */ 
    curl_formadd(&formpost,
                 &lastptr,
                 CURLFORM_COPYNAME, "DataFile",
                 CURLFORM_COPYCONTENTS, tempDataFile.getFullPathName().toRawUTF8(),
                 CURLFORM_END);
 
 
    /* Fill in the submit field too, even if this is rarely needed */ 
    curl_formadd(&formpost,
                 &lastptr,
                 CURLFORM_COPYNAME, "submit",
                 CURLFORM_COPYCONTENTS, "send",
                 CURLFORM_END);
 
    headerlist = curl_slist_append(headerlist, buf);
    if(curl) 
    {
        curl_easy_setopt(curl->curl, CURLOPT_URL, "http://193.60.133.151/SAFE/fileUpload.php");
        curl_easy_setopt(curl->curl, CURLOPT_HTTPPOST, formpost);
 
        res = curl_easy_perform(curl->curl);
 
        curl_formfree(formpost);
        curl_slist_free_all (headerlist);
    }
    #else
    URL dataUpload ("http://193.60.133.151/SAFE/fileUpload.php");
    dataUpload = dataUpload.withFileToUpload ("DataFile", tempDataFile, "text/xml");

    ScopedPointer <InputStream> stream (dataUpload.createInputStream (true));
    String serverResponse = stream->readEntireStreamAsString();
    #endif

    tempDataFile.deleteFile();

    return warning;
}

WarningID SAFEAudioProcessor::getServerData (const String& descriptor)
{
    // get descriptors
    StringArray descriptorArray;
    descriptorArray.addTokens (descriptor, " ,;", String::empty);
    descriptorArray.removeEmptyStrings();

    if (descriptorArray.size() > 0)
    {
        // we just take the first descriptor in the box
        // This will be changed to a more comprehensive model soon...
        String firstDescriptor = descriptorArray [0];
        
        // send call to script with user descriptor...
        String userDataTableName = JucePlugin_Name + String ("UserData");
        URL downloadParamData ("http://193.60.133.151/SAFE/download.php");
        downloadParamData = downloadParamData.withParameter(String("TableName"),   userDataTableName);
        downloadParamData = downloadParamData.withParameter(String("Descriptors"), firstDescriptor);
        
        // just returns an ordered list of strings for the params...
        String dbOutput = downloadParamData.readEntireTextStream();
        
        //return the param values from the download.php script.
        StringArray outputStringArray, fieldNames, paramValues;
        outputStringArray.addTokens(dbOutput, ",");
        outputStringArray.removeEmptyStrings();
        
        if (outputStringArray.size()>0) // if the term exists
        {
            // seperate the strings into fields and paramValues.
            for (int i=0; i<outputStringArray.size()-1; i+=2)
            {
                fieldNames.add(outputStringArray[i].removeCharacters(", "));
                paramValues.add(outputStringArray[i+1].removeCharacters(", "));
            }
            
            for (int parameterNum = 0; parameterNum < parameters.size(); ++parameterNum)
            {
                String tempFieldName = String ("Param_") + makeXmlString (parameters [parameterNum]->getName());
                int index = fieldNames.indexOf (tempFieldName);

                float newParameterValue = String (paramValues[index]).getFloatValue();
                setScaledParameterNotifyingHost (parameterNum, newParameterValue);
            }
            return NoWarning;
        }
        else
        {
            return DescriptorNotOnServer;
        }
    }
    else
    {
        return DescriptorNotOnServer;
    }
}

//==========================================================================
//      Analysis Thread
//==========================================================================
WarningID SAFEAudioProcessor::startAnalysisThread()
{
    if (analysisThread->isThreadRunning())
    {
        resetRecording();
        sendWarningToEditor (AnalysisThreadBusy);
        return AnalysisThreadBusy;
    }
    else
    {
        analysisThread->setParameters (descriptorsToSave, metaDataToSave, sendToServer);
        analysisThread->startThread();
        resetRecording();
    }

    return NoWarning;
}

void SAFEAudioProcessor::sendWarningToEditor (WarningID warning)
{
    SAFEAudioProcessorEditor* editor = static_cast <SAFEAudioProcessorEditor*> (getActiveEditor());

    if (editor)
    {
        editor->flagWarning (warning);
    }
}

//==========================================================================
//      Process Block
//==========================================================================
void SAFEAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // get the channel configuration
    numInputs = getNumInputChannels();
    numOutputs = getNumOutputChannels();

    // work out how many frames we will get in the analysis time 
    int samplesInRecording = (int) floor (sampleRate * analysisTime / 1000);
    numAnalysisFrames = (int) floor ((float) (samplesInRecording / analysisFrameLength));
    numSamplesToRecord = numAnalysisFrames * analysisFrameLength;

    // set up analysis buffers
    unprocessedBuffer.clear();
    unprocessedFeatureExtractors.clear();

    for (int inputChannel = 0; inputChannel < numInputs; ++inputChannel)
    {
        unprocessedBuffer.add (new Array <double>);
        unprocessedBuffer [inputChannel]->resize (numSamplesToRecord);

        unprocessedFeatureExtractors.add (new SAFEFeatureExtractor);
        unprocessedFeatureExtractors [inputChannel]->initialise (numAnalysisFrames, analysisFrameLength, sampleRate);
    }

    processedBuffer.clear();
    processedFeatureExtractors.clear();

    for (int outputChannel = 0; outputChannel < numOutputs; ++outputChannel)
    {
        processedBuffer.add (new Array <double>);
        processedBuffer [outputChannel]->resize (numSamplesToRecord);

        processedFeatureExtractors.add (new SAFEFeatureExtractor);
        processedFeatureExtractors [outputChannel]->initialise (numAnalysisFrames, analysisFrameLength, sampleRate);
    }

    for (int i = 0; i < parameters.size(); ++i)
    {
        parameters [i]->setSampleRate (sampleRate);
    }

    controlBlockSize = (int) (sampleRate / controlRate);
    midiControlBlock.ensureSize (2048);
    midiControlBlock.clear();
    
    // call any prep the plugin processing wants to do
    pluginPreparation (sampleRate, samplesPerBlock);
}

void SAFEAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    localRecording = recording;

    recordUnprocessedSamples (buffer);

    // call the plugin dsp
    bool parametersInterpolating = false;

    for (int i = 0; i < parameters.size(); ++i)
    {
        parametersInterpolating |= parameters [i]->isInterpolating();
    }

    if (parametersInterpolating)
    {
        int numChannels = buffer.getNumChannels();
        int numSamples = buffer.getNumSamples();

        if (numSamples < remainingControlBlockSamples)
        {
            AudioSampleBuffer controlBlock (buffer.getArrayOfWritePointers(), numChannels, numSamples);

            midiControlBlock.clear();
            midiControlBlock.addEvents (midiMessages, 0, numSamples, 0);

            pluginProcessing (controlBlock, midiControlBlock);

            remainingControlBlockSamples -= numSamples;
        }
        else
        {
            if (remainingControlBlockSamples)
            {
                AudioSampleBuffer controlBlock (buffer.getArrayOfWritePointers(), numChannels, remainingControlBlockSamples);

                midiControlBlock.clear();
                midiControlBlock.addEvents (midiMessages, 0, remainingControlBlockSamples, 0);

                pluginProcessing (controlBlock, midiControlBlock);
            }
        
            int numControlBlocks = (int) ((numSamples - remainingControlBlockSamples) / controlBlockSize);
            int sampleNumber = remainingControlBlockSamples;

            for (int block = 0; block < numControlBlocks; ++block)
            {
                for (int i = 0; i < parameters.size(); ++i)
                {
                    parameters [i]->smoothValues();
                    parameterUpdateCalculations (i);
                }

                AudioSampleBuffer controlBlock (buffer.getArrayOfWritePointers(), numChannels, sampleNumber, controlBlockSize);

                midiControlBlock.clear();
                midiControlBlock.addEvents (midiMessages, sampleNumber, controlBlockSize, 0);

                pluginProcessing (controlBlock, midiControlBlock);

                sampleNumber += controlBlockSize;
            }

            int samplesLeft = numSamples - sampleNumber;

            if (samplesLeft)
            {
                for (int i = 0; i < parameters.size(); ++i)
                {
                    parameters [i]->smoothValues();
                    parameterUpdateCalculations (i);
                }

                AudioSampleBuffer controlBlock (buffer.getArrayOfWritePointers(), numChannels, sampleNumber, samplesLeft);

                midiControlBlock.clear();
                midiControlBlock.addEvents (midiMessages, sampleNumber, samplesLeft, 0);

                pluginProcessing (controlBlock, midiControlBlock);
            }

            remainingControlBlockSamples = controlBlockSize - samplesLeft;
        }
    }
    else
    {
        pluginProcessing (buffer, midiMessages);
        remainingControlBlockSamples = 0;
    }

    // In case we have more outputs than inputs, we'll clear any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
    {
        buffer.clear (i, 0, buffer.getNumSamples());
    }

    updatePlayHead();
    recordProcessedSamples (buffer);
}

//==========================================================================
//      Playing & Recording Info
//==========================================================================
bool SAFEAudioProcessor::isPlaying()
{
    return playHead.isPlaying;
}

void SAFEAudioProcessor::startRecording (const String& descriptors, const SAFEMetaData& metaData, bool newSendToServer)
{
    currentUnprocessedAnalysisFrame = 0;
    currentProcessedAnalysisFrame = 0;
    unprocessedTap = 0;
    processedTap = 0;

    descriptorsToSave = descriptors;
    metaDataToSave = metaData;
    sendToServer = newSendToServer;
    cacheCurrentParameters();

    recording = true;
    readyToSave = false;

    startTimer (50);
}

bool SAFEAudioProcessor::isRecording()
{
    return recording;
}

bool SAFEAudioProcessor::isReadyToSave()
{
    return readyToSave;
}

//==========================================================================
//      Methods to Create New Parameters
//==========================================================================
void SAFEAudioProcessor::addParameter (String name, float& valueRef, float initialValue, float minValue, float maxValue, String units, float skewFactor, bool convertDBToGainValue, double interpolationTime)
{
    parameters.add (new SAFEParameter (name, valueRef, initialValue, minValue, maxValue, units, skewFactor, convertDBToGainValue, interpolationTime));

    parametersToSave.add (0);
}

void SAFEAudioProcessor::addDBParameter (String name, float& valueRef, float initialValue, float minValue, float maxValue, String units, float skewFactor, double interpolationTime)
{
    parameters.add (new SAFEParameter (name, valueRef, initialValue, minValue, maxValue, units, skewFactor, true, interpolationTime));

    parametersToSave.add (0);
}

//==========================================================================
//      Buffer Playing Audio For Analysis
//==========================================================================
void SAFEAudioProcessor::recordUnprocessedSamples (AudioSampleBuffer& buffer)
{
    if (localRecording)
    {
        int numSamples = buffer.getNumSamples();

        for (int i = 0; i < numSamples; ++i)
        {
            for (int inputChannel = 0; inputChannel < numInputs; ++inputChannel)
            {
                unprocessedBuffer [inputChannel]->set (unprocessedTap, buffer.getSample (inputChannel, i));
            }

            ++unprocessedTap;

            if (unprocessedTap >= numSamplesToRecord)
            {
                break;
            }
        }
    }
}

void SAFEAudioProcessor::recordProcessedSamples (AudioSampleBuffer& buffer)
{
    if (localRecording)
    {
        int numSamples = buffer.getNumSamples();

        for (int i = 0; i < numSamples; ++i)
        {
            for (int outputChannel = 0; outputChannel < numOutputs; ++outputChannel)
            {
                processedBuffer [outputChannel]->set (processedTap, buffer.getSample (outputChannel, i));
            }

            ++processedTap;

            if (processedTap >= numSamplesToRecord)
            {
                startAnalysisThread();
                break;
            }
        }
    }
}

//==========================================================================
//      Buffer Playing Audio For Analysis
//==========================================================================
WarningID SAFEAudioProcessor::analyseRecordedSamples()
{
    for (int frameNum = 0; frameNum < numAnalysisFrames; ++frameNum)
    {
        for (int inputChannel = 0; inputChannel < numInputs; ++inputChannel)
        {
            double* unprocessedSamples = unprocessedBuffer [inputChannel]->getRawDataPointer() + analysisFrameLength * frameNum;

            unprocessedFeatureExtractors [inputChannel]->getAllFeatures (unprocessedSamples, analysisFrameLength, frameNum);
        }

        for (int outputChannel = 0; outputChannel < numOutputs; ++outputChannel)
        {
            double* processedSamples = processedBuffer [outputChannel]->getRawDataPointer() + analysisFrameLength * frameNum;

            processedFeatureExtractors [outputChannel]->getAllFeatures (processedSamples, analysisFrameLength, frameNum);
        }
    }

    bool signalUnprocessed = true;

    for (int channel = 0; channel < numInputs; ++channel)
    {   
        signalUnprocessed = signalUnprocessed && * (unprocessedFeatureExtractors [channel]) == * (processedFeatureExtractors [channel]);
    }

    if (signalUnprocessed)
    {
        return AudioNotProcessed;
    }
    else
    {
        return NoWarning;
    }
}

//==========================================================================
//      Play Head Stuff
//==========================================================================
void SAFEAudioProcessor::updatePlayHead()
{
    AudioPlayHead::CurrentPositionInfo newPlayHead;
    
    if (getPlayHead() != nullptr && getPlayHead()->getCurrentPosition (newPlayHead))
    {
        playHead = newPlayHead;
    }
    else
    {
        playHead.resetToDefault();
    }
}

//==========================================================================
//      Recording Tests
//==========================================================================
void SAFEAudioProcessor::cacheCurrentParameters()
{
    for (int i = 0; i < parameters.size(); ++i)
    {
        parametersToSave.set (i, parameters [i]->getScaledValue());
    }
}

bool SAFEAudioProcessor::haveParametersChanged()
{
    bool returnValue = false;

    for (int i = 0; i < parameters.size(); ++i)
    {
        if (parametersToSave [i] != parameters [i]->getScaledValue())
        {
            returnValue = true;
            break;
        }
    }

    return returnValue;
}

void SAFEAudioProcessor::timerCallback()
{
    if (haveParametersChanged())
    {
        resetRecording();
        sendWarningToEditor (ParameterChange);
    }

    if (! isPlaying())
    {
        resetRecording();
        sendWarningToEditor (AudioNotPlaying);
    }
}

void SAFEAudioProcessor::resetRecording()
{
    recording = false;
    readyToSave = true;
    stopTimer();
}

//==========================================================================
//      Make String ok for use in XML
//==========================================================================
String SAFEAudioProcessor::makeXmlString (String input)
{
    return input.retainCharacters ("1234567890qwertyuioplkjhgfdsazxcvbnmMNBVCXZASDFGHJKLPOIUYTREWQ:-_");
}
