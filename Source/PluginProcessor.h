/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthSound.h"
#include "SynthVoice.h"
#include <cmath>
#include <vector>
//==============================================================================
/**
*/
typedef std::vector<int> Vector;
class VocoderAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    VocoderAudioProcessor();
    ~VocoderAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    AudioProcessorValueTreeState::ParameterLayout createParameters();
    AudioProcessorValueTreeState parametros;
    float gnoise, gsaw, gpulse, gmodulator;
    int nbands;
private:
    void generadorruido(AudioBuffer<float>&buff, float ganancia);
    void procesamientovocoder();
    void calculobandas();
    Synthesiser vocodercarrier;
    SynthVoice* voice;
    AudioBuffer <float> buffercarrier;
    AudioBuffer <float> buffernoise;
    AudioBuffer <float> buffermodulatoranterior;
    AudioBuffer <float> buffercarrieranterior;
    AudioBuffer <float> buffervocodersolape;
    int fftsize;
    int numbands;
    HeapBlock<dsp::Complex<float>> fftmodulator;
    HeapBlock<dsp::Complex<float>> fftcarrier;
    HeapBlock<dsp::Complex<float>> timemodulator;
    HeapBlock<dsp::Complex<float>> timecarrier;
    HeapBlock<dsp::Complex<float>> timevocoder;
    HeapBlock<dsp::Complex<float>> fftvocoder;
    HeapBlock<float> window;
    std::unique_ptr<dsp::FFT> fft;
    Vector bandas;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VocoderAudioProcessor)
};
