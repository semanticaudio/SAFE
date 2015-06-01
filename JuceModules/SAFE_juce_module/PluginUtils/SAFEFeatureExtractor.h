#ifndef SAFE_FEATURE_EXTRACTOR_H_INCLUDED
#define SAFE_FEATURE_EXTRACTOR_H_INCLUDED

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
    void initialise (int numChannelsInit, int frameSizeInit, int stepSizeInit, double sampleRate);

    //==========================================================================
    //      Add Features
    //==========================================================================
    void addLibXtractFeature (LibXtract::Feature feature);

    //==========================================================================
    //      Analyse Audio
    //==========================================================================
    void analyseAudio (AudioSampleBuffer &buffer);
    void addFeaturesToXmlElement (XmlElement *element);

private:
    bool initialised;
    int numChannels, frameSize, stepSize;
    double fs;

    std::map <int, ScopedPointer <FFT> > fftCache;
    const FFT *fft;
    bool spectrumNeeded;
    AudioSampleBuffer spectra;

    Array <AudioFeature> featureList;

    void calculateSpectra (const AudioSampleBuffer &frame);
    
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

    void calculateLibXtractSpectra();
    void calculateLibXtractFeatures (const AudioSampleBuffer &frame);
    void addLibXtractFeaturesToList (Array <AudioFeature> &featureList, int timeStamp);
};

#endif // SAFE_FEATURE_EXTRACTOR_H_INCLUDED
