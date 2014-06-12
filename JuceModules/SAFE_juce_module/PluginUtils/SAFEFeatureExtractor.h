#ifndef __SAFEFEATUREEXTRACTOR__
#define __SAFEFEATUREEXTRACTOR__

class SAFEFeatureExtractor
{
public:
    //==========================================================================
    //      Constructor and Destructor
    //==========================================================================
    SAFEFeatureExtractor();
    ~SAFEFeatureExtractor();

    void initialise (int numAnalysisFramesInit, int analysisFrameLengthInit, double sampleRate);

    void getAllFeatures (double* sampleData, int numSamples, int frameNum);

    void addToXml (XmlElement* parentElement);

    Array <double> getFeatureArray (int frameNum) const;

    MemoryBlock getMD5Checksum() const;

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
