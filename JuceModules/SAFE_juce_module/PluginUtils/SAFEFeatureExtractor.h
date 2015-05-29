#ifndef SAFE_FEATURE_EXTRACTOR_H_INCLUDED
#define SAFE_FEATURE_EXTRACTOR_H_INCLUDED

enum LibXtractFeature
{
    // Temporal Features
    XtractTemporalMean = 0,
    XtractTemporalVariance,
    XtractTemporalStandardDeviation,
    XtractRMSAmplitude,
    XtractZeroCrossingRate,
    
    // Spectral Features
    XtractFundamentalFrequency,
    XtractSpectralCentriod,
    XtractSpectralVariance,
    XtractSpectralStandardDeviation,
    XtractSpectralSkewness,
    XtractSpectralKurtosis,
    XtractJensenIrregularity,
    XtractKrimphoffIrregularity,
    XtractSpectralSmoothness,
    XtractSpectralRollOff,
    XtractSpectralFlatness,
    XtractTonality,
    XtractCrestFactor,
    XtractSpectralSlope,

    // Peak Spectral Features
    XtractPeakSpectralCentriod,
    XtractPeakSpectralVariance,
    XtractPeakSpectralStandardDeviation,
    XtractPeakSpectralSkewness,
    XtractPeakSpectralKurtosis,
    XtractPeakJensenIrregularity,
    XtractPeakKrimphoffIrregularity,
    XtractPeakTristimulus1,
    XtractPeakTristimulus2,
    XtractPeakTristimulus3,

    // Harmonic Spectral Features
    XtractInharmonicity,
    XtractHarmonicSpectralCentriod,
    XtractHarmonicSpectralVariance,
    XtractHarmonicSpectralStandardDeviation,
    XtractHarmonicSpectralSkewness,
    XtractHarmonicSpectralKurtosis,
    XtractHarmonicJensenIrregularity,
    XtractHarmonicKrimphoffIrregularity,
    XtractHarmonicTristimulus1,
    XtractHarmonicTristimulus2,
    XtractHarmonicTristimulus3,
    XtractNoisiness,
    XtractHarmonicParityRatio,

    // Number of Features
    NumLibXtractScalarFeatures,

    // vector features
    XtractBarkCoefficients,
    XtractMFCCs,

    // Feature Groups
    XtractTemporalFeatures,
    XtractSpectralFeatures,
    XtractPeakSpectralFeatures,
    XtractHarmonicSpectralFeatures,
    XtractAll
};

struct AudioFeature
{
    String Name;
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
    void initialise (int numChannelsInit, int frameOrderInit, int stepSizeInit, double sampleRate);

    //==========================================================================
    //      Add Features
    //==========================================================================
    void addLibXtractFeature (LibXtractFeature feature);

private:
    bool initialised;
    int numChannels, frameSize, stepSize;
    double fs;

    std::map <int, ScopedPointer <FFT> > fftCache;
    const FFT *fft;
    bool spectrumNeeded;
    AudioSampleBuffer spectra;

    void calculateSpectra (const AudioSampleBuffer &frame);
    
    //==========================================================================
    //      libxtract stuff
    //==========================================================================
    bool libXtractSpectrumNeeded, libXtractPeakSpectrumNeeded, libXtractHarmonicSpectrumNeeded;
    bool calculateLibXtractScalarFeature [NumLibExtractScalardFeatures];
    bool saveLibXtractScalarFeature [NumLibExtractScalardFeatures];
    Array <Array <double> > libXtractScalarFeatureValues [NumLibXtractScalarFeatures];

    static int numLibXtractBarkBands = 25;
    HeapBlock <double> libXtractBarkBandLimits;
    bool calculateLibXtractBarkCoefficients;
    Array <Array <double> > libXtractBarkCoefficients;

    static int numLibXtractMelFilters = 13;
    xtract_mel_filter libXtractMelFilters;
    bool libXtractMelFiltersInitialised;
    bool calculateLibXtractMFCCs;
    Array <Array <double> > libXtractMFCCs;
    void deleteLibXtractMelFilters();

    Array <Array <double> > libXtractSpectra, libXtractPeakSpectra, libXtractHarmonicSpectra;

    HeapBlock <double> libXtractChannelData;

    void calculateLibXtractSpectra();
    void calculateLibXtractFeatures (const AudioSampleBuffer &frame);
};

#endif // SAFE_FEATURE_EXTRACTOR_H_INCLUDED
