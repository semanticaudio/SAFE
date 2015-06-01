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
    for (int i = 0; i < LibXtract::NumScalarFeatures; ++i)
    {
        calculateLibXtractScalarFeature [i] = false;
        saveLibXtractScalarFeature [i] = false;
    }

    // allocate bark band limits and mel filters
    libXtractBarkBandLimits.allocate (numLibXtractBarkBands + 1, true);

    libXtractMelFilters.n_filters = numLibXtractMelFilters;
    libXtractMelFilters.filters = new double* [numLibXtractMelFilters];

    // get the vamp loader instance
    vampPluginLoader = VampPluginLoader::getInstance();
}

SAFEFeatureExtractor::~SAFEFeatureExtractor()
{
    deleteLibXtractMelFilters();
    delete[] libXtractMelFilters.filters;
}

//==========================================================================
//      Setup
//==========================================================================
void SAFEFeatureExtractor::initialise (int numChannelsInit, int frameSizeInit, int stepSizeInit, double sampleRate)
{
    // save the number of channels
    numChannels = numChannelsInit;

    // calculate the frame size
    frameSize = frameSizeInit;

    int frameOrder = floor (log (frameSize) / log (2));

    // frame size must be a power of two
    jassert (pow (2, frameOrder) == frameSize);

    // get the FFT object creating a new one if needs be
    if (! fftCache.count (frameSize))
    {
        fftCache.insert (std::pair <int, ScopedPointer <FFT> > (frameSize, new FFT (frameOrder, false)));
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
	libXtractMFCCs.resize (numChannels);

    //allocate some memory for the libxtract spectra to live in
    libXtractSpectra.resize (numChannels);
    libXtractPeakSpectra.resize (numChannels);
    libXtractHarmonicSpectra.resize (numChannels);

    // allocate memory for multi channel buffers
    for (int i = 0; i < numChannels; ++i)
    {
        libXtractScalarFeatureValues.getReference (i).resize (LibXtract::NumScalarFeatures);
        libXtractBarkCoefficients.getReference (i).resize (numLibXtractBarkBands);
        libXtractMFCCs.getReference (i).resize (numLibXtractMelFilters);

        libXtractSpectra.getReference (i).resize (frameSize);
        libXtractPeakSpectra.getReference (i).resize (frameSize);
        libXtractHarmonicSpectra.getReference (i).resize (frameSize);
    }

    libXtractChannelData.allocate (frameSize, true);

    // initialise vamp plug-ins
    vampPlugins.clear();

    for (int i = 0; i < vampPluginKeys.size(); ++i)
    {
        VampPluginKey pluginKey = vampPluginKeys [i];

        if (VampPlugin *newPlugin = vampPluginLoader->loadPlugin (pluginKey,
                fs, VampPluginLoader::ADAPT_CHANNEL_COUNT |
                    VampPluginLoader::ADAPT_BUFFER_SIZE))
        {
            vampPlugins.add (newPlugin);
            newPlugin->initialise (numChannels, stepSize, frameSize);
        }
    }

    initialised = true;
}

void SAFEFeatureExtractor::addLibXtractFeature (LibXtract::Feature feature)
{
    if (feature < LibXtract::NumScalarFeatures)
    {
        calculateLibXtractScalarFeature [feature] = true;
        saveLibXtractScalarFeature [feature] = true;

        if (feature >= LibXtract::SpectralCentroid)
        {
            spectrumNeeded = true;
            libXtractSpectrumNeeded = true;
        }

        if (feature >= LibXtract::PeakSpectralCentroid)
        {
            libXtractPeakSpectrumNeeded = true;
        }

        if (feature >= LibXtract::PeakTristimulus1)
        {
            calculateLibXtractScalarFeature [LibXtract::FundamentalFrequency] = true;
        }

        if (feature >= LibXtract::HarmonicSpectralCentroid)
        {
            libXtractHarmonicSpectrumNeeded = true;
        }

        // make sure prerequisite features are calculated
        switch (feature)
        {
            // temporal features
            case LibXtract::TemporalStandardDeviation:
                calculateLibXtractScalarFeature [LibXtract::TemporalVariance] = true;

            case LibXtract::TemporalVariance:
                calculateLibXtractScalarFeature [LibXtract::TemporalMean] = true;
                break;

            // spectral features
            case LibXtract::SpectralSkewness:
            case LibXtract::SpectralKurtosis:
                calculateLibXtractScalarFeature [LibXtract::SpectralStandardDeviation] = true;

            case LibXtract::SpectralStandardDeviation:
                calculateLibXtractScalarFeature [LibXtract::SpectralVariance] = true;

            case LibXtract::SpectralVariance:
                calculateLibXtractScalarFeature [LibXtract::SpectralCentroid] = true;
                break;

            case LibXtract::Tonality:
                calculateLibXtractScalarFeature [LibXtract::SpectralFlatness] = true;
                break;

            // peak spectral features
            case LibXtract::PeakSpectralSkewness:
            case LibXtract::PeakSpectralKurtosis:
                calculateLibXtractScalarFeature [LibXtract::PeakSpectralStandardDeviation] = true;

            case LibXtract::PeakSpectralStandardDeviation:
                calculateLibXtractScalarFeature [LibXtract::PeakSpectralVariance] = true;

            case LibXtract::PeakSpectralVariance:
                calculateLibXtractScalarFeature [LibXtract::PeakSpectralCentroid] = true;
                break;

            // harmonic spectral features
            case LibXtract::HarmonicSpectralSkewness:
            case LibXtract::HarmonicSpectralKurtosis:
                calculateLibXtractScalarFeature [LibXtract::HarmonicSpectralStandardDeviation] = true;

            case LibXtract::HarmonicSpectralStandardDeviation:
                calculateLibXtractScalarFeature [LibXtract::HarmonicSpectralVariance] = true;

            case LibXtract::HarmonicSpectralVariance:
                calculateLibXtractScalarFeature [LibXtract::HarmonicSpectralCentroid] = true;
                break;

            default:
                break;
        }
    }
    else
    {
        switch (feature)
        {
            case LibXtract::BarkCoefficients:
                spectrumNeeded = true;
                libXtractSpectrumNeeded = true;
                calculateLibXtractBarkCoefficients = true;
                break;

            case LibXtract::MFCCs:
                spectrumNeeded = true;
                libXtractSpectrumNeeded = true;
                calculateLibXtractMFCCs = true;
                break;

            case LibXtract::TemporalFeatures:

                for (int i = LibXtract::TemporalMean; i <= LibXtract::ZeroCrossingRate; ++i)
                {
                    calculateLibXtractScalarFeature [i] = true;
                    saveLibXtractScalarFeature [i] = true;
                }

                break;

            case LibXtract::SpectralFeatures:

                for (int i = LibXtract::FundamentalFrequency; i <= LibXtract::SpectralSlope; ++i)
                {
                    calculateLibXtractScalarFeature [i] = true;
                    saveLibXtractScalarFeature [i] = true;
                }

                spectrumNeeded = true;
                libXtractSpectrumNeeded = true;
                break;

            case LibXtract::PeakSpectralFeatures:

                for (int i = LibXtract::PeakSpectralCentroid; i <= LibXtract::PeakTristimulus3; ++i)
                {
                    calculateLibXtractScalarFeature [i] = true;
                    saveLibXtractScalarFeature [i] = true;
                }

                calculateLibXtractScalarFeature [LibXtract::FundamentalFrequency] = true;
                spectrumNeeded = true;
                libXtractSpectrumNeeded = true;
                libXtractPeakSpectrumNeeded = true;
                break;

            case LibXtract::HarmonicSpectralFeatures:

                for (int i = LibXtract::Inharmonicity; i <= LibXtract::HarmonicParityRatio; ++i)
                {
                    calculateLibXtractScalarFeature [i] = true;
                    saveLibXtractScalarFeature [i] = true;
                }

                calculateLibXtractScalarFeature [LibXtract::FundamentalFrequency] = true;
                spectrumNeeded = true;
                libXtractSpectrumNeeded = true;
                libXtractPeakSpectrumNeeded = true;
                libXtractHarmonicSpectrumNeeded = true;
                break;

			case LibXtract::AllFeatures:
                
                for (int i = 0; i < LibXtract::NumScalarFeatures; ++i)
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

void SAFEFeatureExtractor::addVampPlugin (const String &libraryName, const String &pluginName)
{
    VampPluginKey pluginKey = vampPluginLoader->composePluginKey (libraryName.toRawUTF8(),
                                                                  pluginName.toRawUTF8());
    vampPluginKeys.add (pluginKey);
}

void SAFEFeatureExtractor::analyseAudio (AudioSampleBuffer &buffer)
{
    // the number of channels passed in must be the number the extractor was
    // initialised for
    jassert (buffer.getNumChannels() == numChannels);

    int numSamples = buffer.getNumSamples();

    // can't do any analysis without enough samples
    if (numSamples < frameSize)
    {
        return;
    }

    featureList.clear();

    int lastFrameStart = numSamples - frameSize;

    float **audioData = buffer.getArrayOfWritePointers();

    for (int frameStart = 0; frameStart <= lastFrameStart; frameStart += stepSize)
    {
        AudioSampleBuffer frameBuffer (audioData, numChannels, frameStart, frameSize);

        calculateSpectra (frameBuffer);
        calculateLibXtractFeatures (frameBuffer);
        addLibXtractFeaturesToList (featureList, frameStart);
    }
}

void SAFEFeatureExtractor::addFeaturesToXmlElement (XmlElement *element)
{
    int numFeatures = featureList.size();

    for (int i = numFeatures - 1; i >= 0; --i)
    {
        const AudioFeature &currentFeature = featureList.getReference (i);
        int numValues = currentFeature.values.size();

        for (int i = numValues - 1; i >= 0; --i)
        {
            XmlElement *featureElement = new XmlElement ("AudioFeature");
            featureElement->setAttribute ("Name", currentFeature.name);
            featureElement->setAttribute ("Time", currentFeature.timeStamp);
            featureElement->setAttribute ("Channel", currentFeature.channelNumber);
            featureElement->setAttribute ("ArrayIndex", i);
            featureElement->setAttribute ("Value", currentFeature.values [i]);
            featureElement->setAttribute ("HasDuration", currentFeature.hasDuration);
            featureElement->setAttribute ("Duration", currentFeature.duration);

            element->prependChildElement (featureElement);
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
    float singleBinGain = 1.0f / frameSize;
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

        libXtractMelFiltersInitialised = false;
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
            double magnitude = pow (pow (realPart, 2.0) + pow (imagPart, 2.0), 0.5);

            libXtractSpectrumChannel [i - 1] = magnitude;
            libXtractSpectrumChannel [i - 1 + numBins] = i * binWidth;
        }
    }
}

void SAFEFeatureExtractor::calculateLibXtractFeatures (const AudioSampleBuffer &frame)
{
    calculateLibXtractSpectra();

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
        if (calculateLibXtractScalarFeature [LibXtract::TemporalMean])
        {
            xtract_mean (libXtractChannelData, 
                         frameSize, 
                         NULL, 
                         scalarFeatureValues + LibXtract::TemporalMean);
        }

        if (calculateLibXtractScalarFeature [LibXtract::TemporalVariance])
        {
            xtract_variance (libXtractChannelData, 
                             frameSize, 
                             scalarFeatureValues + LibXtract::TemporalMean, 
                             scalarFeatureValues + LibXtract::TemporalVariance);
        }

        if (calculateLibXtractScalarFeature [LibXtract::TemporalStandardDeviation])
        {
            xtract_standard_deviation (libXtractChannelData, 
                                       frameSize, 
                                       scalarFeatureValues + LibXtract::TemporalVariance, 
                                       scalarFeatureValues + LibXtract::TemporalStandardDeviation);
        }

        if (calculateLibXtractScalarFeature [LibXtract::RMSAmplitude])
        {
            xtract_rms_amplitude (libXtractChannelData, 
                                  frameSize, 
                                  NULL, 
                                  scalarFeatureValues + LibXtract::RMSAmplitude);
        }

        if (calculateLibXtractScalarFeature [LibXtract::ZeroCrossingRate])
        {
            xtract_zcr (libXtractChannelData, 
                        frameSize, 
                        NULL, 
                        scalarFeatureValues + LibXtract::ZeroCrossingRate);
        }
        
        // spectral features
        if (calculateLibXtractScalarFeature [LibXtract::FundamentalFrequency])
        {
            xtract_failsafe_f0 (libXtractChannelData, 
                                frameSize, 
                                &fs, 
                                scalarFeatureValues + LibXtract::FundamentalFrequency);
        }

        if (calculateLibXtractScalarFeature [LibXtract::SpectralCentroid])
        {
            xtract_spectral_centroid (spectrum, 
                                      frameSize, 
                                      NULL, 
                                      scalarFeatureValues + LibXtract::SpectralCentroid);
        }

        if (calculateLibXtractScalarFeature [LibXtract::SpectralVariance])
        {
            xtract_spectral_variance (spectrum, 
                                      frameSize, 
                                      scalarFeatureValues + LibXtract::SpectralCentroid, 
                                      scalarFeatureValues + LibXtract::SpectralVariance);
        }

        if (calculateLibXtractScalarFeature [LibXtract::SpectralStandardDeviation])
        {
            xtract_spectral_standard_deviation (spectrum, 
                                                frameSize, 
                                                scalarFeatureValues + LibXtract::SpectralVariance, 
                                                scalarFeatureValues + LibXtract::SpectralStandardDeviation);
        }

        double argumentArray [2] = {scalarFeatureValues [LibXtract::SpectralCentroid],
                                    scalarFeatureValues [LibXtract::SpectralStandardDeviation]};

        if (calculateLibXtractScalarFeature [LibXtract::SpectralSkewness])
        {
            xtract_spectral_skewness (spectrum, 
                                      frameSize, 
                                      argumentArray, 
                                      scalarFeatureValues + LibXtract::SpectralSkewness);
        }

        if (calculateLibXtractScalarFeature [LibXtract::SpectralKurtosis])
        {
            xtract_spectral_kurtosis (spectrum, 
                                      frameSize, 
                                      argumentArray, 
                                      scalarFeatureValues + LibXtract::SpectralKurtosis);
        }

        if (calculateLibXtractScalarFeature [LibXtract::JensenIrregularity])
        {
            xtract_irregularity_j (spectrum, 
                                   frameSize / 2, 
                                   NULL, 
                                   scalarFeatureValues + LibXtract::JensenIrregularity);
        }

        if (calculateLibXtractScalarFeature [LibXtract::KrimphoffIrregularity])
        {
            xtract_irregularity_k (spectrum, 
                                   frameSize / 2, 
                                   NULL, 
                                   scalarFeatureValues + LibXtract::KrimphoffIrregularity);
        }

        if (calculateLibXtractScalarFeature [LibXtract::SpectralSmoothness])
        {
            xtract_smoothness (spectrum, 
                               frameSize / 2, 
                               NULL, 
                               scalarFeatureValues + LibXtract::SpectralSmoothness);
        }

        argumentArray [0] = fs / frameSize;
        argumentArray [1] = 45;

        if (calculateLibXtractScalarFeature [LibXtract::SpectralRollOff])
        {
            xtract_rolloff (spectrum, 
                            frameSize / 2, 
                            argumentArray, 
                            scalarFeatureValues + LibXtract::SpectralRollOff);
        }

        if (calculateLibXtractScalarFeature [LibXtract::SpectralFlatness])
        {
            xtract_flatness (spectrum, 
                             frameSize / 2, 
                             NULL, 
                             scalarFeatureValues + LibXtract::SpectralFlatness);
        }

        if (calculateLibXtractScalarFeature [LibXtract::Tonality])
        {
            double logFlatness;

            xtract_flatness_db (NULL,
                                0,
                                scalarFeatureValues + LibXtract::SpectralFlatness,
                                &logFlatness);

            xtract_tonality (NULL, 
                             0, 
                             &logFlatness, 
                             scalarFeatureValues + LibXtract::Tonality);
        }

        if (calculateLibXtractScalarFeature [LibXtract::CrestFactor])
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
                          scalarFeatureValues + LibXtract::CrestFactor);
        }

        if (calculateLibXtractScalarFeature [LibXtract::SpectralSlope])
        {
            xtract_spectral_slope (spectrum, 
                                   frameSize, 
                                   NULL, 
                                   scalarFeatureValues + LibXtract::SpectralSlope);
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

        if (calculateLibXtractScalarFeature [LibXtract::PeakSpectralCentroid])
        {
            xtract_spectral_centroid (peakSpectrum, 
                                      frameSize, 
                                      NULL, 
                                      scalarFeatureValues + LibXtract::PeakSpectralCentroid);
        }

        if (calculateLibXtractScalarFeature [LibXtract::PeakSpectralVariance])
        {
            xtract_spectral_variance (peakSpectrum, 
                                      frameSize, 
                                      scalarFeatureValues + LibXtract::PeakSpectralCentroid, 
                                      scalarFeatureValues + LibXtract::PeakSpectralVariance);
        }

        if (calculateLibXtractScalarFeature [LibXtract::PeakSpectralStandardDeviation])
        {
            xtract_spectral_standard_deviation (peakSpectrum, 
                                                frameSize, 
                                                scalarFeatureValues + LibXtract::PeakSpectralVariance, 
                                                scalarFeatureValues + LibXtract::PeakSpectralStandardDeviation);
        }

        argumentArray [0] = scalarFeatureValues [LibXtract::PeakSpectralCentroid];
        argumentArray [1] = scalarFeatureValues [LibXtract::PeakSpectralStandardDeviation];

        if (calculateLibXtractScalarFeature [LibXtract::PeakSpectralSkewness])
        {
            xtract_spectral_skewness (peakSpectrum, 
                                      frameSize, 
                                      argumentArray, 
                                      scalarFeatureValues + LibXtract::PeakSpectralSkewness);
        }

        if (calculateLibXtractScalarFeature [LibXtract::PeakSpectralKurtosis])
        {
            xtract_spectral_kurtosis (peakSpectrum, 
                                      frameSize, 
                                      argumentArray, 
                                      scalarFeatureValues + LibXtract::PeakSpectralKurtosis);
        }

        if (calculateLibXtractScalarFeature [LibXtract::PeakJensenIrregularity])
        {
            xtract_irregularity_j (peakSpectrum, 
                                   frameSize / 2, 
                                   NULL, 
                                   scalarFeatureValues + LibXtract::PeakJensenIrregularity);
        }

        if (calculateLibXtractScalarFeature [LibXtract::PeakKrimphoffIrregularity])
        {
            xtract_irregularity_k (peakSpectrum, 
                                   frameSize / 2, 
                                   NULL, 
                                   scalarFeatureValues + LibXtract::PeakKrimphoffIrregularity);
        }

        if (calculateLibXtractScalarFeature [LibXtract::PeakTristimulus1])
        {
            xtract_tristimulus_1 (peakSpectrum, 
                                  frameSize, 
                                  scalarFeatureValues + LibXtract::FundamentalFrequency, 
                                  scalarFeatureValues + LibXtract::PeakTristimulus1);
        }

        if (calculateLibXtractScalarFeature [LibXtract::PeakTristimulus2])
        {
            xtract_tristimulus_2 (peakSpectrum, 
                                  frameSize, 
                                  scalarFeatureValues + LibXtract::FundamentalFrequency, 
                                  scalarFeatureValues + LibXtract::PeakTristimulus2);
        }

        if (calculateLibXtractScalarFeature [LibXtract::PeakTristimulus3])
        {
            xtract_tristimulus_3 (peakSpectrum, 
                                  frameSize, 
                                  scalarFeatureValues + LibXtract::FundamentalFrequency, 
                                  scalarFeatureValues + LibXtract::PeakTristimulus3);
        }

        // harmonic spectral features
        
        if (calculateLibXtractScalarFeature [LibXtract::Inharmonicity])
        {
            xtract_spectral_inharmonicity (peakSpectrum, 
                                           frameSize, 
                                           scalarFeatureValues + LibXtract::FundamentalFrequency, 
                                           scalarFeatureValues + LibXtract::Inharmonicity);
        }

        if (libXtractHarmonicSpectrumNeeded)
        {
            argumentArray [0] = scalarFeatureValues [LibXtract::FundamentalFrequency];
            argumentArray [1] = 0.2;

            xtract_harmonic_spectrum (peakSpectrum,
                                      frameSize,
                                      argumentArray,
                                      harmonicSpectrum);
        }

        if (calculateLibXtractScalarFeature [LibXtract::HarmonicSpectralCentroid])
        {
            xtract_spectral_centroid (harmonicSpectrum, 
                                      frameSize, 
                                      NULL, 
                                      scalarFeatureValues + LibXtract::HarmonicSpectralCentroid);
        }

        if (calculateLibXtractScalarFeature [LibXtract::HarmonicSpectralVariance])
        {
            xtract_spectral_variance (harmonicSpectrum, 
                                      frameSize, 
                                      scalarFeatureValues + LibXtract::HarmonicSpectralCentroid, 
                                      scalarFeatureValues + LibXtract::HarmonicSpectralVariance);
        }

        if (calculateLibXtractScalarFeature [LibXtract::HarmonicSpectralStandardDeviation])
        {
            xtract_spectral_standard_deviation (harmonicSpectrum, 
                                                frameSize, 
                                                scalarFeatureValues + LibXtract::HarmonicSpectralVariance, 
                                                scalarFeatureValues + LibXtract::HarmonicSpectralStandardDeviation);
        }

        argumentArray [0] = scalarFeatureValues [LibXtract::HarmonicSpectralCentroid];
        argumentArray [1] = scalarFeatureValues [LibXtract::HarmonicSpectralStandardDeviation];

        if (calculateLibXtractScalarFeature [LibXtract::HarmonicSpectralSkewness])
        {
            xtract_spectral_skewness (harmonicSpectrum, 
                                      frameSize, 
                                      argumentArray, 
                                      scalarFeatureValues + LibXtract::HarmonicSpectralSkewness);
        }

        if (calculateLibXtractScalarFeature [LibXtract::HarmonicSpectralKurtosis])
        {
            xtract_spectral_kurtosis (harmonicSpectrum, 
                                      frameSize, 
                                      argumentArray, 
                                      scalarFeatureValues + LibXtract::HarmonicSpectralKurtosis);
        }

        if (calculateLibXtractScalarFeature [LibXtract::HarmonicJensenIrregularity])
        {
            xtract_irregularity_j (harmonicSpectrum, 
                                   frameSize / 2, 
                                   NULL, 
                                   scalarFeatureValues + LibXtract::HarmonicJensenIrregularity);
        }

        if (calculateLibXtractScalarFeature [LibXtract::HarmonicKrimphoffIrregularity])
        {
            xtract_irregularity_k (harmonicSpectrum, 
                                   frameSize / 2, 
                                   NULL, 
                                   scalarFeatureValues + LibXtract::HarmonicKrimphoffIrregularity);
        }

        if (calculateLibXtractScalarFeature [LibXtract::HarmonicTristimulus1])
        {
            xtract_tristimulus_1 (harmonicSpectrum, 
                                  frameSize, 
                                  scalarFeatureValues + LibXtract::FundamentalFrequency, 
                                  scalarFeatureValues + LibXtract::HarmonicTristimulus1);
        }

        if (calculateLibXtractScalarFeature [LibXtract::HarmonicTristimulus2])
        {
            xtract_tristimulus_2 (harmonicSpectrum, 
                                  frameSize, 
                                  scalarFeatureValues + LibXtract::FundamentalFrequency, 
                                  scalarFeatureValues + LibXtract::HarmonicTristimulus2);
        }

        if (calculateLibXtractScalarFeature [LibXtract::HarmonicTristimulus3])
        {
            xtract_tristimulus_3 (harmonicSpectrum, 
                                  frameSize, 
                                  scalarFeatureValues + LibXtract::FundamentalFrequency, 
                                  scalarFeatureValues + LibXtract::HarmonicTristimulus3);
        }

        if (calculateLibXtractScalarFeature [LibXtract::Noisiness])
        {
            xtract_nonzero_count (harmonicSpectrum,
                                  frameSize / 2,
                                  NULL,
                                  argumentArray);

            xtract_nonzero_count (peakSpectrum,
                                  frameSize / 2,
                                  NULL,
                                  argumentArray + 1);

            xtract_noisiness (NULL, 
                              0, 
                              argumentArray, 
                              scalarFeatureValues + LibXtract::Noisiness);
        }

        if (calculateLibXtractScalarFeature [LibXtract::HarmonicParityRatio])
        {
            xtract_odd_even_ratio (harmonicSpectrum, 
                                   frameSize, 
                                   scalarFeatureValues + LibXtract::FundamentalFrequency, 
                                   scalarFeatureValues + LibXtract::HarmonicParityRatio);
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

            xtract_mfcc (spectrum,
                         frameSize / 2,
                         &libXtractMelFilters,
                         mfccs);
        }
    }
}

void SAFEFeatureExtractor::addLibXtractFeaturesToList (Array <AudioFeature> &featureList, int timeStamp)
{
    for (int channel = 0; channel < numChannels; ++channel)
    {
        for (int i = LibXtract::TemporalMean; i < LibXtract::NumScalarFeatures; ++i)
        {
            if (saveLibXtractScalarFeature [i])
            {
                AudioFeature tempFeature;
                tempFeature.name = LibXtract::getFeatureName (static_cast <LibXtract::Feature> (i));
                tempFeature.timeStamp = timeStamp;
                tempFeature.channelNumber = channel;
                tempFeature.values.add (libXtractScalarFeatureValues.getReference (channel) [i]);
                tempFeature.hasDuration = false;
                tempFeature.duration = 0;

                featureList.add (tempFeature);
            }
        }

        if (calculateLibXtractBarkCoefficients)
        {
			AudioFeature tempFeature;

            tempFeature.name = LibXtract::getFeatureName (LibXtract::BarkCoefficients);
            tempFeature.timeStamp = timeStamp;
            tempFeature.channelNumber = channel;
            tempFeature.values = libXtractBarkCoefficients.getReference (channel);
            tempFeature.hasDuration = false;
            tempFeature.duration = 0;

            featureList.add (tempFeature);
        }

        if (calculateLibXtractMFCCs)
        {
			AudioFeature tempFeature;

            tempFeature.name = LibXtract::getFeatureName (LibXtract::MFCCs);
            tempFeature.timeStamp = timeStamp;
            tempFeature.channelNumber = channel;
            tempFeature.values = libXtractMFCCs.getReference (channel);
            tempFeature.hasDuration = false;
            tempFeature.duration = 0;

            featureList.add (tempFeature);
        }
    }
}
