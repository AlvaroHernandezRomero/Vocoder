/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SynthVoice.h"

//==============================================================================
VocoderAudioProcessorEditor::VocoderAudioProcessorEditor (VocoderAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    setSize(1100, 350);
    addAndMakeVisible(bandsslider);
    bandsslider.setRange(2, 128, 1);
    bandsslider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    bandsslider.setTextBoxStyle(Slider::TextBoxAbove, false, 80, 20);
    bandsslider.setColour(Slider::textBoxTextColourId, Colours::black);
    bandsslider.setColour(Slider::textBoxHighlightColourId, Colour(0x00ffffff));
    bandsslider.setColour(Slider::textBoxOutlineColourId, Colour(0x00ffffff));
    bandsslider.onValueChange = [this] {bandsslider.getValue(); };
    processor.nbands = (int) bandsslider.getValue();
    bandsslider.addListener(this);
    sld_nbands = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.parametros, "bandsslider", bandsslider);

    addAndMakeVisible(&carrierGroup);
    carrierGroup.setText("CARRIER");
    carrierGroup.setTextLabelPosition(Justification::centred);
    carrierGroup.setColour(GroupComponent::outlineColourId, Colour(0xff8b8b8b));
    carrierGroup.setColour(GroupComponent::textColourId, Colours::black);

    

    addAndMakeVisible(&noiseslider);
    noiseslider.setRange(0, 1, 0.01);
    noiseslider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    noiseslider.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    noiseslider.setColour(Slider::textBoxTextColourId, Colours::black);
    noiseslider.setColour(Slider::textBoxBackgroundColourId, Colours::white);
    noiseslider.onValueChange = [this] {noiseslider.getValue(); };
    processor.gnoise = (float)noiseslider.getValue();
    noiseslider.addListener(this);
    sld_noise = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.parametros, "noiseslider", noiseslider);
    

    addAndMakeVisible(&sawslider);
    sawslider.setRange(0, 1, 0.01);
    sawslider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    sawslider.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    sawslider.setColour(Slider::textBoxTextColourId, Colours::black);
    sawslider.setColour(Slider::textBoxBackgroundColourId, Colours::white);
    sawslider.onValueChange = [this] {sawslider.getValue(); };
    processor.gsaw = (float) sawslider.getValue();
    sawslider.addListener(this);
    sld_saw = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.parametros, "sawslider", sawslider);
    

    addAndMakeVisible(&pulseslider);
    pulseslider.setRange(0, 1, 0.01);
    pulseslider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    pulseslider.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    pulseslider.setColour(Slider::textBoxTextColourId, Colours::black);
    pulseslider.setColour(Slider::textBoxBackgroundColourId, Colours::white);
    pulseslider.onValueChange = [this] {pulseslider.getValue(); };
    processor.gpulse = (float) pulseslider.getValue();
    pulseslider.addListener(this);
    sld_pulse = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.parametros, "pulseslider", pulseslider);
    

    addAndMakeVisible(&modulatorslider);
    modulatorslider.setRange(0, 1, 0.01);
    modulatorslider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    modulatorslider.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    modulatorslider.setColour(Slider::textBoxTextColourId, Colours::black);
    modulatorslider.onValueChange = [this] {modulatorslider.getValue(); };
    processor.gmodulator = (float) modulatorslider.getValue();
    modulatorslider.addListener(this);
    sld_modulator = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.parametros, "modulatorslider", modulatorslider);

    addAndMakeVisible(&noiselabel);
    noiselabel.setFont(Font("Arial", 12.00f, Font::plain).withTypefaceStyle("Bold"));
    noiselabel.setJustificationType(Justification::centredTop);
    noiselabel.setEditable(false, false, false);
    noiselabel.setColour(Label::textColourId, Colours::black);
    noiselabel.setColour(Label::outlineColourId, Colour(0x00000000));
    noiselabel.setColour(TextEditor::textColourId, Colours::black);
    noiselabel.setColour(TextEditor::backgroundColourId, Colour(0x00000000));
    noiselabel.setColour(TextEditor::highlightColourId, Colour(0x00e0ec65));
    noiselabel.setText("NOISE", dontSendNotification);

    addAndMakeVisible(&sawlabel);
    sawlabel.setFont(Font("Arial", 12.00f, Font::plain).withTypefaceStyle("Bold"));
    sawlabel.setJustificationType(Justification::centredTop);
    sawlabel.setEditable(false, false, false);
    sawlabel.setColour(Label::textColourId, Colours::black);
    sawlabel.setColour(TextEditor::textColourId, Colours::black);
    sawlabel.setColour(TextEditor::backgroundColourId, Colour(0x00000000));
    sawlabel.setText("SAW", dontSendNotification);
    

    addAndMakeVisible(&pulselabel);
    pulselabel.setFont(Font("Arial", 12.00f, Font::plain).withTypefaceStyle("Bold"));
    pulselabel.setJustificationType(Justification::centredTop);
    pulselabel.setEditable(false, false, false);
    pulselabel.setColour(Label::textColourId, Colours::black);
    pulselabel.setColour(TextEditor::textColourId, Colours::black);
    pulselabel.setColour(TextEditor::backgroundColourId, Colour(0x00000000));
    pulselabel.setText("PULSE", dontSendNotification);
    

    addAndMakeVisible(&modulatorlabel);
    modulatorlabel.setFont(Font("Arial", 12.00f, Font::plain).withTypefaceStyle("Bold"));
    modulatorlabel.setJustificationType(Justification::centredTop);
    modulatorlabel.setEditable(false, false, false);
    modulatorlabel.setColour(Label::textColourId, Colours::black);
    modulatorlabel.setColour(TextEditor::textColourId, Colours::black);
    modulatorlabel.setColour(TextEditor::backgroundColourId, Colour(0x00000000));
    modulatorlabel.setText("MODULATOR", dontSendNotification);



    addAndMakeVisible(&bandslabel);
    bandslabel.setFont(Font(15.00f, Font::plain).withTypefaceStyle("Bold"));
    bandslabel.setJustificationType(Justification::centredTop);
    bandslabel.setEditable(false, false, false);
    bandslabel.setColour(Label::textColourId, Colours::black);
    bandslabel.setColour(TextEditor::textColourId, Colours::black);
    bandslabel.setColour(TextEditor::backgroundColourId, Colour(0x00000000));
    bandslabel.setText("NºBANDS", dontSendNotification);
    startTimer(50);

}

VocoderAudioProcessorEditor::~VocoderAudioProcessorEditor()
{
}

//==============================================================================
void VocoderAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(Colours::white);
    g.setColour(Colours::black);//color de las letras
    g.setFont(30.0f); // tamaño de las letras 
    g.drawFittedText("VOCODER", getLocalBounds(), Justification::centredTop, 1);

    {
        float x = 856.0f, y = 120.0f, width = 120.0f, height = 64.0f;
        Colour fillColour = Colour(0xff00f7ff);
        Colour strokeColour = Colour(0xff8b8b8b);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(fillColour);
        g.fillEllipse(x, y, width, height);
        g.setColour(strokeColour);
        g.drawEllipse(x, y, width, height, 2.000f);
    }

    {
        float x = 92.0f, y = 90.0f, width = 257.0f, height = 187.0f;
        Colour fillColour = Colour(0xff00f7ff);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(fillColour);
        g.fillRoundedRectangle(x, y, width, height, 10.000f);
    }
}

void VocoderAudioProcessorEditor::resized()
{
    bandsslider.setBounds(848, 140, 136, 96);
    carrierGroup.setBounds(88, 80, 264, 200);
    noiseslider.setBounds(96, 104, 40, 168);
    sawslider.setBounds(202, 104, 40, 168);
    pulseslider.setBounds(304, 104, 40, 168);
    modulatorslider.setBounds(464, 104, 40, 168);
    noiselabel.setBounds(96, 96, 40, 16);
    sawlabel.setBounds(200, 96, 40, 16);
    pulselabel.setBounds(304, 96, 40, 16);
    modulatorlabel.setBounds(448, 96, 80, 16);
    bandslabel.setBounds(840, 96, 150, 14);
}

void VocoderAudioProcessorEditor::timerCallback()
{
    bandsslider.setValue(processor.nbands, dontSendNotification);
    noiseslider.setValue(processor.gnoise, dontSendNotification);
    sawslider.setValue(processor.gsaw, dontSendNotification);
    pulseslider.setValue(processor.gpulse, dontSendNotification);
    modulatorslider.setValue(processor.gmodulator, dontSendNotification);
}
void VocoderAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    if (slider == &bandsslider) {
        bandsslider.setValue(bandsslider.getValue(), dontSendNotification);
        processor.nbands = (int)bandsslider.getValue();
    }
    if (slider == &noiseslider) {
        noiseslider.setValue(noiseslider.getValue(), dontSendNotification);
        processor.gnoise = (float)noiseslider.getValue();
    }
    if (slider == &sawslider) {
        sawslider.setValue(sawslider.getValue(), dontSendNotification);
        processor.gsaw = (float)sawslider.getValue();
    }
    if (slider == &pulseslider) {
        pulseslider.setValue(pulseslider.getValue(), dontSendNotification);
        processor.gpulse = (float)pulseslider.getValue();
    }
    if (slider == &modulatorslider) {
        modulatorslider.setValue(modulatorslider.getValue(), dontSendNotification);
        processor.gmodulator = (float)modulatorslider.getValue();
    }
}