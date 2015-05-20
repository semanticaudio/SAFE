#ifndef __SAFEFEATUREEXTRACTOR__
#define __SAFEFEATUREEXTRACTOR__

/**
 *  A class for extracting features from audio using libXtract.
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

    /** Initialise the feature extractor.
     *  
     *  @param numAnalysisFramesInit     The number of analysis frames.
     *  @param analysisFramesLengthInit  The number of samples in each analysis frame.
     *  @param sampleRate                The sample rate of the audio to analyse.
     */
    void initialise (int numAnalysisFramesInit, int analysisFrameLengthInit, double sampleRate);

    /** Analyse a frame of audio.
     *
     *  @param sampleData  a pointer to an array containing the audio samples to analyse
     *  @param numSamples  the number of samples to analyse - this should be the same as
     *                     the value for analysisFrameLengthInit passed to initialise()
     *  @param frameNum    the frame number of the current frame - this should
     *                     be between 0 and the value for numAnalysisFrameInit passed
     *                     to initialise() - 1
     */
    void getAllFeatures (double* sampleData, int numSamples, int frameNum);

    /** Add all the audio features from a set of frames to an XMLElement
     *
     *  @param parentElement  a pointer to the XMLElement to add the audio features to
     */
    void addToXml (XmlElement* parentElement);

    /** Get an array filled with the audio features for a particular frame.
     *  
     *  @param frameNum  the frame number to get the features of - this should
     *                   be between 0 and the value for numAnalysisFrameInit passed
     *                   to initialise() - 1
     *
     *  The features are as follows:
     *  - Temporal Mean
     *  - Temporal Variance
     *  - Temporal Standard Deviation
     *  - RMS Amplitude
     *  - Zero Crossing Rate
     *  - Spectral Centriod
     *  - Spectral Variance
     *  - Spectral Standard Deviation
     *  - Spectral Skewness
     *  - Spectral Kurtosis
     *  - Jensen Irregularity
     *  - Krimphoff Irregularity
     *  - Fundamental Frequency
     *  - Spectral Smoothness
     *  - Spectral Roll Off
     *  - Spectral Flatness
     *  - Tonality
     *  - Crest Factor
     *  - Spectral Slope
     *  - Peak Spectral Centroid
     *  - Peak Spectral Variance
     *  - Peak Spectral Standard Deviation
     *  - Peak Spectral Skewness
     *  - Peak Spectral Kurtosis
     *  - Peak Spectral Jensen Irregularity
     *  - Peak Spectral Krimphoff Irregularity
     *  - Peak Tristimulus 1
     *  - Peak Tristimulus 2
     *  - Peak Tristimulus 3
     *  - Inharmonicity
     *  - Harmonic Spectral Centroid
     *  - Harmonic Spectral Variance
     *  - Harmonic Spectral Standard Deviation
     *  - Harmonic Spectral Skewness
     *  - Harmonic Spectral Kurtosis
     *  - Harmonic Spectral Jensen Irregularity
     *  - Harmonic Spectral Krimphoff Irregularity
     *  - Harmonic Tristimulus 1
     *  - Harmonic Tristimulus 2
     *  - Harmonic Tristimulus 3
     *  - Noisiness
     *  - Parity Ratio (ratio of odd and even harmonics in the signal)
     *  - Bark Coefficient 1
     *  - Bark Coefficient 2
     *  - Bark Coefficient 3
     *  - Bark Coefficient 4
     *  - Bark Coefficient 5
     *  - Bark Coefficient 6
     *  - Bark Coefficient 7
     *  - Bark Coefficient 8
     *  - Bark Coefficient 9
     *  - Bark Coefficient 10
     *  - Bark Coefficient 11
     *  - Bark Coefficient 12
     *  - Bark Coefficient 13
     *  - Bark Coefficient 14
     *  - Bark Coefficient 15
     *  - Bark Coefficient 16
     *  - Bark Coefficient 17
     *  - Bark Coefficient 18
     *  - Bark Coefficient 19
     *  - Bark Coefficient 20
     *  - Bark Coefficient 21
     *  - Bark Coefficient 22
     *  - Bark Coefficient 23
     *  - Bark Coefficient 24
     *  - Bark Coefficient 25
     *  - MFCC 1
     *  - MFCC 2
     *  - MFCC 3
     *  - MFCC 4
     *  - MFCC 5
     *  - MFCC 6
     *  - MFCC 7
     *  - MFCC 8
     *  - MFCC 9
     *  - MFCC 10
     *  - MFCC 11
     *  - MFCC 12
     *  - MFCC 13
     */
    Array <double> getFeatureArray (int frameNum) const;

    /** Get an MD5 checksum of the audio features. */
    MemoryBlock getMD5Checksum() const;

    /** Returns true if every audio feature in every frame of
     *  two feature extractors is the same. */
    bool operator == (const SAFEFeatureExtractor& testObject);

private:
    HeapBlock <double> means;
    HeapBlock <double> variances;
    HeapBlock <double> standardDeviations;
    HeapBlock <double> rmsAmplitudes;
    HeapBlock <double> zeroCrossingRates;

    HeapBlock <double> spectrum;
    HeapBlock <double> spectralCentroids;
    HeapBlock <double> spectralVariances;
    HeapBlock <double> spectralStandardDeviations;
    HeapBlock <double> spectralSkewnesses;
    HeapBlock <double> spectralKurtosises;
    HeapBlock <double> irregularityJs;
    HeapBlock <double> irregularityKs;
    HeapBlock <double> fundamentals;
    HeapBlock <double> smoothnesses;
    HeapBlock <double> rolloffs;
    HeapBlock <double> flatnesses;
    HeapBlock <double> tonalities;
    HeapBlock <double> crests;
    HeapBlock <double> spectralSlopes;

    HeapBlock <double> peakSpectrum;
    HeapBlock <double> peakSpectralCentroids;
    HeapBlock <double> peakSpectralVariances;
    HeapBlock <double> peakSpectralStandardDeviations;
    HeapBlock <double> peakSpectralSkewnesses;
    HeapBlock <double> peakSpectralKurtosises;
    HeapBlock <double> peakIrregularityJs;
    HeapBlock <double> peakIrregularityKs;
    HeapBlock <double> peakTristimulus1s;
    HeapBlock <double> peakTristimulus2s;
    HeapBlock <double> peakTristimulus3s;
    HeapBlock <double> inharmonicities;

    HeapBlock <double> harmonicSpectrum;
    HeapBlock <double> harmonicSpectralCentroids;
    HeapBlock <double> harmonicSpectralVariances;
    HeapBlock <double> harmonicSpectralStandardDeviations;
    HeapBlock <double> harmonicSpectralSkewnesses;
    HeapBlock <double> harmonicSpectralKurtosises;
    HeapBlock <double> harmonicIrregularityJs;
    HeapBlock <double> harmonicIrregularityKs;
    HeapBlock <double> harmonicTristimulus1s;
    HeapBlock <double> harmonicTristimulus2s;
    HeapBlock <double> harmonicTristimulus3s;
    HeapBlock <double> noisinesses;
    HeapBlock <double> parityRatios;

    OwnedArray <Array <double> > barkCoefficients;

    OwnedArray <Array <double> > mfccs;

    int numAnalysisFrames;
    int analysisFrameLength;
    double fs;

    bool initialised;

    SharedResourcePointer <LibXtractHolder> libXtract;

    bool checkEqualityOrNan (double a, double b);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SAFEFeatureExtractor);
};

#endif //__SAFEFEATUREEXTRACTOR__
