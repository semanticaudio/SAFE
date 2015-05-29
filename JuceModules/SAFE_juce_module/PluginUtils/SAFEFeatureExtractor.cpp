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
      libXtractSpectrumNeeded (false),
      libXtractPeakSpectrumNeeded (false),
      libXtractHarmonicSpectrumNeeded (false),
      calculateLibXtractBarkCoefficients (false),
      libXtractMelFiltersInitialised (false),
      calculateLibXtractMFCCs (false)
{
    // initialise lib xtract arrays
    for (int i = 0; i < NumLibXtractFeatures; ++i)
    {
        calculateLibXtractFeature [i] = false;
        libXtractFeatureCalculated [i] = false;
        libXtractFeatureValues [i] = 0.0;
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
    libXtractFeatureValues.resize (numChannels);

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

    for (int i = 0; i < numChannels; ++i)
    {
        libXtractFeatureValues.getReference (i).resize (NumLibXtractFeatures);
        libXtractBarkCoefficients.getReference (i).resize (numLibXtractBarkBands);
        libXtractMFCCs.getReference (i).resize (numLibXtractMFCCs);
    }

    xtract_init_wavelet_f0_state();

    initialised = true;
}

void SAFEFeatureExtractor::addLibXtractFeature (LibXtractFeature feature)
{
    calculateLibXtractFeature [feature] = true;

    if (feature >= XtractSpectralCentroid)
    {
        libXtractSpectrumNeeded = true;
    }

    if (feature >= XtractPeakSpectralCentroid)
    {
        libXtractPeakSpectrumNeeded = true;
    }

    if (feature >= XtractHarmonicSpectralCentroid)
    {
        libXtractHarmonicSpectrumNeeded = true;
    }
}

void SAFEFeatureExtractor::addLibXtractFeatureGroup (LibXtractFeatureGroup featureGroup)
{
    switch (featureGroup)
    {
        case XtractTemporalFeatures:

            for (int i = XtractTemporalMean; i <= XtractZeroCrossingRate; ++i)
            {
                calculateLibXtractFeature [i] = true;
            }

            break;

        case XtractSpectralFeatures:

            for (int i = XtractFundamentalFrequency; i <= XtractSpectralSlope; ++i)
            {
                calculateLibXtractFeature [i] = true;
            }

            libXtractSpectrumNeeded = true;

            break;

        case XtractBarkCoefficients:
            calculateLibXtractBarkCoefficients = true;
            libXtractSpectrumNeeded = true;
            break;

        case XtractMFCCs:
            calculateLibXtractMFCCs = true;
            libXtractSpectrumNeeded = true;
            break;

        case XtractPeakSpectralFeatures:

            for (int i = XtractPeakSpectralCentroid; i <= XtractPeakTristimulus3; ++i)
            {
                calculateLibXtractFeature [i] = true;
            }

            libXtractSpectrumNeeded = true;
            libXtractPeakSpectrumNeeded = true;

            break;

        case XtractHarmonicSpectralFeatures:

            for (int i = XtractInharmonicity; i <= XtractHarmonicParityRatio; ++i)
            {
                calculateLibXtractFeature [i] = true;
            }

            libXtractSpectrumNeeded = true;
            libXtractPeakSpectrumNeeded = true;
            libXtractHarmonicSpectrumNeeded = true;

            break;

        case XtractAll
            
            for (int i = 0; i < NumLibXtractFeatures; ++i)
            {
                calculateLibXtractFeature [i] = true;
            }

            libXtractSpectrumNeeded = true;
            libXtractPeakSpectrumNeeded = true;
            libXtractHarmonicSpectrumNeeded = true;

            break;
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

        libXtractMelFilteresInitialised = false;
    }
}
