/*
  ==============================================================================

    Saul 7.26

  ==============================================================================
*/

//==============================================================================
/**
*/
class HybridCurtisVCF
{
public:

    //constructor
    HybridCurtisVCF() noexcept;

    //destructor
    ~HybridCurtisVCF() noexcept;

    //==============================================================================
    //does pretty much what it says - deactivates thread safety
    void makeInactive() noexcept;

    //sets all coefficients and intermediate variables
    void makeCurtis (double sampleRate, double frequency, double resonance) noexcept;
    //==============================================================================
    //resets pipeline via coefficient reset
    void reset() noexcept;

    //processes all samples fed via buffer
    void processSamples (float* samples, int numSamples) noexcept;
    
private:
    
    //core variables
    float cutoff;   //in Hz
    float fs;       //format eg - 44100
    float res;      // 0 - 1
    
    //intermediates
    float f;
    float k;
    float p;
    float scale;
    float r;
    
    //coeffs
    float c1;
    float c2;
    float c3;
    float c4;
    
    float oldC1;
    float oldC2;
    float oldC3;
    
    float c0;
    float oldC0;

protected:
    //==============================================================================
    
    //thread safety
    SpinLock processLock;
    bool active = false;

};
