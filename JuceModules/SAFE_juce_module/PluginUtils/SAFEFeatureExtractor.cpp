//==========================================================================
//      Constructor and Destructor
//==========================================================================
SAFEFeatureExtractor::SAFEFeatureExtractor()
    : initialised (false),
      numChannels (0),
      frameSize (0),
      stepSize (0),
      fs (0.0),
      fft (nullptr),
      spectrumNeeded (false),
      libXtractSpectrumNeeded (false),
      libXtractPeakSpectrumNeeded (false),
      libXtractHarmonicSpectrumNeeded (false),
      calculateLibXtractBarkCoefficients (false),
      libXtractMelFiltersInitialised (false),
      calculateLibXtractMFCCs (false)
{
    // initialise libxtract arrays
    for (int i = 0; i < NumLibXtractScalarFeatures; ++i)
    {
        calculateLibXtractScalarFeature [i] = false;
        saveLibXtractScalarFeature [i] = false;
        libXtractScalarFeatureValues [i] = 0.0;
    }

    // allocate bark band limits and mel filters
    libXtractBarkBandLimits.allocate (numLibXtractBarkBands + 1, true);

    libXtractMelFilters.n_filters = numLibXtractMelFilters;
    libXtractMelFilters.filters = new double* [numLibXtractMelFilters];
}

SAFEFeatureExtractor::~SAFEFeatureExtractor()
{
    deleteLibXtractMelFilters();
    delete[] libXtractMelFilters.filters;
}

//==========================================================================
//      Setup
//==========================================================================
void SAFEFeatureExtractor::initialise (int numChannelsInit, int frameOrderInit, int stepSizeInit, double sampleRate)
{
    // save the number of channels
    numChannels = numChannelsInit;

    // calculate the frame size
    frameSize = 1 << frameOrderInit;

    // get the FFT object creating a new one if needs be
    if (! fftCache.count (frameSize))
    {
        fftCache.insert (std::pair <int, ScopedPointer <FFT> > (frameSize, new FFT (frameOrderInit, false)));
    }

    fft = fftCache [frameSize].get();

    // allocate some memory for the spectra to live in
    spectra.setSize (numChannels, 2 * frameSize);

    // make sure the step size is sensible
    if (stepSizeInit > frameSize || stepSizeInit < 1)
    {
        stepSize = frameSize;
    }
    else
    {
        stepSize = stepSizeInit;
    }

    // save the sample rate
    fs = sampleRate;

    // initialise libxtract bits
    libXtractScalarFeatureValues.resize (numChannels);

    xtract_init_bark (frameSize, fs, libXtractBarkBandLimits);
    libXtractBarkCoefficients.resize (numChannels);

    deleteLibXtractMelFilters();

    for (int i = 0; i < libXtractMelFilters.n_filters; ++i)
    {
        libXtractMelFilters.filters [i] = new double [frameSize];
    }

    xtract_init_mfcc (frameSize / 2, fs / 2, XTRACT_EQUAL_GAIN, 20, 20000,
                      libXtractMelFilters.n_filters, libXtractMelFilters.filters);
    libXtractMelFiltersInitialised = true;

    //allocate some memory for the libxtract spectra to live in
    libXtractSpectra.resize (numChannels);
    libXtractPeakSpectra.resize (numChannels);
    libXtractHarmonicSpectra.resize (numChannels);

    // allocate memory for multi channel buffers
    for (int i = 0; i < numChannels; ++i)
    {
        libXtractScalarFeatureValues.getReference (i).resize (NumLibXtractScalarFeatures);
        libXtractBarkCoefficients.getReference (i).resize (numLibXtractBarkBands);
        libXtractMFCCs.getReference (i).resize (numLibXtractMFCCs);

        libXtractSpectra.getReference (i).resize (frameSize);
        libXtractPeakSpectra.getReference (i).resize (frameSize);
        libXtractHarmonicSpectra.getReference (i).resize (frameSize);
    }

    libXtractChannelData.allocate (frameSize);

    initialised = true;
}

void SAFEFeatureExtractor::addLibXtractFeature (LibXtractFeature feature)
{
    if (feature < NumLibXtractScalarFeatures)
    {
        calculateLibXtractScalarFeature [feature] = true;
        saveLibXtractScalarFeature [feature] = true;

        if (feature >= XtractSpectralCentroid)
        {
            spectrumNeeded = true;
            libXtractSpectrumNeeded = true;
        }

        if (feature >= XtractPeakSpectralCentroid)
        {
            libXtractPeakSpectrumNeeded = true;
        }

        if (feature >= XtractPeakTristimulus1)
        {
            calculateLibXtractScalarFeature [XtractFundamentalFrequency] = true;
        }

        if (feature >= XtractHarmonicSpectralCentroid)
        {
            libXtractHarmonicSpectrumNeeded = true;
        }

        // make sure prerequisite features are calculated
        switch (feature)
        {
            // temporal features
            case XtractTemporalStandardDeviation:
                calculateLibXtractScalarFeature [XtractTemporalVariance] = true;

            case XtractTemporalVariance:
                calculateLibXtractScalarFeature [XtractTemporalMean] = true;
                break;

            // spectral features
            case XtractSpetralSkewness:
            case XtractSpetralKurtosis:
                calculateLibXtractScalarFeature [XtractSpectralStandardDeviation] = true;

            case XtractSpetralStandardDeviation:
                calculateLibXtractScalarFeature [XtractSpectralVariance] = true;

            case XtractSpetralVariance:
                calculateLibXtractScalarFeature [XtractSpectralCentriod] = true;
                break;

            case XtractTonality:
                calculateLibXtractScalarFeature [XtractSpectralFlatness] = true;
                break;

            // peak spectral features
            case XtractPeakSpetralSkewness:
            case XtractPeakSpetralKurtosis:
                calculateLibXtractScalarFeature [XtractPeakSpectralStandardDeviation] = true;

            case XtractPeakSpetralStandardDeviation:
                calculateLibXtractScalarFeature [XtractPeakSpectralVariance] = true;

            case XtractPeakSpetralVariance:
                calculateLibXtractScalarFeature [XtractPeakSpectralCentriod] = true;
                break;

            // harmonic spectral features
            case XtractHarmonicSpetralSkewness:
            case XtractHarmonicSpetralKurtosis:
                calculateLibXtractScalarFeature [XtractHarmonicSpectralStandardDeviation] = true;

            case XtractHarmonicSpetralStandardDeviation:
                calculateLibXtractScalarFeature [XtractHarmonicSpectralVariance] = true;

            case XtractHarmonicSpetralVariance:
                calculateLibXtractScalarFeature [XtractHarmonicSpectralCentriod] = true;
                break;

            default:
                break;
        }
    }
    else
    {
        switch (feature)
        {
            case XtractBarkCoefficients:
                spectrumNeeded = true;
                libXtractSpectrumNeeded = true;
                calculateLibXtractBarkCoefficients = true;
                break;

            case XtractMFCCs:
                spectrumNeeded = true;
                libXtractSpectrumNeeded = true;
                calculateLibXtractMFCCs = true;
                break;

            case XtractTemporalFeatures:

                for (int i = XtractTemporalMean; i <= XtractZeroCrossingRate; ++i)
                {
                    calculateLibXtractScalarFeature [i] = true;
                    saveLibXtractScalarFeature [i] = true;
                }

                break;

            case XtractSpectralFeatures:

                for (int i = XtractFundamentalFrequency; i <= XtractSpectralSlope; ++i)
                {
                    calculateLibXtractScalarFeature [i] = true;
                    saveLibXtractScalarFeature [i] = true;
                }

                spectrumNeeded = true;
                libXtractSpectrumNeeded = true;
                break;

            case XtractPeakSpectralFeatures:

                for (int i = XtractPeakSpectralCentroid; i <= XtractPeakTristimulus3; ++i)
                {
                    calculateLibXtractScalarFeature [i] = true;
                    saveLibXtractScalarFeature [i] = true;
                }

                calculateLibXtractScalarFeature [XtractFundamentalFrequency] = true;
                spectrumNeeded = true;
                libXtractSpectrumNeeded = true;
                libXtractPeakSpectrumNeeded = true;
                break;

            case XtractHarmonicSpectralFeatures:

                for (int i = XtractInharmonicity; i <= XtractHarmonicParityRatio; ++i)
                {
                    calculateLibXtractScalarFeature [i] = true;
                    saveLibXtractScalarFeature [i] = true;
                }

                calculateLibXtractScalarFeature [XtractFundamentalFrequency] = true;
                spectrumNeeded = true;
                libXtractSpectrumNeeded = true;
                libXtractPeakSpectrumNeeded = true;
                libXtractHarmonicSpectrumNeeded = true;
                break;

            case XtractAll
                
                for (int i = 0; i < NumLibXtractScalarFeatures; ++i)
                {
                    calculateLibXtractScalarFeature [i] = true;
                    saveLibXtractScalarFeature [i] = true;
                }

                calculateLibXtractBarkCoefficients = true;
                calculateLibXtractMFCCs = true;
                spectrumNeeded = true;
                libXtractSpectrumNeeded = true;
                libXtractPeakSpectrumNeeded = true;
                libXtractHarmonicSpectrumNeeded = true;
                break;

            default:
                break;
        }
    }
}

void SAFEFeatureExtractor::calculateSpectra (const AudioSampleBuffer &frame)
{
    if (! spectrumNeeded)
    {
        return;
    }

    for (int i = 0; i < numChannels; ++i)
    {
        spectra.copyFrom (i, 0, frame, i, 0, frameSize);
        fft->performRealOnlyForwardTransform (spectra.getWritePointer (i));
    }

    // scale spectra
    float signleBinGain = 1.0f / frameSize;
    float duplicateBinGain = 2.0f * singleBinGain;
    spectra.applyGain (0, 1, singleBinGain);
    spectra.applyGain (1, frameSize - 2, duplicateBinGain);
    spectra.applyGain (frameSize - 1, 1, singleBinGain);
}

void SAFEFeatureExtractor::deleteLibXtractMelFilters()
{
    if (libXtractMelFiltersInitialised)
    {
        for (int i = 0; i < libXtractMelFilters.n_filters; ++i)
        {
            delete[] libXtractMelFilters.filters [i];
        }

        libXtractMelFilteresInitialised = false;
    }
}

void SAFEFeatureExtractor::calculateLibXtractSpectra()
{
    if (! libXtractSpectrumNeeded)
    {
        return;
    }

    int numBins = frameSize / 2;
    double binWidth = fs / frameSize;

    for (int channel = 0; channel < numChannels; ++channel)
    {
        const float *spectrumChannel = spectra.getReadPointer (channel);
        double *libXtractSpectrumChannel = libXtractSpectra.getReference (channel).getRawDataPointer();

        for (int i = 1; i <= numBins; ++i)
        {
            double realPart = spectrumChannel [2 * i];
            double imagPart = spectrumChannel [2 * i + 1];
            double magnitude = pow (pow (realPart, 2.0), pow (imagPart, 2.0), 0.5);

            libXtractSpectrumChannel [i - 1] = magnitude;
            libXtractSpectrumChannel [i - 1 + numBins] = i * binWidth;
        }
    }
}

void SAFEFeatureExtractor::calculateLibXtractFeatures (const AudioSampleBuffer &frame)
{
    for (int channel = 0; channel < numChannels; ++channel)
    {
        const float *channelData = frame.getReadPointer (channel);
        double *scalarFeatureValues = libXtractScalarFeatureValues.getReference (channel).getRawDataPointer();
        double *spectrum = libXtractSpectra.getReference (channel).getRawDataPointer();
        double *peakSpectrum = libXtractPeakSpectra.getReference (channel).getRawDataPointer();
        double *harmonicSpectrum = libXtractHarmonicSpectra.getReference (channel).getRawDataPointer();

        for (int i = 0; i < frameSize; ++i)
        {
            libXtractChannelData [i] = channelData [i];
        }

        // temporal features
        if (calculateLibXtractScalarFeature [XtractTemporalMean])
        {
            xtract_mean (libXtractChannelData, 
                         frameSize, 
                         NULL, 
                         scalarFeatureValues + XtractTemporalMean);
        }

        if (calculateLibXtractScalarFeature [XtractTemporalVariance])
        {
            xtract_variance (libXtractChannelData, 
                             frameSize, 
                             scalarFeatureValues + XtractTemporalMean, 
                             scalarFeatureValues + XtractTemporalVariance);
        }

        if (calculateLibXtractScalarFeature [XtractTemporalStandardDeviation])
        {
            xtract_standard_deviation (libXtractChannelData, 
                                       frameSize, 
                                       scalarFeatureValues + XtractTemporalVariance, 
                                       scalarFeatureValues + XtractTemporalStandardDeviation);
        }

        if (calculateLibXtractScalarFeature [XtractRMSAmplitude])
        {
            xtract_rms_amplitude (libXtractChannelData, 
                                  frameSize, 
                                  NULL, 
                                  scalarFeatureValues + XtractRMSAmplitude);
        }

        if (calculateLibXtractScalarFeature [XtractZeroCrossingRate])
        {
            xtract_zcr (libXtractChannelData, 
                        frameSize, 
                        NULL, 
                        scalarFeatureValues + XtractZeroCrossingRate);
        }
        
        // spectral features
        if (calculateLibXtractScalarFeature [XtractFundamentalFrequency])
        {
            xtract_failsafe_f0 (libXtractChannelData, 
                                frameSize, 
                                &fs, 
                                scalarFeatureValues + XtractFundamentalFrequency);
        }

        if (calculateLibXtractScalarFeature [XtractSpectralCentroid])
        {
            xtract_spectral_centroid (spectrum, 
                                      frameSize, 
                                      NULL, 
                                      scalarFeatureValues + XtractSpectralCentroid);
        }

        if (calculateLibXtractScalarFeature [XtractSpectralVariance])
        {
            xtract_spectral_variance (spectrum, 
                                      frameSize, 
                                      scalarFeatureValues + XtractSpectralCentroid, 
                                      scalarFeatureValues + XtractSpectralVariance);
        }

        if (calculateLibXtractScalarFeature [XtractSpectralStandardDeviation])
        {
            xtract_spectral_standard_deviation (spectrum, 
                                                frameSize, 
                                                scalarFeatureValues + XtractSpectralVariance, 
                                                scalarFeatureValues + XtractSpectralStandardDeviation);
        }

        double argumentArray [2] = {scalarFeatureValues [XtractSpectralCentroid],
                                    scalarFeatureValues [XtractSpectralStandardDeviation]};

        if (calculateLibXtractScalarFeature [XtractSpectralSkewness])
        {
            xtract_spectral_skewness (spectrum, 
                                      frameSize, 
                                      argumentArray, 
                                      scalarFeatureValues + XtractSpectralSkewness);
        }

        if (calculateLibXtractScalarFeature [XtractSpectralKurtosis])
        {
            xtract_spectral_kurtosis (spectrum, 
                                      frameSize, 
                                      argumentArray, 
                                      scalarFeatureValues + XtractSpectralKurtosis);
        }

        if (calculateLibXtractScalarFeature [XtractJensenIrregularity])
        {
            xtract_irregularity_j (spectrum, 
                                   frameSize / 2, 
                                   NULL, 
                                   scalarFeatureValues + XtractJensenIrregularity);
        }

        if (calculateLibXtractScalarFeature [XtractKrimphoffIrregularity])
        {
            xtract_irregularity_k (spectrum, 
                                   frameSize / 2, 
                                   NULL, 
                                   scalarFeatureValues + XtractKrimphoffIrregularity);
        }

        if (calculateLibXtractScalarFeature [XtractSpectralSmoothness])
        {
            xtract_smoothness (spectrum, 
                               frameSize / 2, 
                               NULL, 
                               scalarFeatureValues + XtractSpectralSmoothness);
        }

        argumentArray [0] = fs / frameSize;
        argumentArray [1] = 45;

        if (calculateLibXtractScalarFeature [XtractSpectralRollOff])
        {
            xtract_rolloff (spectrum, 
                            frameSize / 2, 
                            argumentArray, 
                            scalarFeatureValues + XtractSpectralRollOff);
        }

        if (calculateLibXtractScalarFeature [XtractSpectralFlatness])
        {
            xtract_flatness (spectrum, 
                             frameSize / 2, 
                             NULL, 
                             scalarFeatureValues + XtractSpectralFlatness);
        }

        if (calculateLibXtractScalarFeature [XtractTonality])
        {
            double logFlatness;

            xtract_flatness_db (NULL,
                                0,
                                scalarFeatureValues + XtractSpectralFlatness,
                                &logFlatness);

            xtract_tonality (NULL, 
                             0, 
                             &logFlatness, 
                             scalarFeatureValues + XtractTonality);
        }

        if (calculateLibXtractScalarFeature [XtractCrestFactor])
        {
            xtract_highest_value (spectrum,
                                  frameSize / 2,
                                  NULL,
                                  argumentArray);

            xtract_mean (spectrum,
                         frameSize / 2,
                         NULL,
                         argumentArray + 1);

            xtract_crest (NULL, 
                          0, 
                          argumentArray, 
                          scalarFeatureValues + XtractCrestFactor);
        }

        if (calculateLibXtractScalarFeature [XtractSpectralSlope])
        {
            xtract_spectral_slope (spectrum, 
                                   frameSize, 
                                   NULL, 
                                   scalarFeatureValues + XtractSpectralSlope);
        }

        // peak spectral features
        if (libXtractPeakSpectrumNeeded)
        {
            argumentArray [0] = fs / frameSize;
            argumentArray [1] = 10;

            xtract_peak_spectrum (spectrum,
                                  frameSize / 2,
                                  argumentArray,
                                  peakSpectrum);
        }

        if (calculateLibXtractScalarFeature [XtractPeakSpectralCentroid])
        {
            xtract_spectral_centroid (peakSpectrum, 
                                      frameSize, 
                                      NULL, 
                                      scalarFeatureValues + XtractPeakSpectralCentroid);
        }

        if (calculateLibXtractScalarFeature [XtractPeakSpectralVariance])
        {
            xtract_spectral_variance (peakSpectrum, 
                                      frameSize, 
                                      scalarFeatureValues + XtractPeakSpectralCentroid, 
                                      scalarFeatureValues + XtractPeakSpectralVariance);
        }

        if (calculateLibXtractScalarFeature [XtractPeakSpectralStandardDeviation])
        {
            xtract_spectral_standard_deviation (peakSpectrum, 
                                                frameSize, 
                                                scalarFeatureValues + XtractPeakSpectralVariance, 
                                                scalarFeatureValues + XtractPeakSpectralStandardDeviation);
        }

        argumentArray [0] = scalarFeatureValues [XtractPeakSpectralCentroid];
        argumentArray [1] = scalarFeatureValues [XtractPeakSpectralStandardDeviation]};

        if (calculateLibXtractScalarFeature [XtractPeakSpectralSkewness])
        {
            xtract_spectral_skewness (peakSpectrum, 
                                      frameSize, 
                                      argumentArray, 
                                      scalarFeatureValues + XtractPeakSpectralSkewness);
        }

        if (calculateLibXtractScalarFeature [XtractPeakSpectralKurtosis])
        {
            xtract_spectral_kurtosis (peakSpectrum, 
                                      frameSize, 
                                      argumentArray, 
                                      scalarFeatureValues + XtractPeakSpectralKurtosis);
        }

        if (calculateLibXtractScalarFeature [XtractPeakJensenIrregularity])
        {
            xtract_irregularity_j (peakSpectrum, 
                                   frameSize / 2, 
                                   NULL, 
                                   scalarFeatureValues + XtractPeakJensenIrregularity);
        }

        if (calculateLibXtractScalarFeature [XtractPeakKrimphoffIrregularity])
        {
            xtract_irregularity_k (peakSpectrum, 
                                   frameSize / 2, 
                                   NULL, 
                                   scalarFeatureValues + XtractPeakKrimphoffIrregularity);
        }

        if (calculateLibXtractScalarFeature [XtractPeakTristimulus1])
        {
            xtract_tristimulus_1 (peakSpectrum, 
                                  frameSize, 
                                  scalarFeatureValues + XtractFundamentalFrequency, 
                                  scalarFeatureValues + XtractPeakTristimulus1);
        }

        if (calculateLibXtractScalarFeature [XtractPeakTristimulus2])
        {
            xtract_tristimulus_2 (peakSpectrum, 
                                  frameSize, 
                                  scalarFeatureValues + XtractFundamentalFrequency, 
                                  scalarFeatureValues + XtractPeakTristimulus2);
        }

        if (calculateLibXtractScalarFeature [XtractPeakTristimulus3])
        {
            xtract_tristimulus_3 (peakSpectrum, 
                                  frameSize, 
                                  scalarFeatureValues + XtractFundamentalFrequency, 
                                  scalarFeatureValues + XtractPeakTristimulus3);
        }

        // harmonic spectral features
        
        if (calculateLibXtractScalarFeature [XtractInharmonicity])
        {
            xtract_spectral_inharmonicity (peakSpectrum, 
                                           frameSize, 
                                           scalarFeatureValues + XtractFundamentalFrequency, 
                                           scalarFeatureValues + XtractInharmonicity);
        }

        if (libXtractHarmonicSpectrumNeeded)
        {
            argumentArray [0] = scalarFeatureValues [XtractFundamentalFrequency];
            argumentArray [1] = 0.2;

            xtract_harmonic_spectrum (peakSpectrum,
                                      frameSize,
                                      argumentArray,
                                      harmonicSpectrum);
        }

        if (calculateLibXtractScalarFeature [XtractHarmonicSpectralCentroid])
        {
            xtract_spectral_centroid (harmonicSpectrum, 
                                      frameSize, 
                                      NULL, 
                                      scalarFeatureValues + XtractHarmonicSpectralCentroid);
        }

        if (calculateLibXtractScalarFeature [XtractHarmonicSpectralVariance])
        {
            xtract_spectral_variance (harmonicSpectrum, 
                                      frameSize, 
                                      scalarFeatureValues + XtractHarmonicSpectralCentroid, 
                                      scalarFeatureValues + XtractHarmonicSpectralVariance);
        }

        if (calculateLibXtractScalarFeature [XtractHarmonicSpectralStandardDeviation])
        {
            xtract_spectral_standard_deviation (harmonicSpectrum, 
                                                frameSize, 
                                                scalarFeatureValues + XtractHarmonicSpectralVariance, 
                                                scalarFeatureValues + XtractHarmonicSpectralStandardDeviation);
        }

        argumentArray [0] = scalarFeatureValues [XtractHarmonicSpectralCentroid];
        argumentArray [1] = scalarFeatureValues [XtractHarmonicSpectralStandardDeviation]};

        if (calculateLibXtractScalarFeature [XtractHarmonicSpectralSkewness])
        {
            xtract_spectral_skewness (harmonicSpectrum, 
                                      frameSize, 
                                      argumentArray, 
                                      scalarFeatureValues + XtractHarmonicSpectralSkewness);
        }

        if (calculateLibXtractScalarFeature [XtractHarmonicSpectralKurtosis])
        {
            xtract_spectral_kurtosis (harmonicSpectrum, 
                                      frameSize, 
                                      argumentArray, 
                                      scalarFeatureValues + XtractHarmonicSpectralKurtosis);
        }

        if (calculateLibXtractScalarFeature [XtractHarmonicJensenIrregularity])
        {
            xtract_irregularity_j (harmonicSpectrum, 
                                   frameSize / 2, 
                                   NULL, 
                                   scalarFeatureValues + XtractHarmonicJensenIrregularity);
        }

        if (calculateLibXtractScalarFeature [XtractHarmonicKrimphoffIrregularity])
        {
            xtract_irregularity_k (harmonicSpectrum, 
                                   frameSize / 2, 
                                   NULL, 
                                   scalarFeatureValues + XtractHarmonicKrimphoffIrregularity);
        }

        if (calculateLibXtractScalarFeature [XtractHarmonicTristimulus1])
        {
            xtract_tristimulus_1 (harmonicSpectrum, 
                                  frameSize, 
                                  scalarFeatureValues + XtractFundamentalFrequency, 
                                  scalarFeatureValues + XtractHarmonicTristimulus1);
        }

        if (calculateLibXtractScalarFeature [XtractHarmonicTristimulus2])
        {
            xtract_tristimulus_2 (harmonicSpectrum, 
                                  frameSize, 
                                  scalarFeatureValues + XtractFundamentalFrequency, 
                                  scalarFeatureValues + XtractHarmonicTristimulus2);
        }

        if (calculateLibXtractScalarFeature [XtractHarmonicTristimulus3])
        {
            xtract_tristimulus_3 (harmonicSpectrum, 
                                  frameSize, 
                                  scalarFeatureValues + XtractFundamentalFrequency, 
                                  scalarFeatureValues + XtractHarmonicTristimulus3);
        }

        if (calculateLibXtractScalarFeature [XtractNoisiness])
        {
            xtract_nonzero_count (harmonicSpectrum,
                                  frameSize / 2,
                                  NULL,
                                  argumentArray);

            xtract_nonzero_count (peakSpectrum,
                                  frameSize / 2,
                                  NULL,
                                  arguemtnArray + 1);

            xtract_noisiness (NULL, 
                              0, 
                              argumentArray, 
                              scalarFeatureValues + XtractNoisiness);
        }

        if (calculateLibXtractScalarFeature [XtractHarmonicParityRatio])
        {
            xtract_odd_even_ratio (harmonicSpectrum, 
                                   frameSize, 
                                   scalarFeatureValues + XtractFundamentalFrequency, 
                                   scalarFeatureValues + XtractHarmonicParityRatio);
        }

        if (calculateLibXtractBarkCoefficients)
        {
            double *barkCoefficients = libXtractBarkCoefficients.getReference (channel).getRawDataPointer();

            xtract_bark_coefficients (spectrum,
                                      frameSize / 2,
                                      libXtractBarkBandLimits,
                                      barkCoefficients);
        }

        if (calculateLibXtractMFCCs)
        {
            double *mfccs = libXtractMFCCs.getReference (channel).getRawDataPointer();

            xtract_bark_coefficients (spectrum,
                                      frameSize / 2,
                                      &libXtractMelFilters,
                                      mfccs);
        }
    }
}
