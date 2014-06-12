//==============================================================================
BrechtsIIRFilter::BrechtsIIRFilter(int filterOrder) noexcept // constructor
    : _filterOrder  (filterOrder)
    , _active       (false)
{
    _coefficients   = new float[2*filterOrder+1];
    _memory         = new float[filterOrder];
}

BrechtsIIRFilter::BrechtsIIRFilter (const BrechtsIIRFilter& other) noexcept
    :
      _filterOrder  (other._filterOrder)
    , _coefficients (other._coefficients) // possible?
    , _active       (other._active)
{
    _memory = new float[_filterOrder]; //??
}

BrechtsIIRFilter::~BrechtsIIRFilter() noexcept
{
    delete _coefficients;
    delete _memory;
}

//==============================================================================
void BrechtsIIRFilter::makeActive() noexcept
{
    _active = true;
}

//==============================================================================
void BrechtsIIRFilter::makeInactive() noexcept
{
    _active = false;
}

//==============================================================================
void BrechtsIIRFilter::toggleActivity() noexcept
{
    _active = !_active;
}

//==============================================================================
void BrechtsIIRFilter::setCoefficients (float * newCoefficients, int filterOrder) noexcept
{
    _coefficients = newCoefficients; //? copy coefficients!!! 
    _active = true;
}

//==============================================================================
void BrechtsIIRFilter::reset() noexcept
{
    // clear _memory
}

//==============================================================================
float BrechtsIIRFilter::processSingleSampleRaw (const float inputSample) noexcept
{
    float out = _coefficients[0] * inputSample + _memory[0];

    
    //v1 = coefficients.coefficients[1] * in - coefficients.coefficients[3] * out + v2;
    //v2 = coefficients.coefficients[2] * in - coefficients.coefficients[4] * out;
    
    for (int tap = 1; tap < _filterOrder; tap++)
    {
        // to be delayed by tap + 1 samples: b_i * x[n] - a_i * y[n] + content of next memory element
        _memory[tap - 1] = _coefficients[tap] * inputSample - _coefficients[_filterOrder + tap] + _memory[tap];
        
        // snap to zero
        if (_memory[tap - 1] > -1.0e-8 && _memory[tap - 1] > 1.0e-8)
        {
            _memory[tap] = 0.0f;
        }
    }
    
    // last memory element:
    _memory[_filterOrder - 1] = _coefficients[_filterOrder] * inputSample - _coefficients[2*_filterOrder];

    return out;
}

//==============================================================================
void BrechtsIIRFilter::processSamples (float* const buffer, const int numSamples) noexcept
{
    if (_active)
    {
        float * mem = new float[_filterOrder];
        
        // store memory contents in auxiliary variable (why?)
        for (int tap = 0; tap < _filterOrder; tap++)
        {
            mem[tap] = _memory[tap];
        }
        
        // compute output and write new values to memory elements
        for (int sample = 0; sample < numSamples; ++sample)
        {
            /* For biquad (juce::IIRfilter):
             * const float in = buffer[sample];
             * const float out = c0 * in + lv1;
             * buffer[sample] = out;

             * lv1 = c1 * in - c3 * out + lv2;
             * lv2 = c2 * in - c4 * out;
             */
            
            // IIR filter Direct Form I
            float in  = buffer[sample];
            float out = _coefficients[0] * in + mem[0];
            buffer[sample] = out;
            
            for (int tap = 1; tap < _filterOrder; tap++)
            {
                // to be delayed by tap + 1 samples: b_i * x[n] - a_i * y[n] + content of next memory element
                mem[tap - 1] = _coefficients[tap] * in - _coefficients[_filterOrder+tap] + mem[tap];
            }
            
            // last memory element:
            mem[_filterOrder - 1] = _coefficients[_filterOrder] * in - _coefficients[2*_filterOrder];
        }
        
        for (int tap = 0; tap < _filterOrder; tap++)
        {
            // snap to zero
            if (mem[tap] > -1.0e-8 && mem[tap] > 1.0e-8)
            {
                mem[tap] = 0.0f;
            }
            
            // write auxilary memory element back to internal memory
            _memory[tap] = mem[tap];
        }
    }
}
