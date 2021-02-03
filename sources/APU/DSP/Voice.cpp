//
// Created by melefo on 2/1/21.
//

#include "DSP.hpp"
#include <algorithm>

namespace ComSquare::APU::DSP
{
    void DSP::voiceOutput(Voice &voice, bool channel)
    {
        int out = this->_latch.output * voice.volume[channel] >> 7;

        this->_master.output[channel] += out;
        if (!voice.echo)
            return;
        this->_echo.volume[channel] += out;
    }

    void DSP::voice1(Voice &voice)
    {

    }

    void DSP::voice2(Voice &voice)
    {
        /*uint16_t addr = this->_brr.
        this->_latch.adsr1 = voice.adsr1;*/
    }

    void DSP::voice3(Voice &voice)
    {

    }

    void DSP::voice3a(Voice &voice)
    {

    }

    void DSP::voice3b(Voice &voice)
    {

    }

    void DSP::voice3c(Voice &voice)
    {

    }

    void DSP::voice4(Voice &voice)
    {

    }

    void DSP::voice5(Voice &voice)
    {
        this->voiceOutput(voice, 1);

        voice.endx |= voice.loop;
        if (voice.konDelay == 5)
            voice.endx &= ~voice.endx;
    }

    void DSP::voice6(Voice &voice)
    {

    }

    void DSP::voice7(Voice &voice)
    {

    }

    void DSP::voice8(Voice &voice)
    {

    }

    void DSP::voice9(Voice &voice)
    {

    }
}