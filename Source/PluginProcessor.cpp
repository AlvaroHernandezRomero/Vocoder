/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <math.h>
#include <cmath>
#include <vector>

//==============================================================================
VocoderAudioProcessor::VocoderAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ), parametros(*this,nullptr,"EstadoSliders",createParameters())
#endif
{
    parametros.state = ValueTree("EstadoSliders");
    gnoise = 0;
    gsaw = 0;
    gpulse = 0;
    gmodulator = 0;
    nbands = 32;
    numbands = 32;
    vocodercarrier.clearVoices();
    for (int i = 0; i < 10; i++)
    {
        vocodercarrier.addVoice(new SynthVoice());
    }
    vocodercarrier.clearSounds();
    vocodercarrier.addSound(new SynthSound());
}

VocoderAudioProcessor::~VocoderAudioProcessor()
{
}

//==============================================================================
const String VocoderAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool VocoderAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool VocoderAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool VocoderAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double VocoderAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int VocoderAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int VocoderAudioProcessor::getCurrentProgram()
{
    return 0;
}

void VocoderAudioProcessor::setCurrentProgram (int index)
{
}

const String VocoderAudioProcessor::getProgramName (int index)
{
    return {};
}

void VocoderAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void VocoderAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    vocodercarrier.setCurrentPlaybackSampleRate(sampleRate);

    buffercarrier.clear();
    buffernoise.clear();

    buffermodulatoranterior.setSize(1, samplesPerBlock, false, false, false);
    buffermodulatoranterior.clear();

    buffercarrieranterior.setSize(1,samplesPerBlock, false, false, false);
    buffercarrieranterior.clear();

    buffervocodersolape.setSize(1, samplesPerBlock, false, false, false);
    buffervocodersolape.clear();
    
    fftsize = 4 * samplesPerBlock;

    /*window.realloc(fftsize/4,sizeof(float));
    window.clear(fftsize/4);*/
    window.allocate(fftsize/2 , true);
    float tam = (float)fftsize/2 - 1.0f;
    tam = (float)fftsize / 2 - 1.0f;
    for (int sample = 0; sample < fftsize / 2 ; ++sample) {
        //window[sample] = (0.5f * (1.0f - cos(2.0f * M_PI * (float)sample / tam)));
        window[sample] = 1.0f;
    }
    fftmodulator.realloc(fftsize);
    fftmodulator.clear(fftsize);
    fftcarrier.realloc(fftsize);
    fftcarrier.clear(fftsize);
    timemodulator.realloc(fftsize);
    timemodulator.clear(fftsize);
    timecarrier.realloc(fftsize);
    timecarrier.clear(fftsize);
    timevocoder.realloc(fftsize);
    timevocoder.clear(fftsize);
    fftvocoder.realloc(fftsize);
    fftvocoder.clear(fftsize);
    fft = std::make_unique<dsp::FFT>(log2(fftsize));
    
    bandas.resize(nbands);
    bandas.empty();
}

void VocoderAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool VocoderAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif
void VocoderAudioProcessor::generadorruido(AudioBuffer<float>& buff, float ganancia)
{
    auto* noisedata= buff.getWritePointer(0);
    Random ruid;
    for (int i = 0; i < buff.getNumSamples(); i++) {
        //noisedata[i] = ganancia *  (-1 + ((float)((rand() % 200) + 1)/100));
        noisedata[i] = ganancia * (ruid.nextFloat());
    }

}
void VocoderAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    buffercarrier.clear();
    buffercarrier.setSize(1, buffer.getNumSamples(), false, false, false);
    for (int i = 0; i < vocodercarrier.getNumVoices(); i++) {
        if (voice = dynamic_cast<SynthVoice*>(vocodercarrier.getVoice(i))) {
            voice->getParam(&gpulse,&gsaw);
        }
    }
    vocodercarrier.renderNextBlock(buffercarrier, midiMessages, 0, buffer.getNumSamples()); //se genera la señal de carrier de onda cuadrada y diente de sierra
    buffernoise.setSize(1, buffer.getNumSamples(), false, false, false);
    buffernoise.clear();
    generadorruido(buffernoise, gnoise);//se genera el ruido blanco con ganancia gnoise
    
    if (midiMessages.isEmpty() == true) {
        midiMessages.clear();
    }
    auto* carrier = buffercarrier.getWritePointer(0); //La variable carrier es el buffer con las muestras la señal de carrier de onda cuadrada y diente de sierra
    auto* noise = buffernoise.getWritePointer(0); //La variable noise es el buffer con las muestras la señal de ruido blanco
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    timemodulator.clear(fftsize); //buffer de procesado en tiempo de la señal moduladora
    timecarrier.clear(fftsize); //buffer de procesado en frecuencia de la señal portadora
    auto* modulatoranterior = buffermodulatoranterior.getWritePointer(0); //buffer para guardar señal moduladora actual la cuál es procesada en el bloque siguiente 
    auto* carrieranterior = buffercarrieranterior.getWritePointer(0); //buffer para guardar señal portadora actual la cuál es procesada en el bloque siguiente 


    for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
        timemodulator[sample].imag(0); //se asigna la parte imaginaria(0) en el primer cuarto del buffer
        timemodulator[sample + buffer.getNumSamples()].imag(0); //se asigna la parte imaginaria(0) en el segundo cuarto del buffer
        timemodulator[sample].real(window[sample] * gmodulator * modulatoranterior[sample]); //se asigna al primer cuarto del buffer real el buffer 
        //de la señal moduladora del bloque anterior
        timemodulator[sample + buffer.getNumSamples()].real(window[sample + buffer.getNumSamples()]*gmodulator *buffer.getSample(0, sample));
        //se asigna al segundo cuarto del buffer real el buffer actual de la señal moduladora
        modulatoranterior[sample] = buffer.getSample(0, sample); //Se actualiza con el buffer de señal moduladora(señal de entrada) de este bloque

        timecarrier[sample].imag(0);//se asigna la parte imaginaria(0) en el primer cuarto del buffer
        timecarrier[sample + buffer.getNumSamples()].imag(0);//se asigna la parte imaginaria(0) en el segundo cuarto del buffer
        timecarrier[sample].real(carrieranterior[sample]* window[sample]); //se asigna al primer cuarto del buffer real el buffer
        //de la señal portadora del bloque anterior
        timecarrier[sample + buffer.getNumSamples()].real(window[sample + buffer.getNumSamples()]*(float)(carrier[sample] + noise[sample]));
        //se asigna al segundo cuarto del buffer real el buffer actual de la señal portadora
        carrieranterior[sample] = (carrier[sample] + noise[sample]);//Se actualiza con el buffer de señal portadora de este bloque
    }
    procesamientovocoder();
   
    for (int i = 0; i < buffer.getNumSamples(); i++) {
        float in = timevocoder[i].real() + buffervocodersolape.getSample(0, i);
        buffervocodersolape.setSample(0, i, timevocoder[i + fftsize/2].real());
        
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            auto* channelData = buffer.getWritePointer(channel);
            channelData[i] = in ;

        }
    }
}

void VocoderAudioProcessor::procesamientovocoder()
{
    calculobandas();

    fftmodulator.clear(fftsize);
    fftcarrier.clear(fftsize);

    fft->perform(timemodulator, fftmodulator, false);//se realiza la fft el resultado se guarda en fftmodulator
    fft->perform(timecarrier,fftcarrier, false);//se realiza la fft el resultado se guarda en fftcarrier

    fftvocoder.clear(fftsize);//buffer para almacenar el resultado del vocoder en frecuencia
    timevocoder.clear(fftsize);//buffer para almacenar el resultado del vocoder en tiempo
    AudioBuffer<float> rmsmodulator(1, fftsize);
    AudioBuffer<float>  rmscarrier(1, fftsize);
    rmsmodulator.clear();
    rmscarrier.clear();
    auto* rmsmod = rmsmodulator.getWritePointer(0);//buffer para guardar el valor absoluto del número imaginario de cada muestra de la fft de la moduladora
    auto* rmscar = rmscarrier.getWritePointer(0);//buffer para guardar el valor absoluto del número imaginario de cada muestra de la fft de la portadora

    for (int i = 0; i < numbands-1; i++) {
        
        for (int b = bandas[i]; b < bandas[i + 1]; b++) {
            rmsmod[b] = abs(fftmodulator[b]);
            rmscar[b] = abs(fftcarrier[b]);
        }
        float rmsbandmodulator = rmsmodulator.getRMSLevel(0, bandas[i],bandas[i+1] - bandas[i]);
        //se guarda el valor rms de la banda actual de la señal moduladora
        float rmsbandcarrier = rmscarrier.getRMSLevel(0, bandas[i], bandas[i + 1] - bandas[i]);
        //se guarda el valor rms de la banda actual de la señal portadora
        if (rmsbandmodulator == 0) {
            rmsbandmodulator = 1;
            rmsbandcarrier = 1;
        }
        if (rmsbandcarrier == 0){
            rmsbandcarrier = 1;
        }
        for (int b = bandas[i]; b < bandas[i + 1]; b++) {
           fftvocoder[b].real(fftcarrier[b].real() * rmsbandmodulator / rmsbandcarrier);
           fftvocoder[b].imag(fftcarrier[b].imag() * rmsbandmodulator / rmsbandcarrier);
        }
    }

    fft->perform(fftvocoder, timevocoder, true);//se realiza la ifft el resultado se guarda en timevocoder

}

void VocoderAudioProcessor::calculobandas()
{
    
    int saltobandas = (int)round(fftsize / nbands);
    numbands = (int)round(fftsize / saltobandas);
    bandas.clear();
    bandas.resize(numbands);
    bandas.empty();
    for (int i = 0; i < numbands; i++) {
        bandas[i] = saltobandas * i;
    }
    bandas[numbands-1] = fftsize-1;
}
//==============================================================================
bool VocoderAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* VocoderAudioProcessor::createEditor()
{
    return new VocoderAudioProcessorEditor(*this);
}

//==============================================================================
void VocoderAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    std::unique_ptr<XmlElement> xml(parametros.copyState().createXml());
    copyXmlToBinary(*xml, destData);
}

void VocoderAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<XmlElement> params(getXmlFromBinary(data, sizeInBytes));

    if (params != nullptr)
    {
        if (params->hasTagName(parametros.copyState().getType())) {
            parametros.state = ValueTree::fromXml(*params);
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VocoderAudioProcessor();
}
AudioProcessorValueTreeState::ParameterLayout
VocoderAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> parameters;
    parameters.push_back(std::make_unique<AudioParameterInt>("bandsslider", "BandsSlider", 2, 128, 32));
    parameters.push_back(std::make_unique<AudioParameterFloat>("noiseslider", "NoisesSlider", 0.0f, 1.0f, 0.0f));
    parameters.push_back(std::make_unique<AudioParameterFloat>("sawslider", "SawsSlider", 0.0f, 1.0f, 0.0f));
    parameters.push_back(std::make_unique<AudioParameterFloat>("pulseslider", "PulseSlider", 0.0f, 1.0f, 0.0f));
    parameters.push_back(std::make_unique<AudioParameterFloat>("modulatorslider", "ModulatorSlider", 0.0f, 1.0f, 0.0f));
    return{ parameters.begin(), parameters.end() };
}