//==========================================================================
//      Constructor and Destructor
//==========================================================================
SAFEFeatureExtractor::SAFEFeatureExtractor()
    : initialised (false),
      numChannels (0),
      frameSize (0),
      stepSize (0),
      fft (nullptr)
{
}

SAFEFeatureExtractor::~SAFEFeatureExtractor()
{
}

//==========================================================================
//      Setup
//==========================================================================
void SAFEFeatureExtractor::initialise (int numChannelsInit, int frameOrderInit, int stepSizeInit)
{
    // save the number of channels
    numChannels = numChannelsInit;

    // calculate the frame size
    frameSize = 1 << frameOrderInit;

    // get the FFT object creating a new one if needs be
    if (! fftCache.count (frameSize))
    {
        fftCache.insert (std::pair <int, FFT> (frameSize, FFT (frameOrderInit, false)));
    }

    fft = &(fftCache [frameSize]);

    // make sure the step size is sensible
    if (stepSizeInit > frameSize || stepSizeInit < 1)
    {
        stepSize = frameSize;
    }
    else
    {
        stepSize = stepSizeInit;
    }

    initialised = true;
}
