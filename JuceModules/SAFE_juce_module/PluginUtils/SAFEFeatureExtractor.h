#ifndef SAFE_FEATURE_EXTRACTOR_H_INCLUDED
#define SAFE_FEATURE_EXTRACTOR_H_INCLUDED

typedef Vamp::HostExt::PluginLoader VampPluginLoader;
typedef Vamp::HostExt::PluginLoader::PluginKey VampPluginKey;
typedef Vamp::Plugin VampPlugin;
typedef Vamp::RealTime VampTime;
typedef Vamp::Plugin::OutputList VampOutputList;
typedef Vamp::Plugin::OutputDescriptor VampOutputDescriptor;
typedef Vamp::Plugin::FeatureSet VampFeatureSet;
typedef Vamp::Plugin::FeatureList VampFeatureList;
typedef Vamp::Plugin::Feature VampFeature;

struct AudioFeature
{
    String name;
    int timeStamp;
    int channelNumber;
    Array <double> values;
    bool hasDuration;
    int duration;
};

/** 
 *  A class for extracting features from a block of audio.
 */
class SAFEFeatureExtractor
{
public:
    //==========================================================================
    //      Constructor and Destructor
    //==========================================================================
    /** Create a new feature extractor. */
    SAFEFeatureExtractor();

    /** Destructor */
    ~SAFEFeatureExtractor();

    //==========================================================================
    //      Setup
    //==========================================================================
    /** Initialise the feature extractor.
     *
     *  @param numChannelsInit  the number of input channels
     *  @param frameOrderInit   the size of the analysis frames - this is given as an exponent
     *                          of two, the frame length in samples will be 2 ^ frameOrder
     *  @param stepSizeInit     the step size between analysis frames in samples - if this is
     *                          greater than the frame length it will be set to the 
     *                          frame length
     *  @param sampleRate       the sample rate of the audio to be analysed
     */
    void initialise (int numChannelsInit, int defaultFrameSizeInit, int defaultStepSizeInit, double sampleRate);

    //==========================================================================
    //      Add Features
    //==========================================================================
    void addLibXtractFeature (LibXtract::Feature feature);
    void addVampPlugin (const String &libraryName, const String &pluginName);

    //==========================================================================
    //      Analyse Audio
    //==========================================================================
    void analyseAudio (AudioSampleBuffer &buffer);
    void setWindowingFunction (void (*newWindowingFunction) (float*, int));
    void addFeaturesToXmlElement (XmlElement *element);

private:
    bool initialised;
    int numChannels, defaultFrameSize, defaultStepSize;
    double fs;

    // some fft bits
    std::map <int, ScopedPointer <FFT> > fftCache;
    std::map <int, AudioSampleBuffer> spectraCache;

    void cacheNewFFT (int size);

    Array <AudioFeature> featureList;

    void calculateSpectra (const AudioSampleBuffer &frame);
    void (*windowingFunction) (float*, int);
    static void applyHannWindow (float *data, int numSamples);

    struct AnalysisConfiguration
    {
        int frameSize;
        int stepSize;
        bool libXtractConfiguration;
        Array <int> vampPluginIndicies;
    };

    OwnedArray <AnalysisConfiguration> analysisConfigurations;

    AnalysisConfiguration* addNewAnalysisConfiguration (int frameSize, int stepSize, bool libXtractConfiguration);
    void addVampPluginToAnalysisConfigurations (int pluginIndex, int frameSize, int stepSize);
    
    //==========================================================================
    //      libxtract stuff
    //==========================================================================
    bool libXtractSpectrumNeeded, libXtractPeakSpectrumNeeded, libXtractHarmonicSpectrumNeeded;
    bool calculateLibXtractScalarFeature [LibXtract::NumScalarFeatures];
    bool saveLibXtractScalarFeature [LibXtract::NumScalarFeatures];
    Array <Array <double> > libXtractScalarFeatureValues;

    static const int numLibXtractBarkBands = 25;
    HeapBlock <int> libXtractBarkBandLimits;
    bool calculateLibXtractBarkCoefficients;
    Array <Array <double> > libXtractBarkCoefficients;

    static const int numLibXtractMelFilters = 13;
    xtract_mel_filter libXtractMelFilters;
    bool libXtractMelFiltersInitialised;
    bool calculateLibXtractMFCCs;
    Array <Array <double> > libXtractMFCCs;
    void deleteLibXtractMelFilters();

    Array <Array <double> > libXtractSpectra, libXtractPeakSpectra, libXtractHarmonicSpectra;

    HeapBlock <double> libXtractChannelData;

    void initialiseLibXtract();
    void calculateLibXtractSpectra();
    void calculateLibXtractFeatures (const AudioSampleBuffer &frame);
    void addLibXtractFeaturesToList (Array <AudioFeature> &featureList, int timeStamp);

    //==========================================================================
    //      vamp stuff
    //==========================================================================
    VampPluginLoader *vampPluginLoader;
    Array <VampPluginKey> vampPluginKeys;
    OwnedArray <VampPlugin> vampPlugins;
    Array <VampOutputList> vampOutputs;

    void initialiseVampPlugins();
    void resetVampPlugins();
    void loadAndInitialiseVampPlugin (const VampPluginKey &key);
    void calculateVampPluginFeatures (const Array <int> &plugins, const AudioSampleBuffer &frame, int timeStamp);
    void getRemainingVampPluginFeatures();
    void addVampPluginFeaturesToList (VampOutputList &outputs, VampFeatureSet &features, int timeStamp);
    bool getVampPluginFeatureTimeAndDuration (AudioFeature &newFeature, 
                                              const VampOutputDescriptor &output,
                                              const VampFeature &feature,
                                              int timeStamp);
    int nextVampFeatureTimeStamp;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SAFEFeatureExtractor);
};

#endif // SAFE_FEATURE_EXTRACTOR_H_INCLUDED
