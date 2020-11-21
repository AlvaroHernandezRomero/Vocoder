/*
  ==============================================================================

    SynthSound.h
    Created: 4 May 2020 1:35:58pm
    Author:  Alvaro

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class SynthSound : public SynthesiserSound
{
 
public:

    bool appliesToNote(int /*	midiNoteNumber*/) 
    {
        return true;
    }

     bool appliesToChannel(int 	/*midiChannel*/)
    {
        return true;
    }



};