//==========================================================================
//      Constructor and Destructor
//==========================================================================
SAFEFeatureExtractor::SAFEFeatureExtractor()
    : initialised (false),
      numChannels (0),
      defaultFrameSize (0),
      defaultStepSize (0),
      fs (0.0),
      windowingFunction (applyHannWindow),
      libXtractSpectrumNeeded (false),
      libXtractPeakSpectrumNeeded (false),
      libXtractHarmonicSpectrumNeeded (false),
      calculateLibXtractBarkCoefficients (false),
      libXtractMelFiltersInitialised (false),
      calculateLibXtractMFCCs (false),
      nextVampFeatureTimeStamp (0)
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
void SAFEFeatureExtractor::initialise (int numChannelsInit, int defaultFrameSizeInit, int defaultStepSizeInit, double sampleRate)
{
    // save the number of channels
    numChannels = numChannelsInit;

    // calculate the frame size
    defaultFrameSize = defaultFrameSizeInit;

    // make sure the step size is sensible
    if (defaultStepSizeInit > defaultFrameSize || defaultStepSizeInit < 1)
    {
        defaultStepSize = defaultFrameSize;
    }
    else
    {
        defaultStepSize = defaultStepSizeInit;
    }

    // save the sample rate
    fs = sampleRate;

    analysisConfigurations.clear();

    // initialise libxtract bits
    initialiseLibXtract();

    // initialise the vamp stuff
    initialiseVampPlugins();

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
                libXtractSpectrumNeeded = true;
                calculateLibXtractBarkCoefficients = true;
                break;

            case LibXtract::MFCCs:
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

                libXtractSpectrumNeeded = true;
                break;

            case LibXtract::PeakSpectralFeatures:

                for (int i = LibXtract::PeakSpectralCentroid; i <= LibXtract::PeakTristimulus3; ++i)
                {
                    calculateLibXtractScalarFeature [i] = true;
                    saveLibXtractScalarFeature [i] = true;
                }

                calculateLibXtractScalarFeature [LibXtract::FundamentalFrequency] = true;
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
    float **audioData = buffer.getArrayOfWritePointers();

    featureLists.clear();

    resetVampPlugins();

    for (int i = 0; i < analysisConfigurations.size(); ++i)
    {
        AnalysisConfiguration *config = analysisConfigurations [i];

        int currentFrameSize = config->frameSize;
        int currentStepSize = config->stepSize;

        int lastFrameStart = numSamples - currentFrameSize;

        for (int frameStart = 0; frameStart <= lastFrameStart; frameStart += currentStepSize)
        {
            AudioSampleBuffer frameBuffer (audioData, numChannels, frameStart, currentFrameSize);

            calculateSpectra (frameBuffer);

            int time = 1000 * frameStart / fs;

            if (config->libXtractConfiguration)
            {
                calculateLibXtractFeatures (frameBuffer);
                addLibXtractFeaturesToList (time);
            }

            calculateVampPluginFeatures (config->vampPluginIndicies, frameBuffer, time);
        }
    }
    
    getRemainingVampPluginFeatures();
}

void SAFEFeatureExtractor::setWindowingFunction (void (*newWindowingFunction) (float*, int))
{
    windowingFunction = newWindowingFunction;
}

void SAFEFeatureExtractor::addFeaturesToXmlElement (XmlElement *element)
{
    for (AudioFeatureSet::reverse_iterator i = featureLists.rbegin(); i != featureLists.rend(); ++i)
    {
        String featureName = i->first;

        XmlElement *featureElement = new XmlElement ("AudioFeature");
        featureElement->setAttribute ("Name", featureName);

        AudioFeatureList &featureList = i->second;
        int numFeatures = featureList.features.size();

        featureElement->setAttribute ("FrameSize", featureList.frameSize);
        featureElement->setAttribute ("StepSize", featureList.stepSize);

        for (int i = numFeatures - 1; i >= 0; --i)
        {
            AudioFeature &currentFeature = featureList.features.getReference (i);

            XmlElement *valueElement = new XmlElement ("Value");
            valueElement->setAttribute ("TimeStamp", currentFeature.timeStamp);
            valueElement->setAttribute ("Duration", currentFeature.duration);
            valueElement->setAttribute ("Channel", currentFeature.channelNumber);

            String valueString;
            int numValues = currentFeature.values.size();

            for (int d = 0; d < numValues - 1; ++d)
            {
                valueString += String (currentFeature.values [d]) + ", ";
            }

            if (numValues)
            {
                valueString += String (currentFeature.values.getLast());
            }

            valueElement->setAttribute ("Values", valueString);

            featureElement->prependChildElement (valueElement);
        }

        element->prependChildElement (featureElement);
    }
}

void SAFEFeatureExtractor::cacheNewFFT (int size)
{
    // make a new FFT object if needs be
    if (! fftCache.count (size))
    {
        int frameOrder = floor (log (size) / log (2));

        // frame size must be a power of two
        jassert (pow (2, frameOrder) == size);

        fftCache.insert (std::pair <int, ScopedPointer <FFT> > (size, 
                                                                new FFT (frameOrder, false)));
    }

    if (! spectraCache.count (size))
    {
        spectraCache [size].setSize (numChannels, size * 2);
    }
}

void SAFEFeatureExtractor::calculateSpectra (const AudioSampleBuffer &frame)
{
    int numSamples = frame.getNumSamples();

    if (fftCache.count (numSamples) == 0)
    {
        return;
    }

    FFT *fft = fftCache [numSamples];
    AudioSampleBuffer &spectra = spectraCache [numSamples];

    for (int i = 0; i < numChannels; ++i)
    {
        spectra.copyFrom (i, 0, frame, i, 0, numSamples);
        windowingFunction (spectra.getWritePointer (i), numSamples);
        fft->performRealOnlyForwardTransform (spectra.getWritePointer (i));
    }

    // scale spectra
    float singleBinGain = 1.0f / numSamples;
    float duplicateBinGain = 2.0f * singleBinGain;
    spectra.applyGain (0, 1, singleBinGain);
    spectra.applyGain (1, 1, 0);
    spectra.applyGain (2, numSamples - 2, duplicateBinGain);
    spectra.applyGain (numSamples, 1, singleBinGain);
    spectra.applyGain (numSamples + 1, 1, 0);
}

void SAFEFeatureExtractor::applyHannWindow (float *data, int numSamples)
{
    for (int i = 0; i < numSamples; ++i)
    {
        data [i] *= 0.5 * (1 - cos (2 * float_Pi * i / (numSamples - 1)));
    }
}

SAFEFeatureExtractor::AnalysisConfiguration* SAFEFeatureExtractor::addNewAnalysisConfiguration(int frameSize, int stepSize, bool libXtractConfiguration)
{
    AnalysisConfiguration *config = new AnalysisConfiguration;

    config->frameSize = frameSize;
    config->stepSize = stepSize;
    config->libXtractConfiguration = libXtractConfiguration;

    return analysisConfigurations.add (config);
}

void SAFEFeatureExtractor::addVampPluginToAnalysisConfigurations (int pluginIndex, int frameSize, int stepSize)
{
    AnalysisConfiguration *configToAlter = nullptr;
    bool configExists = false;

    for (int i = 0; i < analysisConfigurations.size(); ++i)
    {
        AnalysisConfiguration *config = analysisConfigurations [i];

        if (config->frameSize == frameSize && config->stepSize == stepSize)
        {
            configToAlter = config;
            configExists = true;
            break;
        }
    }

    if (! configExists)
    {
        configToAlter = addNewAnalysisConfiguration (frameSize, stepSize, false);
    }

    configToAlter->vampPluginIndicies.add (pluginIndex);

}

const SAFEFeatureExtractor::AnalysisConfiguration* SAFEFeatureExtractor::getVampPluginAnalysisConfiguration (int pluginIndex)
{
    for (int i = 0; i < analysisConfigurations.size(); ++i)
    {
        const AnalysisConfiguration *config = analysisConfigurations [i];

        if (config->vampPluginIndicies.contains (pluginIndex))
        {
            return config;
        }
    }
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

void SAFEFeatureExtractor::initialiseLibXtract()
{
    libXtractScalarFeatureValues.resize (numChannels);

    xtract_init_bark (defaultFrameSize, fs, libXtractBarkBandLimits);
    libXtractBarkCoefficients.resize (numChannels);

    deleteLibXtractMelFilters();

    for (int i = 0; i < libXtractMelFilters.n_filters; ++i)
    {
        libXtractMelFilters.filters [i] = new double [defaultFrameSize];
    }

    xtract_init_mfcc (defaultFrameSize / 2, fs / 2, XTRACT_EQUAL_GAIN, 20, 20000,
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

        libXtractSpectra.getReference (i).resize (defaultFrameSize);
        libXtractPeakSpectra.getReference (i).resize (defaultFrameSize);
        libXtractHarmonicSpectra.getReference (i).resize (defaultFrameSize);
    }

    libXtractChannelData.allocate (defaultFrameSize, true);

    if (libXtractSpectrumNeeded)
    {
        cacheNewFFT (defaultFrameSize);
    }

    addNewAnalysisConfiguration (defaultFrameSize, defaultStepSize, true);
}

void SAFEFeatureExtractor::calculateLibXtractSpectra()
{
    if (! libXtractSpectrumNeeded)
    {
        return;
    }

    int numBins = defaultFrameSize / 2;
    double binWidth = fs / defaultFrameSize;
    AudioSampleBuffer &spectra = spectraCache [defaultFrameSize];

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

        for (int i = 0; i < defaultFrameSize; ++i)
        {
            libXtractChannelData [i] = channelData [i];
        }

        // temporal features
        if (calculateLibXtractScalarFeature [LibXtract::TemporalMean])
        {
            xtract_mean (libXtractChannelData, 
                         defaultFrameSize, 
                         NULL, 
                         scalarFeatureValues + LibXtract::TemporalMean);
        }

        if (calculateLibXtractScalarFeature [LibXtract::TemporalVariance])
        {
            xtract_variance (libXtractChannelData, 
                             defaultFrameSize, 
                             scalarFeatureValues + LibXtract::TemporalMean, 
                             scalarFeatureValues + LibXtract::TemporalVariance);
        }

        if (calculateLibXtractScalarFeature [LibXtract::TemporalStandardDeviation])
        {
            xtract_standard_deviation (libXtractChannelData, 
                                       defaultFrameSize, 
                                       scalarFeatureValues + LibXtract::TemporalVariance, 
                                       scalarFeatureValues + LibXtract::TemporalStandardDeviation);
        }

        if (calculateLibXtractScalarFeature [LibXtract::RMSAmplitude])
        {
            xtract_rms_amplitude (libXtractChannelData, 
                                  defaultFrameSize, 
                                  NULL, 
                                  scalarFeatureValues + LibXtract::RMSAmplitude);
        }

        if (calculateLibXtractScalarFeature [LibXtract::ZeroCrossingRate])
        {
            xtract_zcr (libXtractChannelData, 
                        defaultFrameSize, 
                        NULL, 
                        scalarFeatureValues + LibXtract::ZeroCrossingRate);
        }
        
        // spectral features
        if (calculateLibXtractScalarFeature [LibXtract::FundamentalFrequency])
        {
            xtract_failsafe_f0 (libXtractChannelData, 
                                defaultFrameSize, 
                                &fs, 
                                scalarFeatureValues + LibXtract::FundamentalFrequency);
        }

        if (calculateLibXtractScalarFeature [LibXtract::SpectralCentroid])
        {
            xtract_spectral_centroid (spectrum, 
                                      defaultFrameSize, 
                                      NULL, 
                                      scalarFeatureValues + LibXtract::SpectralCentroid);
        }

        if (calculateLibXtractScalarFeature [LibXtract::SpectralVariance])
        {
            xtract_spectral_variance (spectrum, 
                                      defaultFrameSize, 
                                      scalarFeatureValues + LibXtract::SpectralCentroid, 
                                      scalarFeatureValues + LibXtract::SpectralVariance);
        }

        if (calculateLibXtractScalarFeature [LibXtract::SpectralStandardDeviation])
        {
            xtract_spectral_standard_deviation (spectrum, 
                                                defaultFrameSize, 
                                                scalarFeatureValues + LibXtract::SpectralVariance, 
                                                scalarFeatureValues + LibXtract::SpectralStandardDeviation);
        }

        double argumentArray [2] = {scalarFeatureValues [LibXtract::SpectralCentroid],
                                    scalarFeatureValues [LibXtract::SpectralStandardDeviation]};

        if (calculateLibXtractScalarFeature [LibXtract::SpectralSkewness])
        {
            xtract_spectral_skewness (spectrum, 
                                      defaultFrameSize, 
                                      argumentArray, 
                                      scalarFeatureValues + LibXtract::SpectralSkewness);
        }

        if (calculateLibXtractScalarFeature [LibXtract::SpectralKurtosis])
        {
            xtract_spectral_kurtosis (spectrum, 
                                      defaultFrameSize, 
                                      argumentArray, 
                                      scalarFeatureValues + LibXtract::SpectralKurtosis);
        }

        if (calculateLibXtractScalarFeature [LibXtract::JensenIrregularity])
        {
            xtract_irregularity_j (spectrum, 
                                   defaultFrameSize / 2, 
                                   NULL, 
                                   scalarFeatureValues + LibXtract::JensenIrregularity);
        }

        if (calculateLibXtractScalarFeature [LibXtract::KrimphoffIrregularity])
        {
            xtract_irregularity_k (spectrum, 
                                   defaultFrameSize / 2, 
                                   NULL, 
                                   scalarFeatureValues + LibXtract::KrimphoffIrregularity);
        }

        if (calculateLibXtractScalarFeature [LibXtract::SpectralSmoothness])
        {
            xtract_smoothness (spectrum, 
                               defaultFrameSize / 2, 
                               NULL, 
                               scalarFeatureValues + LibXtract::SpectralSmoothness);
        }

        argumentArray [0] = fs / defaultFrameSize;
        argumentArray [1] = 45;

        if (calculateLibXtractScalarFeature [LibXtract::SpectralRollOff])
        {
            xtract_rolloff (spectrum, 
                            defaultFrameSize / 2, 
                            argumentArray, 
                            scalarFeatureValues + LibXtract::SpectralRollOff);
        }

        if (calculateLibXtractScalarFeature [LibXtract::SpectralFlatness])
        {
            xtract_flatness (spectrum, 
                             defaultFrameSize / 2, 
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
                                  defaultFrameSize / 2,
                                  NULL,
                                  argumentArray);

            xtract_mean (spectrum,
                         defaultFrameSize / 2,
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
                                   defaultFrameSize, 
                                   NULL, 
                                   scalarFeatureValues + LibXtract::SpectralSlope);
        }

        // peak spectral features
        if (libXtractPeakSpectrumNeeded)
        {
            argumentArray [0] = fs / defaultFrameSize;
            argumentArray [1] = 10;

            xtract_peak_spectrum (spectrum,
                                  defaultFrameSize / 2,
                                  argumentArray,
                                  peakSpectrum);
        }

        if (calculateLibXtractScalarFeature [LibXtract::PeakSpectralCentroid])
        {
            xtract_spectral_centroid (peakSpectrum, 
                                      defaultFrameSize, 
                                      NULL, 
                                      scalarFeatureValues + LibXtract::PeakSpectralCentroid);
        }

        if (calculateLibXtractScalarFeature [LibXtract::PeakSpectralVariance])
        {
            xtract_spectral_variance (peakSpectrum, 
                                      defaultFrameSize, 
                                      scalarFeatureValues + LibXtract::PeakSpectralCentroid, 
                                      scalarFeatureValues + LibXtract::PeakSpectralVariance);
        }

        if (calculateLibXtractScalarFeature [LibXtract::PeakSpectralStandardDeviation])
        {
            xtract_spectral_standard_deviation (peakSpectrum, 
                                                defaultFrameSize, 
                                                scalarFeatureValues + LibXtract::PeakSpectralVariance, 
                                                scalarFeatureValues + LibXtract::PeakSpectralStandardDeviation);
        }

        argumentArray [0] = scalarFeatureValues [LibXtract::PeakSpectralCentroid];
        argumentArray [1] = scalarFeatureValues [LibXtract::PeakSpectralStandardDeviation];

        if (calculateLibXtractScalarFeature [LibXtract::PeakSpectralSkewness])
        {
            xtract_spectral_skewness (peakSpectrum, 
                                      defaultFrameSize, 
                                      argumentArray, 
                                      scalarFeatureValues + LibXtract::PeakSpectralSkewness);
        }

        if (calculateLibXtractScalarFeature [LibXtract::PeakSpectralKurtosis])
        {
            xtract_spectral_kurtosis (peakSpectrum, 
                                      defaultFrameSize, 
                                      argumentArray, 
                                      scalarFeatureValues + LibXtract::PeakSpectralKurtosis);
        }

        if (calculateLibXtractScalarFeature [LibXtract::PeakJensenIrregularity])
        {
            xtract_irregularity_j (peakSpectrum, 
                                   defaultFrameSize / 2, 
                                   NULL, 
                                   scalarFeatureValues + LibXtract::PeakJensenIrregularity);
        }

        if (calculateLibXtractScalarFeature [LibXtract::PeakKrimphoffIrregularity])
        {
            xtract_irregularity_k (peakSpectrum, 
                                   defaultFrameSize / 2, 
                                   NULL, 
                                   scalarFeatureValues + LibXtract::PeakKrimphoffIrregularity);
        }

        if (calculateLibXtractScalarFeature [LibXtract::PeakTristimulus1])
        {
            xtract_tristimulus_1 (peakSpectrum, 
                                  defaultFrameSize, 
                                  scalarFeatureValues + LibXtract::FundamentalFrequency, 
                                  scalarFeatureValues + LibXtract::PeakTristimulus1);
        }

        if (calculateLibXtractScalarFeature [LibXtract::PeakTristimulus2])
        {
            xtract_tristimulus_2 (peakSpectrum, 
                                  defaultFrameSize, 
                                  scalarFeatureValues + LibXtract::FundamentalFrequency, 
                                  scalarFeatureValues + LibXtract::PeakTristimulus2);
        }

        if (calculateLibXtractScalarFeature [LibXtract::PeakTristimulus3])
        {
            xtract_tristimulus_3 (peakSpectrum, 
                                  defaultFrameSize, 
                                  scalarFeatureValues + LibXtract::FundamentalFrequency, 
                                  scalarFeatureValues + LibXtract::PeakTristimulus3);
        }

        // harmonic spectral features
        
        if (calculateLibXtractScalarFeature [LibXtract::Inharmonicity])
        {
            xtract_spectral_inharmonicity (peakSpectrum, 
                                           defaultFrameSize, 
                                           scalarFeatureValues + LibXtract::FundamentalFrequency, 
                                           scalarFeatureValues + LibXtract::Inharmonicity);
        }

        if (libXtractHarmonicSpectrumNeeded)
        {
            argumentArray [0] = scalarFeatureValues [LibXtract::FundamentalFrequency];
            argumentArray [1] = 0.2;

            xtract_harmonic_spectrum (peakSpectrum,
                                      defaultFrameSize,
                                      argumentArray,
                                      harmonicSpectrum);
        }

        if (calculateLibXtractScalarFeature [LibXtract::HarmonicSpectralCentroid])
        {
            xtract_spectral_centroid (harmonicSpectrum, 
                                      defaultFrameSize, 
                                      NULL, 
                                      scalarFeatureValues + LibXtract::HarmonicSpectralCentroid);
        }

        if (calculateLibXtractScalarFeature [LibXtract::HarmonicSpectralVariance])
        {
            xtract_spectral_variance (harmonicSpectrum, 
                                      defaultFrameSize, 
                                      scalarFeatureValues + LibXtract::HarmonicSpectralCentroid, 
                                      scalarFeatureValues + LibXtract::HarmonicSpectralVariance);
        }

        if (calculateLibXtractScalarFeature [LibXtract::HarmonicSpectralStandardDeviation])
        {
            xtract_spectral_standard_deviation (harmonicSpectrum, 
                                                defaultFrameSize, 
                                                scalarFeatureValues + LibXtract::HarmonicSpectralVariance, 
                                                scalarFeatureValues + LibXtract::HarmonicSpectralStandardDeviation);
        }

        argumentArray [0] = scalarFeatureValues [LibXtract::HarmonicSpectralCentroid];
        argumentArray [1] = scalarFeatureValues [LibXtract::HarmonicSpectralStandardDeviation];

        if (calculateLibXtractScalarFeature [LibXtract::HarmonicSpectralSkewness])
        {
            xtract_spectral_skewness (harmonicSpectrum, 
                                      defaultFrameSize, 
                                      argumentArray, 
                                      scalarFeatureValues + LibXtract::HarmonicSpectralSkewness);
        }

        if (calculateLibXtractScalarFeature [LibXtract::HarmonicSpectralKurtosis])
        {
            xtract_spectral_kurtosis (harmonicSpectrum, 
                                      defaultFrameSize, 
                                      argumentArray, 
                                      scalarFeatureValues + LibXtract::HarmonicSpectralKurtosis);
        }

        if (calculateLibXtractScalarFeature [LibXtract::HarmonicJensenIrregularity])
        {
            xtract_irregularity_j (harmonicSpectrum, 
                                   defaultFrameSize / 2, 
                                   NULL, 
                                   scalarFeatureValues + LibXtract::HarmonicJensenIrregularity);
        }

        if (calculateLibXtractScalarFeature [LibXtract::HarmonicKrimphoffIrregularity])
        {
            xtract_irregularity_k (harmonicSpectrum, 
                                   defaultFrameSize / 2, 
                                   NULL, 
                                   scalarFeatureValues + LibXtract::HarmonicKrimphoffIrregularity);
        }

        if (calculateLibXtractScalarFeature [LibXtract::HarmonicTristimulus1])
        {
            xtract_tristimulus_1 (harmonicSpectrum, 
                                  defaultFrameSize, 
                                  scalarFeatureValues + LibXtract::FundamentalFrequency, 
                                  scalarFeatureValues + LibXtract::HarmonicTristimulus1);
        }

        if (calculateLibXtractScalarFeature [LibXtract::HarmonicTristimulus2])
        {
            xtract_tristimulus_2 (harmonicSpectrum, 
                                  defaultFrameSize, 
                                  scalarFeatureValues + LibXtract::FundamentalFrequency, 
                                  scalarFeatureValues + LibXtract::HarmonicTristimulus2);
        }

        if (calculateLibXtractScalarFeature [LibXtract::HarmonicTristimulus3])
        {
            xtract_tristimulus_3 (harmonicSpectrum, 
                                  defaultFrameSize, 
                                  scalarFeatureValues + LibXtract::FundamentalFrequency, 
                                  scalarFeatureValues + LibXtract::HarmonicTristimulus3);
        }

        if (calculateLibXtractScalarFeature [LibXtract::Noisiness])
        {
            xtract_nonzero_count (harmonicSpectrum,
                                  defaultFrameSize / 2,
                                  NULL,
                                  argumentArray);

            xtract_nonzero_count (peakSpectrum,
                                  defaultFrameSize / 2,
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
                                   defaultFrameSize, 
                                   scalarFeatureValues + LibXtract::FundamentalFrequency, 
                                   scalarFeatureValues + LibXtract::HarmonicParityRatio);
        }

        if (calculateLibXtractBarkCoefficients)
        {
            double *barkCoefficients = libXtractBarkCoefficients.getReference (channel).getRawDataPointer();

            xtract_bark_coefficients (spectrum,
                                      defaultFrameSize / 2,
                                      libXtractBarkBandLimits,
                                      barkCoefficients);
        }

        if (calculateLibXtractMFCCs)
        {
            double *mfccs = libXtractMFCCs.getReference (channel).getRawDataPointer();

            xtract_mfcc (spectrum,
                         defaultFrameSize / 2,
                         &libXtractMelFilters,
                         mfccs);
        }
    }
}

void SAFEFeatureExtractor::addLibXtractFeaturesToList (int timeStamp)
{
    for (int channel = 0; channel < numChannels; ++channel)
    {
        for (int i = LibXtract::TemporalMean; i < LibXtract::NumScalarFeatures; ++i)
        {
            if (saveLibXtractScalarFeature [i])
            {
                String featureName = LibXtract::getFeatureName (static_cast <LibXtract::Feature> (i));
                AudioFeature tempFeature;

                tempFeature.timeStamp = timeStamp;
                tempFeature.channelNumber = channel;
                tempFeature.values.add (libXtractScalarFeatureValues.getReference (channel) [i]);
                tempFeature.duration = 0;

                if (! featureLists.count (featureName))
                {
                    featureLists [featureName].frameSize = defaultFrameSize;
                    featureLists [featureName].stepSize = defaultStepSize;
                }

                featureLists [featureName].features.add (tempFeature);
            }
        }

        if (calculateLibXtractBarkCoefficients)
        {
            String featureName = LibXtract::getFeatureName (LibXtract::BarkCoefficients);
			AudioFeature tempFeature;

            tempFeature.timeStamp = timeStamp;
            tempFeature.channelNumber = channel;
            tempFeature.values = libXtractBarkCoefficients.getReference (channel);
            tempFeature.duration = 0;

            if (! featureLists.count (featureName))
            {
                featureLists [featureName].frameSize = defaultFrameSize;
                featureLists [featureName].stepSize = defaultStepSize;
            }

            featureLists [featureName].features.add (tempFeature);
        }

        if (calculateLibXtractMFCCs)
        {
            String featureName = LibXtract::getFeatureName (LibXtract::MFCCs);
			AudioFeature tempFeature;

            tempFeature.timeStamp = timeStamp;
            tempFeature.channelNumber = channel;
            tempFeature.values = libXtractMFCCs.getReference (channel);
            tempFeature.duration = 0;

            if (! featureLists.count (featureName))
            {
                featureLists [featureName].frameSize = defaultFrameSize;
                featureLists [featureName].stepSize = defaultStepSize;
            }

            featureLists [featureName].features.add (tempFeature);
        }
    }
}

void SAFEFeatureExtractor::initialiseVampPlugins()
{
    vampPlugins.clear();
    vampOutputs.clear();

    for (int i = 0; i < vampPluginKeys.size(); ++i)
    {
        loadAndInitialiseVampPlugin (vampPluginKeys [i]);
    }
}

void SAFEFeatureExtractor::resetVampPlugins()
{
    for (int i = 0; i < vampPlugins.size(); ++i)
    {
        vampPlugins [i]->reset();
    }
}

void SAFEFeatureExtractor::loadAndInitialiseVampPlugin(const VampPluginKey &key)
{
    VampPlugin *newPlugin = vampPluginLoader->loadPlugin (key,
                                                          fs, 
                                                          VampPluginLoader::ADAPT_CHANNEL_COUNT);

    if (newPlugin == 0)
    {
        Logger::outputDebugString ("Failed to load vamp plug-in: " + key);
        return;
    }

    vampPlugins.add (newPlugin);

    int pluginFrameSize = newPlugin->getPreferredBlockSize();
    int pluginStepSize = newPlugin->getPreferredStepSize();

    if (pluginFrameSize == 0)
    {
        pluginFrameSize = defaultFrameSize;
    }

    if (pluginStepSize == 0)
    {
        pluginStepSize = defaultStepSize;
    }

    if (! newPlugin->initialise (numChannels, pluginStepSize, pluginFrameSize))
    {
        vampPlugins.removeObject (newPlugin);
        Logger::outputDebugString ("Failed to initialise vamp plug-in: " + key);
        return;
    }

    Logger::outputDebugString ("Initialised vamp plug-in: " + key + " with:\n"
                               "Frame Size: " + String (pluginFrameSize) + "\n"
                               "Step Size " + String (pluginStepSize));

    addVampPluginToAnalysisConfigurations (vampPlugins.size() - 1, pluginFrameSize, pluginStepSize);
    vampOutputs.add (newPlugin->getOutputDescriptors());

    if (newPlugin->getInputDomain() == VampPlugin::FrequencyDomain)
    {
        cacheNewFFT (pluginFrameSize);
    }
}

void SAFEFeatureExtractor::calculateVampPluginFeatures (const Array <int> &plugins, const AudioSampleBuffer &frame, int timeStamp)
{
    for (int i = 0; i < plugins.size(); ++i)
    {
        VampPlugin *currentPlugin = vampPlugins [plugins [i]];
        VampFeatureSet features;

        if (currentPlugin->getInputDomain() == VampPlugin::TimeDomain)
        {
			const float * const *audioData = frame.getArrayOfReadPointers();
            features = currentPlugin->process (audioData, VampTime::fromMilliseconds (timeStamp));
        }
        else
        {
			int numSamples = frame.getNumSamples();
			AudioSampleBuffer &spectra = spectraCache[numSamples];
			const float * const *spectralData = spectra.getArrayOfReadPointers();
            features = currentPlugin->process (spectralData, VampTime::fromMilliseconds (timeStamp));
        }

        VampOutputList &output = vampOutputs.getReference (i);
        addVampPluginFeaturesToList (output, features, timeStamp, i);
    }
}

void SAFEFeatureExtractor::getRemainingVampPluginFeatures()
{
    for (int i = 0; i < vampPlugins.size(); ++i)
    {
        VampPlugin *currentPlugin = vampPlugins [i];
        VampFeatureSet features = currentPlugin->getRemainingFeatures();

        VampOutputList &output = vampOutputs.getReference (i);
        addVampPluginFeaturesToList (output, features, 0, i);
    }
}

void SAFEFeatureExtractor::addVampPluginFeaturesToList (VampOutputList &outputs, VampFeatureSet &features, int timeStamp, int pluginIndex)
{
    for (int feature = 0; feature < features.size(); ++feature)
    {
		VampOutputDescriptor &currentOutput = outputs[feature];
		VampFeatureList &currentFeatureList = features[feature];

        String featureName = "Vamp " + currentOutput.name;

        for (int i = 0; i < currentFeatureList.size(); ++i)
        {
            VampFeature &currentFeature = currentFeatureList [i];

            nextVampFeatureTimeStamp = timeStamp;

			AudioFeature tempFeature;

            tempFeature.channelNumber = 0;

            bool ignoreFeature = getVampPluginFeatureTimeAndDuration (tempFeature, currentOutput, currentFeature, timeStamp);

            for (int value = 0; value < currentFeature.values.size(); ++value)
            {
                tempFeature.values.add (currentFeature.values [value]);
            }

            if (! ignoreFeature)
            {
                if (! featureLists.count (featureName))
                {
                    const AnalysisConfiguration *config = getVampPluginAnalysisConfiguration (pluginIndex);

                    featureLists [featureName].frameSize = config->frameSize;
                    featureLists [featureName].stepSize = config->stepSize;
                }

                featureLists [featureName].features.add (tempFeature);
            }
        }
    }
}

bool SAFEFeatureExtractor::getVampPluginFeatureTimeAndDuration (AudioFeature &newFeature, 
                                                                const VampOutputDescriptor &output,
                                                                const VampFeature &feature,
                                                                int timeStamp)
{
    switch (output.sampleType)
    {
        case VampOutputDescriptor::OneSamplePerStep:
            newFeature.timeStamp = timeStamp;
            newFeature.duration = 0;
            break;

        case VampOutputDescriptor::FixedSampleRate:
            if (output.sampleRate == 0)
            {
                return true;
            }

            if (feature.hasTimestamp)
            {
                newFeature.timeStamp = feature.timestamp.msec();
            }
            else
            {
                int timeStampIncrement = 1000 / output.sampleRate;

                newFeature.timeStamp = nextVampFeatureTimeStamp;
                nextVampFeatureTimeStamp += timeStampIncrement;
            }

            if (feature.hasDuration)
            {
                newFeature.duration = feature.duration.msec();
            }
            else
            {
                newFeature.duration = 0;
            }

            break;

        case VampOutputDescriptor::VariableSampleRate:
            if (! feature.hasTimestamp)
            {
                return true;
            }

            newFeature.timeStamp = feature.timestamp.msec();

            if (feature.hasDuration)
            {
                newFeature.duration = feature.duration.msec();
            }
            else
            {
                int minimalDuration = 0;

                if (output.sampleRate != 0)
                {
                    minimalDuration = 1000 / output.sampleRate;
                }

                newFeature.duration = minimalDuration;
            }
            
            break;
    }

    return false;
}
