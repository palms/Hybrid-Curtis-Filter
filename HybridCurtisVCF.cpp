

#include "HybridCurtisVCF.h"

//==============================================================================
HybridCurtisVCF::HybridCurtisVCF() noexcept
{
}

HybridCurtisVCF::~HybridCurtisVCF() noexcept
{
}

//==============================================================================
void HybridCurtisVCF::makeInactive() noexcept
{
    const SpinLock::ScopedLockType sl (processLock);
    active = false;
}
    
//==============================================================================
void HybridCurtisVCF::makeCurtis (double sampleRate,
                                         double frequency,
                                         double resonance) noexcept
{
    
    //NOTE! check these for safety
    /*
    jassert (sampleRate > 0.0);
    jassert (frequency > 0.0 && frequency <= sampleRate * 0.5);
    jassert (resonance => 0.0 && resonance <= 1.0);
     */
    
    //core variables
    cutoff = (float)frequency;
    fs = (float)sampleRate;
    res = (float)resonance;
    
    //reset all coeffs
    c1 = c2 = c3 = c4 = oldC0 = oldC1 = oldC2 = oldC3 = 0.f;

    //intermediate variables
    f = 2.f * (float)cutoff / (float)fs;    //should range 0 - 1
    k = 3.6f * f - 1.6f * f * f - 1.f;      //empirical tuning
    p = (k + 1.f) * 0.5f;
    scale = exp((1.f - p) * 1.386249f);
    r = (float)res * scale;

    //thread-safety
    const SpinLock::ScopedLockType sl (processLock);
    active = true;
}

//==============================================================================
void HybridCurtisVCF::reset() noexcept
{
    //thread-safety and reset all coeffs
    const SpinLock::ScopedLockType sl (processLock);
    c1 = c2 = c3 = c4 = oldC0 = oldC1 = oldC2 = oldC3 = 0.f;
}

void HybridCurtisVCF::processSamples (float* const samples, const int numSamples) noexcept
{
    const SpinLock::ScopedLockType sl (processLock);

    if (active)
    {
        float sample;
        
        for (int i = 0; i < numSamples; i++)
        {
            //grab single sample for safe keeping
            sample = samples[i];
            
             //--Inverted feed back for corner peaking
            c0 = sample - r * c4;
            
            //Four cascaded onepole filters (bilinear transform)
            c1 = c0 * p + oldC0 * p - k * c1;
            c2 = c1 * p + oldC1 * p - k * c2;
            c3 = c2 * p + oldC2 * p - k * c3;
            c4 = c3 * p + oldC3 * p - k * c4;
            
            //save old coeffs
            oldC0 = c0;
            oldC1 = c1;
            oldC2 = c2;
            oldC3 = c3;
            
            //Clipper band limited sigmoid
            c4 = c4 - pow(c4, 3.f) / 6.f;
            
            //replace sample
            samples[i] = c4;
        }
    }
}

