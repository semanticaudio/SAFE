#ifndef LIB_XTRACT_FEATURES_H_INCLUDED
#define LIB_XTRACT_FEATURES_H_INCLUDED

class LibXtract
{
public:
    enum Feature
    {
        // Temporal Features
        TemporalMean = 0,
        TemporalVariance,
        TemporalStandardDeviation,
        RMSAmplitude,
        ZeroCrossingRate,
        
        // Spectral Features
        FundamentalFrequency,
        SpectralCentroid,
        SpectralVariance,
        SpectralStandardDeviation,
        SpectralSkewness,
        SpectralKurtosis,
        JensenIrregularity,
        KrimphoffIrregularity,
        SpectralSmoothness,
        SpectralRollOff,
        SpectralFlatness,
        Tonality,
        CrestFactor,
        SpectralSlope,

        // Peak Spectral Features
        PeakSpectralCentroid,
        PeakSpectralVariance,
        PeakSpectralStandardDeviation,
        PeakSpectralSkewness,
        PeakSpectralKurtosis,
        PeakJensenIrregularity,
        PeakKrimphoffIrregularity,
        PeakTristimulus1,
        PeakTristimulus2,
        PeakTristimulus3,

        // Harmonic Spectral Features
        Inharmonicity,
        HarmonicSpectralCentroid,
        HarmonicSpectralVariance,
        HarmonicSpectralStandardDeviation,
        HarmonicSpectralSkewness,
        HarmonicSpectralKurtosis,
        HarmonicJensenIrregularity,
        HarmonicKrimphoffIrregularity,
        HarmonicTristimulus1,
        HarmonicTristimulus2,
        HarmonicTristimulus3,
        Noisiness,
        HarmonicParityRatio,

        // Number of Features
        NumScalarFeatures,

        // vector features
        BarkCoefficients,
        MFCCs,

        // Feature Groups
        TemporalFeatures,
        SpectralFeatures,
        PeakSpectralFeatures,
        HarmonicSpectralFeatures,
        AllFeatures
    };

    static String getFeatureName (Feature feature);
};

#endif // LIB_XTRACT_FEATURES_H_INCLUDED

