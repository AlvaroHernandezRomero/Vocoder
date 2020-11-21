/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "SynthVoice.h"

//==============================================================================
/**
*/
class VocoderAudioProcessorEditor  : public AudioProcessorEditor , private Slider::Listener, public Timer
{
public:
    VocoderAudioProcessorEditor (VocoderAudioProcessor&);
    ~VocoderAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void timerCallback();
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    
    void sliderValueChanged(Slider* slider) override;
    VocoderAudioProcessor& processor;
    Slider bandsslider, noiseslider, sawslider, pulseslider, modulatorslider;
    GroupComponent carrierGroup;
    Label noiselabel, sawlabel, pulselabel, modulatorlabel, bandslabel;

    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> sld_nbands;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> sld_noise;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> sld_saw;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> sld_pulse;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> sld_modulator;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VocoderAudioProcessorEditor)
};
