#ifndef SAFE_FEATURE_EXTRACTOR_H_INCLUDED
#define SAFE_FEATURE_EXTRACTOR_H_INCLUDED

enum LibXtractFeature
{
    // All of them
    XtractAll,

    // Temporal Features
    XtractTemporalFeatures,
    XtractTemporalMean,
    XtractTemporalVariance,
    XtractTemporalStandardDeviation,
    XtractRMSAplitude,
    XtractZeroCrossingRate,
    
    // Spectral Features
    XtractSpectralFeatures,
    XtractSpectralCentriod,
    XtractSpectralVariance,
    XtractSpectralStandardDeviation,
    XtractSpectralSkewness,
    XtractSpectralKurtosis,
    XtractJensenIrregularity,
    XtractKrimphoffIrregularity,
    XtractFundamentalFrequency,
    XtractSpectralSmoothness,
    XtractSpectralRollOff,
    XtractSpectralFlatness,
    XtractTonality,
    XtractCrestFactor,
    XtractSpectralSlope,

    // Peak Spectral Features
    XtractPeakSpectralFeatures,
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
    XtractHarmonicSpectralFeatures,
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

    // Bark Coefficients
    XtractBarkCoefficients,
    XtractBarkCoefficient1,
    XtractBarkCoefficient2,
    XtractBarkCoefficient3,
    XtractBarkCoefficient4,
    XtractBarkCoefficient5,
    XtractBarkCoefficient6,
    XtractBarkCoefficient7,
    XtractBarkCoefficient8,
    XtractBarkCoefficient9,
    XtractBarkCoefficient10,
    XtractBarkCoefficient11,
    XtractBarkCoefficient12,
    XtractBarkCoefficient13,
    XtractBarkCoefficient14,
    XtractBarkCoefficient15,
    XtractBarkCoefficient16,
    XtractBarkCoefficient17,
    XtractBarkCoefficient18,
    XtractBarkCoefficient19,
    XtractBarkCoefficient20,
    XtractBarkCoefficient21,
    XtractBarkCoefficient22,
    XtractBarkCoefficient23,
    XtractBarkCoefficient24,
    XtractBarkCoefficient25,

    // MFCCs
    XtractMFCCs,
    XtractMFCC1,
    XtractMFCC2,
    XtractMFCC3,
    XtractMFCC4,
    XtractMFCC5,
    XtractMFCC6,
    XtractMFCC7,
    XtractMFCC8,
    XtractMFCC9,
    XtractMFCC10,
    XtractMFCC11,
    XtractMFCC12,
    XtractMFCC13
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
    /** Initialise the FFT and vamp plug-ins
     *
     *  @param numChannelsInit  the number of input channels
     *  @param frameOrderInit   the size of the analysis frames - this is given as an exponent
     *                          of two, the frame length in samples will be 2 ^ frameOrder
     *  @param stepSizeInit     the step size between analysis frames in samples - if this is
     *                          greater than the frame length it will be set to the 
     *                          frame length
     */
    void initialise (int numChannelsInit, int frameOrderInit, int stepSizeInit);


private:
    bool initialised;
    int numChannels, frameSize, stepSize;

    std::map <int, FFT> fftCache;
    const FFT *fft;
};

#endif // SAFE_FEATURE_EXTRACTOR_H_INCLUDED
