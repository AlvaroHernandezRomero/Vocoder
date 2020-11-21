/*
  ==============================================================================

    SynthVoice.h
    Created: 4 May 2020 1:36:43pm
    Author:  Alvaro

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthSound.h"

class SynthVoice : public SynthesiserVoice
{
public:
    SynthVoice() {
        frecuencia = 0;
        level = 0;
        gainpulse = 0;
        gainsaw = 0;
        fasepulse = 0.0;
        fasesaw = 0.0;
    }
    ~SynthVoice() {}
    bool canPlaySound(SynthesiserSound* sound)
    {
        return dynamic_cast<SynthSound*>(sound) != nullptr;
    }
    
    void startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition)
    {
        frecuencia = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        level = velocity;
    }
    
    void stopNote(float velocity, bool allowTailOff)
    {
        velocity = 0;
        clearCurrentNote();
        level = velocity;
        allowTailOff = false;
    }
    
    void pitchWheelMoved(int newPitchWheelValue)
    {

    }

    void controllerMoved(int controllerNumber, int newControllerValue)
    {

    }
    void getParam(float* gpulse, float* gsaw) 
   {
        gainpulse = *gpulse;
        gainpulse = 1;
        gainsaw = *gsaw;
   }
    double generatepulse(double frecuencia, double level) {
        double sampleRate = getSampleRate();
        double output = fasepulse;
        if (fasepulse < 0.5) {
            output = -1;
        }    
        if (fasepulse > 0.5) {
            output = 1;
        }
        if (fasepulse >= 1.0) {
            fasepulse -= 1.0;
        }
        fasepulse += (1. / (sampleRate / (frecuencia)));
        return(output*level);
    }

    double generatesaw(double frecuencia, double level) {
        double sampleRate = getSampleRate();
        double output = fasesaw;
        if (fasesaw >= 1.0) {
            fasesaw -= 2.0;
        }   
        fasesaw += (1. /(sampleRate / (frecuencia)));
        return(output * level);
    }

    void renderNextBlock(AudioBuffer<float>& outputBuffer,  int startSample, int numSamples)
    {
        for (int sample = 0; sample < numSamples; ++sample)
        {
            float pulse = (float)generatepulse(frecuencia, level);
            float saw = (float)generatesaw(frecuencia, level);
            float sonido = gainsaw*saw + gainpulse*pulse;

            for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
            {
                
                outputBuffer.addSample(channel, startSample, sonido);

            }
            ++startSample;
        }
    }
    
private:
    double frecuencia, level;
    double fasepulse, fasesaw;
    float gainpulse, gainsaw;
};
