String LibXtract::getFeatureName (Feature feature)
{
    switch (feature)
    {
        case TemporalMean:
            return "LibXtract Temporal Mean";

        case TemporalVariance:
            return "LibXtract Temporal Variance";

        case TemporalStandardDeviation:
            return "LibXtract Temporal Standard Deviation";

        case RMSAmplitude:
            return "LibXtract RMS Amplitude";

        case ZeroCrossingRate:
            return "LibXtract Zero Crossing Rate";

        case FundamentalFrequency:
            return "LibXtract Fundamental Frequency";

        case SpectralCentroid:
            return "LibXtract Spectral Centroid";

        case SpectralVariance:
            return "LibXtract Spectral Variance";

        case SpectralStandardDeviation:
            return "LibXtract Spectral Standard Deviation";

        case SpectralSkewness:
            return "LibXtract Spectral Skewness";

        case SpectralKurtosis:
            return "LibXtract Spectral Kurtosis";

        case JensenIrregularity:
            return "LibXtract Jensen Irregularity";

        case KrimphoffIrregularity:
            return "LibXtract Krimphoff Irregularity";

        case SpectralSmoothness:
            return "LibXtract Spectral Smoothness";

        case SpectralRollOff:
            return "LibXtract Spectral Roll Off";

        case SpectralFlatness:
            return "LibXtract Spectral Flatness";

        case Tonality:
            return "LibXtract Tonality";

        case CrestFactor:
            return "LibXtract Crest Factor";

        case SpectralSlope:
            return "LibXtract Spectral Slope";

        case PeakSpectralCentroid:
            return "LibXtract Peak Spectral Centroid";

        case PeakSpectralVariance:
            return "LibXtract Peak Spectral Variance";

        case PeakSpectralStandardDeviation:
            return "LibXtract Peak Spectral Standard Deviation";

        case PeakSpectralSkewness:
            return "LibXtract Peak Spectral Skewness";

        case PeakSpectralKurtosis:
            return "LibXtract Peak Spectral Kurtosis";

        case PeakJensenIrregularity:
            return "LibXtract Peak Jensen Irregularity";

        case PeakKrimphoffIrregularity:
            return "LibXtract Peak Krimphoff Irregularity";

        case PeakTristimulus1:
            return "LibXtract Peak Tristimulus 1";

        case PeakTristimulus2:
            return "LibXtract Peak Tristimulus 2";

        case PeakTristimulus3:
            return "LibXtract Peak Tristimulus 3";

        case Inharmonicity:
            return "LibXtract Inharmonicity";

        case HarmonicSpectralCentroid:
            return "LibXtract Harmonic Spectral Centroid";

        case HarmonicSpectralVariance:
            return "LibXtract Harmonic Spectral Variance";

        case HarmonicSpectralStandardDeviation:
            return "LibXtract Harmonic Spectral Standard Deviation";

        case HarmonicSpectralSkewness:
            return "LibXtract Harmonic Spectral Skewness";

        case HarmonicSpectralKurtosis:
            return "LibXtract Harmonic Spectral Kurtosis";

        case HarmonicJensenIrregularity:
            return "LibXtract Harmonic Jensen Irregularity";

        case HarmonicKrimphoffIrregularity:
            return "LibXtract Harmonic Krimphoff Irregularity";

        case HarmonicTristimulus1:
            return "LibXtract Harmonic Tristimulus 1";

        case HarmonicTristimulus2:
            return "LibXtract Harmonic Tristimulus 2";

        case HarmonicTristimulus3:
            return "LibXtract Harmonic Tristimulus 3";

        case Noisiness:
            return "LibXtract Noisiness";

        case HarmonicParityRatio:
            return "LibXtract Harmonic Parity Ratio";

        case BarkCoefficients:
            return "LibXtract Bark Coefficients";

        case MFCCs:
            return "LibXtract MFCCs";

    }
}
