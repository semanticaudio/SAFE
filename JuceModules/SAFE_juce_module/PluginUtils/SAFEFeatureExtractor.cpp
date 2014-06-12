//==========================================================================
//      Constructor and Destructor
//==========================================================================
SAFEFeatureExtractor::SAFEFeatureExtractor()
{
}

SAFEFeatureExtractor::~SAFEFeatureExtractor()
{
}

void SAFEFeatureExtractor::initialise (int numAnalysisFramesInit, int analysisFrameLengthInit, double sampleRate)
{
    numAnalysisFrames = numAnalysisFramesInit;
    analysisFrameLength = analysisFrameLengthInit;
    fs = sampleRate;

    // initialise storage for time domain features
    means.allocate (numAnalysisFrames, true);
    variances.allocate (numAnalysisFrames, true);
    standardDeviations.allocate (numAnalysisFrames, true);
    rmsAmplitudes.allocate (numAnalysisFrames, true);
    zeroCrossingRates.allocate (numAnalysisFrames, true);
    
    // initialise storage for spectral features
    spectrum.allocate (analysisFrameLength, true);
    spectralCentroids.allocate (numAnalysisFrames, true);
    spectralVariances.allocate (numAnalysisFrames, true);
    spectralStandardDeviations.allocate (numAnalysisFrames, true);
    spectralSkewnesses.allocate (numAnalysisFrames, true);
    spectralKurtosises.allocate (numAnalysisFrames, true);
    irregularityJs.allocate (numAnalysisFrames, true);
    irregularityKs.allocate (numAnalysisFrames, true);
    fundamentals.allocate (numAnalysisFrames, true);
    smoothnesses.allocate (numAnalysisFrames, true);
    rolloffs.allocate (numAnalysisFrames, true);
    flatnesses.allocate (numAnalysisFrames, true);
    tonalities.allocate (numAnalysisFrames, true);
    crests.allocate (numAnalysisFrames, true);
    spectralSlopes.allocate (numAnalysisFrames, true);

    // initialise storage for peak spectral features
    peakSpectrum.allocate (analysisFrameLength, true);
    peakSpectralCentroids.allocate (numAnalysisFrames, true);
    peakSpectralVariances.allocate (numAnalysisFrames, true);
    peakSpectralStandardDeviations.allocate (numAnalysisFrames, true);
    peakSpectralSkewnesses.allocate (numAnalysisFrames, true);
    peakSpectralKurtosises.allocate (numAnalysisFrames, true);
    peakIrregularityJs.allocate (numAnalysisFrames, true);
    peakIrregularityKs.allocate (numAnalysisFrames, true);
    peakTristimulus1s.allocate (numAnalysisFrames, true);
    peakTristimulus2s.allocate (numAnalysisFrames, true);
    peakTristimulus3s.allocate (numAnalysisFrames, true);
    inharmonicities.allocate (numAnalysisFrames, true);

    // initialise storage for harmonic spectral features
    harmonicSpectrum.allocate (analysisFrameLength, true);
    harmonicSpectralCentroids.allocate (numAnalysisFrames, true);
    harmonicSpectralVariances.allocate (numAnalysisFrames, true);
    harmonicSpectralStandardDeviations.allocate (numAnalysisFrames, true);
    harmonicSpectralSkewnesses.allocate (numAnalysisFrames, true);
    harmonicSpectralKurtosises.allocate (numAnalysisFrames, true);
    harmonicIrregularityJs.allocate (numAnalysisFrames, true);
    harmonicIrregularityKs.allocate (numAnalysisFrames, true);
    harmonicTristimulus1s.allocate (numAnalysisFrames, true);
    harmonicTristimulus2s.allocate (numAnalysisFrames, true);
    harmonicTristimulus3s.allocate (numAnalysisFrames, true);
    noisinesses.allocate (numAnalysisFrames, true);
    parityRatios.allocate (numAnalysisFrames, true);

    // initialise storage for bark and mfcc features
    xtract_init_bark (analysisFrameLength, fs, libXtract->barkBandLimits);
    barkCoefficients.clear();
    
    xtract_init_mfcc (analysisFrameLength / 2, fs / 2, XTRACT_EQUAL_GAIN, 20, 20000, libXtract->melFilters.n_filters, libXtract->melFilters.filters);
    mfccs.clear();

    for (int n = 0; n < numAnalysisFrames; ++n)
    {
        barkCoefficients.add (new Array <double>);
        barkCoefficients [n]->resize (25);

        mfccs.add (new Array <double>);
        mfccs [n]->resize (13);
    }

    xtract_init_wavelet_f0_state();
}

void SAFEFeatureExtractor::getAllFeatures (double* sampleData, int numSamples, int frameNum)
{
    // time domain features
    xtract_mean (sampleData, numSamples, NULL, means + frameNum);
    xtract_variance (sampleData, numSamples, means + frameNum, variances + frameNum);
    xtract_standard_deviation (sampleData, numSamples, variances + frameNum, standardDeviations + frameNum);
    xtract_rms_amplitude (sampleData, numSamples, NULL, rmsAmplitudes + frameNum);
    xtract_zcr (sampleData, numSamples, NULL, zeroCrossingRates + frameNum);

    // spectral features
    double argumentArray [4] = {fs / numSamples, XTRACT_MAGNITUDE_SPECTRUM, 0, 0};
    xtract_spectrum (sampleData, numSamples, argumentArray, spectrum);
    xtract_spectral_centroid (spectrum, numSamples, NULL, spectralCentroids + frameNum);
    xtract_spectral_variance (spectrum, numSamples, spectralCentroids + frameNum, spectralVariances + frameNum);
    xtract_spectral_standard_deviation (spectrum, numSamples, spectralVariances + frameNum, spectralStandardDeviations + frameNum);
    argumentArray [0] = spectralCentroids [frameNum];
    argumentArray [1] = spectralStandardDeviations [frameNum];
    xtract_spectral_skewness (spectrum, numSamples, argumentArray, spectralSkewnesses + frameNum);
    xtract_spectral_kurtosis (spectrum, numSamples, argumentArray, spectralKurtosises + frameNum);
    xtract_irregularity_j (spectrum, numSamples / 2, NULL, irregularityJs + frameNum);
    xtract_irregularity_k (spectrum, numSamples / 2, NULL, irregularityKs + frameNum);
    xtract_wavelet_f0 (sampleData, numSamples, &fs, fundamentals + frameNum);
    xtract_smoothness (spectrum, numSamples / 2, NULL, smoothnesses + frameNum);
    argumentArray [0] = fs / numSamples;
    argumentArray [1] = 45;
    xtract_rolloff (spectrum, numSamples / 2, argumentArray, rolloffs + frameNum);
    xtract_flatness (spectrum, numSamples / 2, NULL, flatnesses + frameNum);
    double logFlatness;
    xtract_flatness_db (NULL, 0, flatnesses + frameNum, &logFlatness);
    xtract_tonality (NULL, 0, &logFlatness, tonalities + frameNum);
    xtract_highest_value (spectrum, numSamples / 2, NULL, argumentArray);
    xtract_mean (spectrum, numSamples / 2, NULL, argumentArray + 1);
    xtract_crest (NULL, 0, argumentArray, crests + frameNum);
    xtract_spectral_slope (spectrum, numSamples, NULL, spectralSlopes + frameNum);

    // peak spectrum features
    argumentArray [0] = fs / numSamples;
    argumentArray [1] = 10;
    xtract_peak_spectrum (spectrum, numSamples / 2, argumentArray, peakSpectrum);
    xtract_spectral_centroid (peakSpectrum, numSamples, NULL, peakSpectralCentroids + frameNum);
    xtract_spectral_variance (peakSpectrum, numSamples, peakSpectralCentroids + frameNum, peakSpectralVariances + frameNum);
    xtract_spectral_standard_deviation (peakSpectrum, numSamples, peakSpectralVariances + frameNum, peakSpectralStandardDeviations + frameNum);
    argumentArray [0] = peakSpectralCentroids [frameNum];
    argumentArray [1] = peakSpectralStandardDeviations [frameNum];
    xtract_spectral_skewness (peakSpectrum, numSamples, argumentArray, peakSpectralSkewnesses + frameNum);
    xtract_spectral_kurtosis (peakSpectrum, numSamples, argumentArray, peakSpectralKurtosises + frameNum);
    xtract_irregularity_j (peakSpectrum, numSamples / 2, NULL, peakIrregularityJs + frameNum);
    xtract_irregularity_k (peakSpectrum, numSamples / 2, NULL, peakIrregularityKs + frameNum);
    xtract_tristimulus_1 (peakSpectrum, numSamples, fundamentals + frameNum, peakTristimulus1s + frameNum);
    xtract_tristimulus_2 (peakSpectrum, numSamples, fundamentals + frameNum, peakTristimulus2s + frameNum);
    xtract_tristimulus_3 (peakSpectrum, numSamples, fundamentals + frameNum, peakTristimulus3s + frameNum);
    xtract_spectral_inharmonicity (peakSpectrum, numSamples, fundamentals + frameNum, inharmonicities + frameNum);

    // harmonic spectrum features
    argumentArray [0] = fundamentals [frameNum];
    argumentArray [1] = 0.2;
    xtract_harmonic_spectrum (peakSpectrum, numSamples, argumentArray, harmonicSpectrum);
    xtract_spectral_centroid (harmonicSpectrum, numSamples, NULL, harmonicSpectralCentroids + frameNum);
    xtract_spectral_variance (harmonicSpectrum, numSamples, harmonicSpectralCentroids + frameNum, harmonicSpectralVariances + frameNum);
    xtract_spectral_standard_deviation (harmonicSpectrum, numSamples, harmonicSpectralVariances + frameNum, harmonicSpectralStandardDeviations + frameNum);
    argumentArray [0] = harmonicSpectralCentroids [frameNum];
    argumentArray [1] = harmonicSpectralStandardDeviations [frameNum];
    xtract_spectral_skewness (harmonicSpectrum, numSamples, argumentArray, harmonicSpectralSkewnesses + frameNum);
    xtract_spectral_kurtosis (harmonicSpectrum, numSamples, argumentArray, harmonicSpectralKurtosises + frameNum);
    xtract_irregularity_j (harmonicSpectrum, numSamples / 2, NULL, harmonicIrregularityJs + frameNum);
    xtract_irregularity_k (harmonicSpectrum, numSamples / 2, NULL, harmonicIrregularityKs + frameNum);
    xtract_tristimulus_1 (harmonicSpectrum, numSamples, fundamentals + frameNum, harmonicTristimulus1s + frameNum);
    xtract_tristimulus_2 (harmonicSpectrum, numSamples, fundamentals + frameNum, harmonicTristimulus2s + frameNum);
    xtract_tristimulus_3 (harmonicSpectrum, numSamples, fundamentals + frameNum, harmonicTristimulus3s + frameNum);
    double numHarmonics, numPartials;
    xtract_nonzero_count (harmonicSpectrum, numSamples / 2, NULL, &numHarmonics);
    xtract_nonzero_count (peakSpectrum, numSamples / 2, NULL, &numPartials);
    argumentArray [0] = numHarmonics;
    argumentArray [1] = numPartials;
    xtract_noisiness (NULL, 0, argumentArray, noisinesses + frameNum);
    xtract_odd_even_ratio (harmonicSpectrum, numSamples, fundamentals + frameNum, parityRatios + frameNum);

    // bark features
    double* barkCoefficientsFrame = barkCoefficients [frameNum]->getRawDataPointer();
    xtract_bark_coefficients (spectrum, numSamples / 2, libXtract->barkBandLimits, barkCoefficientsFrame);

    // mfcc features
    double* mfccsFrame = mfccs [frameNum]->getRawDataPointer();
    xtract_mfcc (spectrum, numSamples / 2, &libXtract->melFilters, mfccsFrame);
}

void SAFEFeatureExtractor::addToXml (XmlElement* parentElement)
{
    for (int frameNum = 0; frameNum < numAnalysisFrames; ++frameNum)
    {
        String frameName = String ("Frame") + String (frameNum);

        XmlElement* frameElement = parentElement->getChildByName (frameName);

        if (! frameElement)
        {
            frameElement = parentElement->createNewChildElement (frameName);
        }

        // time domain features
        frameElement->setAttribute ("Mean", means [frameNum]);
        frameElement->setAttribute ("Variance", variances [frameNum]);
        frameElement->setAttribute ("Standard_Deviation", standardDeviations [frameNum]);
        frameElement->setAttribute ("RMS_Amplitude", rmsAmplitudes [frameNum]);
        frameElement->setAttribute ("Zero_Crossing_Rate", zeroCrossingRates [frameNum]);

        // spectral features
        frameElement->setAttribute ("Spectral_Centroid", spectralCentroids [frameNum]);
        frameElement->setAttribute ("Spectral_Variance", spectralVariances [frameNum]);
        frameElement->setAttribute ("Spectral_Standard_Deviation", spectralStandardDeviations [frameNum]);
        frameElement->setAttribute ("Spectral_Skewness", spectralSkewnesses [frameNum]);
        frameElement->setAttribute ("Spectral_Kurtosis", spectralKurtosises [frameNum]);
        frameElement->setAttribute ("Irregularity_J", irregularityJs [frameNum]);
        frameElement->setAttribute ("Irregularity_K", irregularityKs [frameNum]);
        frameElement->setAttribute ("Fundamental", fundamentals [frameNum]);
        frameElement->setAttribute ("Smoothness", smoothnesses [frameNum]);
        frameElement->setAttribute ("Spectral_Roll_Off", rolloffs [frameNum]);
        frameElement->setAttribute ("Spectral_Flatness", flatnesses [frameNum]);
        frameElement->setAttribute ("Tonality", tonalities [frameNum]);
        frameElement->setAttribute ("Spectral_Crest", crests [frameNum]);
        frameElement->setAttribute ("Spectral_Slope", spectralSlopes [frameNum]);

        // peak spectral features
        frameElement->setAttribute ("Peak_Spectral_Centroid", peakSpectralCentroids [frameNum]);
        frameElement->setAttribute ("Peak_Spectral_Variance", peakSpectralVariances [frameNum]);
        frameElement->setAttribute ("Peak_Spectral_Standard_Deviation", peakSpectralStandardDeviations [frameNum]);
        frameElement->setAttribute ("Peak_Spectral_Skewness", peakSpectralSkewnesses [frameNum]);
        frameElement->setAttribute ("Peak_Spectral_Kurtosis", peakSpectralKurtosises [frameNum]);
        frameElement->setAttribute ("Peak_Irregularity_J", peakIrregularityJs [frameNum]);
        frameElement->setAttribute ("Peak_Irregularity_K", peakIrregularityKs [frameNum]);
        frameElement->setAttribute ("Peak_Tristimulus_1", peakTristimulus1s [frameNum]);
        frameElement->setAttribute ("Peak_Tristimulus_2", peakTristimulus2s [frameNum]);
        frameElement->setAttribute ("Peak_Tristimulus_3", peakTristimulus3s [frameNum]);
        frameElement->setAttribute ("Inharmonicity", inharmonicities [frameNum]);

        // harmonic spectral features
        frameElement->setAttribute ("Harmonic_Spectral_Centroid", harmonicSpectralCentroids [frameNum]);
        frameElement->setAttribute ("Harmonic_Spectral_Variance", harmonicSpectralVariances [frameNum]);
        frameElement->setAttribute ("Harmonic_Spectral_Standard_Deviation", harmonicSpectralStandardDeviations [frameNum]);
        frameElement->setAttribute ("Harmonic_Spectral_Skewness", harmonicSpectralSkewnesses [frameNum]);
        frameElement->setAttribute ("Harmonic_Spectral_Kurtosis", harmonicSpectralKurtosises [frameNum]);
        frameElement->setAttribute ("Harmonic_Irregularity_J", harmonicIrregularityJs [frameNum]);
        frameElement->setAttribute ("Harmonic_Irregularity_K", harmonicIrregularityKs [frameNum]);
        frameElement->setAttribute ("Harmonic_Tristimulus_1", harmonicTristimulus1s [frameNum]);
        frameElement->setAttribute ("Harmonic_Tristimulus_2", harmonicTristimulus2s [frameNum]);
        frameElement->setAttribute ("Harmonic_Tristimulus_3", harmonicTristimulus3s [frameNum]);
        frameElement->setAttribute ("Noisiness", noisinesses [frameNum]);
        frameElement->setAttribute ("Parity_Ratio", parityRatios [frameNum]);

        // bark features
        for (int n = 0; n < 25; ++n)
        {
            String attributeName = String ("Bark_Coefficient_") + String (n);
            frameElement->setAttribute (attributeName, barkCoefficients [frameNum]->getUnchecked (n));
        }

        // mfccs
        for (int n = 0; n < 13; ++n)
        {
            String attributeName = String ("MFCC_") + String (n);
            frameElement->setAttribute (attributeName, mfccs [frameNum]->getUnchecked (n));
        }
    }
}

Array <double> SAFEFeatureExtractor::getFeatureArray (int frameNum) const
{
    Array <double> array;

    array.add (means [frameNum]);
    array.add (variances [frameNum]);
    array.add (standardDeviations [frameNum]);
    array.add (rmsAmplitudes [frameNum]);
    array.add (zeroCrossingRates [frameNum]);
    array.add (spectralCentroids [frameNum]);
    array.add (spectralVariances [frameNum]);
    array.add (spectralStandardDeviations [frameNum]);
    array.add (spectralSkewnesses [frameNum]);
    array.add (spectralKurtosises [frameNum]);
    array.add (irregularityJs [frameNum]);
    array.add (irregularityKs [frameNum]);
    array.add (fundamentals [frameNum]);
    array.add (smoothnesses [frameNum]);
    array.add (rolloffs [frameNum]);
    array.add (flatnesses [frameNum]);
    array.add (tonalities [frameNum]);
    array.add (crests [frameNum]);
    array.add (spectralSlopes [frameNum]);
    array.add (peakSpectralCentroids [frameNum]);
    array.add (peakSpectralVariances [frameNum]);
    array.add (peakSpectralStandardDeviations [frameNum]);
    array.add (peakSpectralSkewnesses [frameNum]);
    array.add (peakSpectralKurtosises [frameNum]);
    array.add (peakIrregularityJs [frameNum]);
    array.add (peakIrregularityKs [frameNum]);
    array.add (peakTristimulus1s [frameNum]);
    array.add (peakTristimulus2s [frameNum]);
    array.add (peakTristimulus3s [frameNum]);
    array.add (inharmonicities [frameNum]);
    array.add (harmonicSpectralCentroids [frameNum]);
    array.add (harmonicSpectralVariances [frameNum]);
    array.add (harmonicSpectralStandardDeviations [frameNum]);
    array.add (harmonicSpectralSkewnesses [frameNum]);
    array.add (harmonicSpectralKurtosises [frameNum]);
    array.add (harmonicIrregularityJs [frameNum]);
    array.add (harmonicIrregularityKs [frameNum]);
    array.add (harmonicTristimulus1s [frameNum]);
    array.add (harmonicTristimulus2s [frameNum]);
    array.add (harmonicTristimulus3s [frameNum]);
    array.add (noisinesses [frameNum]);
    array.add (parityRatios [frameNum]);

    for (int n = 0; n < 25; ++n)
    {
        array.add (barkCoefficients [frameNum]->getUnchecked (n));
    }

    for (int n = 0; n < 13; ++n)
    {
        array.add (mfccs [frameNum]->getUnchecked (n));
    }

    return array;
}

MemoryBlock SAFEFeatureExtractor::getMD5Checksum() const
{
    MemoryBlock featureBlock;
    int blockSize = numAnalysisFrames * sizeof (double);

    featureBlock.append (means, blockSize);
    featureBlock.append (variances, blockSize);
    featureBlock.append (standardDeviations, blockSize);
    featureBlock.append (rmsAmplitudes, blockSize);
    featureBlock.append (zeroCrossingRates, blockSize);
    featureBlock.append (spectralCentroids, blockSize);
    featureBlock.append (spectralVariances, blockSize);
    featureBlock.append (spectralStandardDeviations, blockSize);
    featureBlock.append (spectralSkewnesses, blockSize);
    featureBlock.append (spectralKurtosises, blockSize);
    featureBlock.append (irregularityJs, blockSize);
    featureBlock.append (irregularityKs, blockSize);
    featureBlock.append (fundamentals, blockSize);
    featureBlock.append (smoothnesses, blockSize);
    featureBlock.append (rolloffs, blockSize);
    featureBlock.append (flatnesses, blockSize);
    featureBlock.append (tonalities, blockSize);
    featureBlock.append (crests, blockSize);
    featureBlock.append (spectralSlopes, blockSize);
    featureBlock.append (peakSpectralCentroids, blockSize);
    featureBlock.append (peakSpectralVariances, blockSize);
    featureBlock.append (peakSpectralStandardDeviations, blockSize);
    featureBlock.append (peakSpectralSkewnesses, blockSize);
    featureBlock.append (peakSpectralKurtosises, blockSize);
    featureBlock.append (peakIrregularityJs, blockSize);
    featureBlock.append (peakIrregularityKs, blockSize);
    featureBlock.append (peakTristimulus1s, blockSize);
    featureBlock.append (peakTristimulus2s, blockSize);
    featureBlock.append (peakTristimulus3s, blockSize);
    featureBlock.append (inharmonicities, blockSize);
    featureBlock.append (harmonicSpectralCentroids, blockSize);
    featureBlock.append (harmonicSpectralVariances, blockSize);
    featureBlock.append (harmonicSpectralStandardDeviations, blockSize);
    featureBlock.append (harmonicSpectralSkewnesses, blockSize);
    featureBlock.append (harmonicSpectralKurtosises, blockSize);
    featureBlock.append (harmonicIrregularityJs, blockSize);
    featureBlock.append (harmonicIrregularityKs, blockSize);
    featureBlock.append (harmonicTristimulus1s, blockSize);
    featureBlock.append (harmonicTristimulus2s, blockSize);
    featureBlock.append (harmonicTristimulus3s, blockSize);
    featureBlock.append (noisinesses, blockSize);
    featureBlock.append (parityRatios, blockSize);

    for (int n = 0; n < 25; ++n)
    {
        featureBlock.append (barkCoefficients [n]->getRawDataPointer(), blockSize);
    }
    
    for (int n = 0; n < 13; ++n)
    {
        featureBlock.append (mfccs [n]->getRawDataPointer(), blockSize);
    }

    MD5 md5Checksum (featureBlock);

    return md5Checksum.getRawChecksumData();
}

bool SAFEFeatureExtractor::operator == (const SAFEFeatureExtractor& testObject)
{
    bool returnValue = true;

    for (int frameNum = 0; frameNum < numAnalysisFrames; ++ frameNum)
    {
        returnValue = returnValue && checkEqualityOrNan (means [frameNum], testObject.means [frameNum]);
        returnValue = returnValue && checkEqualityOrNan (variances [frameNum], testObject.variances [frameNum]);
        returnValue = returnValue && checkEqualityOrNan (standardDeviations [frameNum], testObject.standardDeviations [frameNum]);
        returnValue = returnValue && checkEqualityOrNan (rmsAmplitudes [frameNum], testObject.rmsAmplitudes [frameNum]);
        returnValue = returnValue && checkEqualityOrNan (zeroCrossingRates [frameNum], testObject.zeroCrossingRates [frameNum]);
        returnValue = returnValue && checkEqualityOrNan (spectralCentroids [frameNum], testObject.spectralCentroids [frameNum]);
        returnValue = returnValue && checkEqualityOrNan (spectralVariances [frameNum], testObject.spectralVariances [frameNum]);
        returnValue = returnValue && checkEqualityOrNan (spectralStandardDeviations [frameNum], testObject.spectralStandardDeviations [frameNum]);
        returnValue = returnValue && checkEqualityOrNan (spectralSkewnesses [frameNum], testObject.spectralSkewnesses [frameNum]);
        returnValue = returnValue && checkEqualityOrNan (spectralKurtosises [frameNum], testObject.spectralKurtosises [frameNum]);
        returnValue = returnValue && checkEqualityOrNan (irregularityJs [frameNum], testObject.irregularityJs [frameNum]);
        returnValue = returnValue && checkEqualityOrNan (irregularityKs [frameNum], testObject.irregularityKs [frameNum]);
        returnValue = returnValue && checkEqualityOrNan (fundamentals [frameNum], testObject.fundamentals [frameNum]);
        returnValue = returnValue && checkEqualityOrNan (smoothnesses [frameNum], testObject.smoothnesses [frameNum]);
        returnValue = returnValue && checkEqualityOrNan (rolloffs [frameNum], testObject.rolloffs [frameNum]);
        returnValue = returnValue && checkEqualityOrNan (flatnesses [frameNum], testObject.flatnesses [frameNum]);
        returnValue = returnValue && checkEqualityOrNan (tonalities [frameNum], testObject.tonalities [frameNum]);
        returnValue = returnValue && checkEqualityOrNan (crests [frameNum], testObject.crests [frameNum]);
        returnValue = returnValue && checkEqualityOrNan (spectralSlopes [frameNum], testObject.spectralSlopes [frameNum]);
        returnValue = returnValue && checkEqualityOrNan (peakSpectralCentroids [frameNum], testObject.peakSpectralCentroids [frameNum]);
        returnValue = returnValue && checkEqualityOrNan (peakSpectralVariances [frameNum], testObject.peakSpectralVariances [frameNum]);
        returnValue = returnValue && checkEqualityOrNan (peakSpectralStandardDeviations [frameNum], testObject.peakSpectralStandardDeviations [frameNum]);
        returnValue = returnValue && checkEqualityOrNan (peakSpectralSkewnesses [frameNum], testObject.peakSpectralSkewnesses [frameNum]);
        returnValue = returnValue && checkEqualityOrNan (peakSpectralKurtosises [frameNum], testObject.peakSpectralKurtosises [frameNum]);
        returnValue = returnValue && checkEqualityOrNan (peakIrregularityJs [frameNum], testObject.peakIrregularityJs [frameNum]);
        returnValue = returnValue && checkEqualityOrNan (peakIrregularityKs [frameNum], testObject.peakIrregularityKs [frameNum]);
        returnValue = returnValue && checkEqualityOrNan (peakTristimulus1s [frameNum], testObject.peakTristimulus1s [frameNum]);
        returnValue = returnValue && checkEqualityOrNan (peakTristimulus2s [frameNum], testObject.peakTristimulus2s [frameNum]);
        returnValue = returnValue && checkEqualityOrNan (peakTristimulus3s [frameNum], testObject.peakTristimulus3s [frameNum]);
        returnValue = returnValue && checkEqualityOrNan (inharmonicities [frameNum], testObject.inharmonicities [frameNum]);
        returnValue = returnValue && checkEqualityOrNan (harmonicSpectralCentroids [frameNum], testObject.harmonicSpectralCentroids [frameNum]);
        returnValue = returnValue && checkEqualityOrNan (harmonicSpectralVariances [frameNum], testObject.harmonicSpectralVariances [frameNum]);
        returnValue = returnValue && checkEqualityOrNan (harmonicSpectralStandardDeviations [frameNum], testObject.harmonicSpectralStandardDeviations [frameNum]);
        returnValue = returnValue && checkEqualityOrNan (harmonicSpectralSkewnesses [frameNum], testObject.harmonicSpectralSkewnesses [frameNum]);
        returnValue = returnValue && checkEqualityOrNan (harmonicSpectralKurtosises [frameNum], testObject.harmonicSpectralKurtosises [frameNum]);
        returnValue = returnValue && checkEqualityOrNan (harmonicIrregularityJs [frameNum], testObject.harmonicIrregularityJs [frameNum]);
        returnValue = returnValue && checkEqualityOrNan (harmonicIrregularityKs [frameNum], testObject.harmonicIrregularityKs [frameNum]);
        returnValue = returnValue && checkEqualityOrNan (harmonicTristimulus1s [frameNum], testObject.harmonicTristimulus1s [frameNum]);
        returnValue = returnValue && checkEqualityOrNan (harmonicTristimulus2s [frameNum], testObject.harmonicTristimulus2s [frameNum]);
        returnValue = returnValue && checkEqualityOrNan (harmonicTristimulus3s [frameNum], testObject.harmonicTristimulus3s [frameNum]);
        returnValue = returnValue && checkEqualityOrNan (noisinesses [frameNum], testObject.noisinesses [frameNum]);
        returnValue = returnValue && checkEqualityOrNan (parityRatios [frameNum], testObject.parityRatios [frameNum]);

        for (int n = 0; n < 25; ++n)
        {
            returnValue = returnValue && checkEqualityOrNan (barkCoefficients [frameNum]->getUnchecked (n), testObject.barkCoefficients [frameNum]->getUnchecked (n));
        }

        for (int n = 0; n < 13; ++n)
        {
            returnValue = returnValue && checkEqualityOrNan (mfccs [frameNum]->getUnchecked (n), testObject.mfccs [frameNum]->getUnchecked (n));
        }
    }

    return returnValue;
}

bool SAFEFeatureExtractor::checkEqualityOrNan (double a, double b)
{
    if ((a != a) && (b != b))
    {
        return true;
    }
    else
    {
        return a == b;
    }
}
