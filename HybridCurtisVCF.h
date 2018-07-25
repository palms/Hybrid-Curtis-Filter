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

    HybridCurtisVCF() noexcept;

    ~HybridCurtisVCF() noexcept;

    //==============================================================================
    void makeInactive() noexcept;

    void makeCurtis (double sampleRate, double frequency, double resonance) noexcept;
    //==============================================================================
    void reset() noexcept;

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
    SpinLock processLock;
    bool active = false;

};
