//=============================================================================
//  Constructor and Destructor
//=============================================================================
FundamentalTracker::FundamentalTracker()
{
    reset();

    gamma = 0.002;
}

FundamentalTracker::~FundamentalTracker()
{
}

//=============================================================================
//  Setup Functions
//=============================================================================
void FundamentalTracker::setSampleRate (double sampleRate)
{
    fs = sampleRate;
}

void FundamentalTracker::reset()
{
    for (int i = 0; i < 3; ++i)
    {
        signalBuffer [i] = 0;
    }

    for (int i = 0; i < 2; ++i)
    {
        f0Buffer [i] = 0;
    }

    signalTap = 0;
    f0Tap = 0;
}

//=============================================================================
//  Operation
//=============================================================================
void FundamentalTracker::addSample (float newSample)
{
    signalBuffer [signalTap] = newSample;

    int prevSampleIndex = (signalTap + 2) % 3;
    int prevPrevSampleIndex = (signalTap + 1) % 3;

    int prevF0Index = 1 - f0Tap;

    f0Buffer [f0Tap] = f0Buffer [prevF0Index] + signalBuffer [prevSampleIndex] * gamma * 
                       (signalBuffer [signalTap] + signalBuffer [prevPrevSampleIndex]
                       - 2 * signalBuffer [prevSampleIndex] * f0Buffer [prevF0Index]);

    signalTap = prevPrevSampleIndex;

    f0Tap = prevF0Index;
}

float FundamentalTracker::getFundamental()
{
    float rawF0 = f0Buffer [1 - f0Tap];
    return fs * acos (rawF0) / (2 * float_Pi);
}
