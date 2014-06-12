//==========================================================================
//      Constructor and Destructor
//==========================================================================
LevelMeterTimer::LevelMeterTimer (int numMeters)
    : riseTime (20),
      decayTime (100),
      numRiseSteps (2),
      numDecaySteps (10),
      numSteps (10)
{
    if (numMeters < 1)
    {
        numMeters = 1;
    }

    for (int n = 0; n < numMeters; ++n)
    {
        targetLevels.add (-100);
        currentLevels.add (-100);
        previousLevels.add (-100);
        stepSizes.add (-100);
        timerCounters.add (-100);
    }
}

LevelMeterTimer::~LevelMeterTimer()
{
}

//==========================================================================
//      Get and Set Level
//==========================================================================
void LevelMeterTimer::setMeterLevel (int index, double newLevel)
{
    previousLevels.set (index, currentLevels [index]);
    targetLevels.set (index, newLevel);

    double levelRange = targetLevels [index] - previousLevels [index];
    
    stepSizes.set (index, levelRange / numDecaySteps);

    if (levelRange <= 0)
    {
        numSteps = numDecaySteps;
    }
    else
    {
        numSteps = numRiseSteps;
    }  

    timerCounters.set (index, 0);

    startTimer (index, 10);
}

double LevelMeterTimer::getMeterLevel (int index)
{
    return currentLevels [index];
}

double LevelMeterTimer::getMeterTargetLevel (int index)
{
    return targetLevels [index];
}

//==========================================================================
//      Set Rise and Decay Times
//==========================================================================
void LevelMeterTimer::setMeterRiseTime (int riseInMilliseconds)
{
    riseTime = abs (riseInMilliseconds);

    numRiseSteps = (int) floor (riseTime / 10);

    if (numRiseSteps < 1)
    {
        numRiseSteps = 1;
    }
}

void LevelMeterTimer::setMeterDecayTime (int decayInMilliseconds)
{
    decayTime = abs (decayInMilliseconds);

    numDecaySteps = (int) floor (decayTime / 10);

    if (numDecaySteps < 1)
    {
        numDecaySteps = 1;
    }
}

//==========================================================================
//      Timer Callback
//==========================================================================
void LevelMeterTimer::timerCallback (int index)
{
    if (timerCounters [index] < numSteps)
    {
        currentLevels.set (index, currentLevels [index] + stepSizes [index]);
        timerCounters.set (index, timerCounters [index] + 1);
    }
    else
    {
        currentLevels.set (index, targetLevels [index]);
        stopTimer (index);
    }

    meterLevelChanged();
}
