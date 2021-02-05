//
// Created by Melefo on 19/02/2020.
//

#include "APUDebug.hpp"
#include "../Utility/Utility.hpp"
#include "../Exceptions/InvalidOpcode.hpp"

using namespace ComSquare::APU;

namespace ComSquare::Debugger
{
	APUDebug::APUDebug(APU &apu, SNES &snes) :
			APU(apu),
			_window(new ClosableWindow<APUDebug>(*this, &APUDebug::disableDebugger)),
			_ui(),
			_snes(snes)
	{
		this->_window->setContextMenuPolicy(Qt::NoContextMenu);
		this->_window->setAttribute(Qt::WA_QuitOnClose, false);
		this->_window->setAttribute(Qt::WA_DeleteOnClose);

		this->_ui.setupUi(this->_window);
		QMainWindow::connect(this->_ui.resumeButton, &QPushButton::clicked, this, &APUDebug::pause);
		QMainWindow::connect(this->_ui.stepButton, &QPushButton::clicked, this, &APUDebug::step);
		this->_window->show();
		this->_updatePanel();
	}

	void APUDebug::_updatePanel()
	{
		this->_ui.port0hexaLineEdit->setText(Utility::to_hex(this->_registers.port0).c_str());
		this->_ui.port0LineEdit->setText(Utility::to_binary(this->_registers.port0).c_str());

		this->_ui.port1hexaLineEdit->setText(Utility::to_hex(this->_registers.port1).c_str());
		this->_ui.port1LineEdit->setText(Utility::to_binary(this->_registers.port1).c_str());

		this->_ui.port2hexaLineEdit->setText(Utility::to_hex(this->_registers.port2).c_str());
		this->_ui.port2LineEdit->setText(Utility::to_binary(this->_registers.port2).c_str());

		this->_ui.port3hexaLineEdit->setText(Utility::to_hex(this->_registers.port3).c_str());
		this->_ui.port3LineEdit->setText(Utility::to_binary(this->_registers.port3).c_str());

		this->_ui.controlhexaLineEdit->setText(Utility::to_hex(this->_registers.ctrlreg).c_str());
		this->_ui.controlLineEdit->setText(Utility::to_binary(this->_registers.ctrlreg).c_str());

		this->_ui.dSPRegAddresshexaLineEdit->setText(Utility::to_hex(this->_registers.dspregAddr).c_str());
		this->_ui.dSPRegAddressLineEdit->setText(Utility::to_binary(this->_registers.dspregAddr).c_str());

		this->_ui.dSPRegDatahexaLineEdit->setText(Utility::to_hex(this->_dsp.read(this->_registers.dspregAddr)).c_str());
		this->_ui.dSPRegDataLineEdit->setText(Utility::to_binary(this->_dsp.read(this->_registers.dspregAddr)).c_str());

		this->_ui.timer0hexaLineEdit->setText(Utility::to_hex(this->_registers.timer0).c_str());
		this->_ui.timer0LineEdit->setText(Utility::to_binary(this->_registers.timer0).c_str());

		this->_ui.timer1hexaLineEdit->setText(Utility::to_hex(this->_registers.timer1).c_str());
		this->_ui.timer1LineEdit->setText(Utility::to_binary(this->_registers.timer1).c_str());

		this->_ui.timer2hexaLineEdit->setText(Utility::to_hex(this->_registers.timer2).c_str());
		this->_ui.timer2LineEdit->setText(Utility::to_binary(this->_registers.timer2).c_str());

		this->_ui.counter0hexaLineEdit->setText(Utility::to_hex(this->_registers.counter0).c_str());
		this->_ui.counter0LineEdit->setText(Utility::to_binary(this->_registers.counter0).c_str());

		this->_ui.counter1hexaLineEdit->setText(Utility::to_hex(this->_registers.counter1).c_str());
		this->_ui.counter1LineEdit->setText(Utility::to_binary(this->_registers.counter1).c_str());

		this->_ui.counter2hexaLineEdit->setText(Utility::to_hex(this->_registers.counter2).c_str());
		this->_ui.counter2LineEdit->setText(Utility::to_binary(this->_registers.counter2).c_str());

		this->_ui.regMemhexaLineEdit->setText(Utility::to_hex(this->_registers.regmem1).c_str());
		this->_ui.regMemLineEdit->setText(Utility::to_binary(this->_registers.regmem1).c_str());

		this->_ui.regMemhexaLineEdit_2->setText(Utility::to_hex(this->_registers.regmem2).c_str());
		this->_ui.regMemLineEdit_2->setText(Utility::to_binary(this->_registers.regmem2).c_str());

		this->_ui.unknownhexaLineEdit->setText(Utility::to_hex(this->_registers.unknown).c_str());
		this->_ui.unknownLineEdit->setText(Utility::to_binary(this->_registers.unknown).c_str());

		this->_ui.stackPointerLineEdit->setText(Utility::to_hex(this->_internalRegisters.sp).c_str());
		this->_ui.xIndexLineEdit->setText(Utility::to_hex(this->_internalRegisters.x).c_str());
		this->_ui.yIndexLineEdit->setText(Utility::to_hex(this->_internalRegisters.y).c_str());
		this->_ui.accumlatorLineEdit->setText(Utility::to_hex(this->_internalRegisters.a).c_str());
		this->_ui.programCounterLineEdit->setText(Utility::to_hex(this->_internalRegisters.pc + 0x0001u).c_str());
		this->_ui.programStatusWordLineEdit->setText(this->_getPSWString().c_str());

		auto voices = this->_dsp.getVoices();
		auto master = this->_dsp.getMaster();
		auto echo = this->_dsp.getEcho();
		auto noise = this->_dsp.getNoise();
		auto brr = this->_dsp.getBrr();
		auto latch = this->_dsp.getLatch();

		this->_ui.mvolLprogressBar->setValue(master.volume[0]);
		this->_ui.mvolRprogressBar->setValue(master.volume[1]);
		this->_ui.evolLprogressBar->setValue(echo.volume[0]);
		this->_ui.evolRprogressBar->setValue(echo.volume[1]);
		this->_ui.echoprogressBar->setValue(echo.feedback);

        uint8_t flg = 0;
        flg += master.reset << 7;
        flg += master.mute << 6;
        flg += echo.enabled << 5;
        flg += noise.clock;
		this->_ui.flagslineEdit->setText(Utility::to_binary(flg).c_str());
		this->_ui.sourceDirectoryLineEdit->setText(Utility::to_hex(brr.offset).c_str());
		this->_ui.echoBufferOffsetLineEdit->setText(Utility::to_hex(echo.data).c_str());
		this->_ui.echoDelayLineEdit->setText(Utility::to_hex(echo.delay).c_str());

		this->_ui.VolumeLprogressBar->setValue(voices[0].volume[0]);
		this->_ui.VolumeRprogressBar->setValue(voices[0].volume[1]);
		this->_ui.WaveHeightprogressBar->setValue(latch.outx);
		this->_ui.EchoFIRCoeffprogressBar->setValue(echo.FIR[0]);
		this->_ui.PitchlineEdit->setText(Utility::to_hex(voices[0].pitch).c_str());
		this->_ui.sourceNumberLineEdit->setText(Utility::to_hex(voices[0].srcn).c_str());
		this->_ui.GainlineEdit->setText(Utility::to_hex(voices[0].gain).c_str());
		this->_ui.EnvelopelineEdit->setText(Utility::to_hex(voices[0].envelope).c_str());
		this->_ui.EnvelopeValueLineEdit->setText(Utility::to_hex(voices[0].envx).c_str());
		this->_ui.KeyOncheckBox->setChecked(voices[0].kon);
		this->_ui.KeyOffcheckBox->setChecked(voices[0].kof);
		this->_ui.NoisecheckBox->setChecked(voices[0].non);
		this->_ui.EchocheckBox->setChecked(voices[0].eon);
		this->_ui.SampleEndcheckBox->setChecked(voices[0].endx);
		this->_ui.PitchModulationcheckBox->setChecked(voices[0].pmon);

		this->_ui.VolumeLprogressBar_2->setValue(voices[1].volume[0]);
		this->_ui.VolumeRprogressBar_2->setValue(voices[1].volume[1]);
		this->_ui.WaveHeightprogressBar_2->setValue(latch.outx);
		this->_ui.EchoFIRCoeffprogressBar_2->setValue(echo.FIR[1]);
		this->_ui.PitchlineEdit_2->setText(Utility::to_hex(voices[1].pitch).c_str());
		this->_ui.sourceNumberLineEdit_2->setText(Utility::to_hex(voices[1].srcn).c_str());
		this->_ui.GainlineEdit_2->setText(Utility::to_hex(voices[1].gain).c_str());
		this->_ui.EnvelopelineEdit_2->setText(Utility::to_hex(voices[1].envelope).c_str());
		this->_ui.EnvelopeValueLineEdit_2->setText(Utility::to_hex(voices[1].envx).c_str());
		this->_ui.KeyOncheckBox_2->setChecked(voices[1].kon);
		this->_ui.KeyOffcheckBox_2->setChecked(voices[1].kof);
		this->_ui.NoisecheckBox_2->setChecked(voices[1].non);
		this->_ui.EchocheckBox_2->setChecked(voices[1].eon);
		this->_ui.SampleEndcheckBox_2->setChecked(voices[1].endx);
		this->_ui.PitchModulationcheckBox_2->setChecked(voices[1].pmon);

		this->_ui.VolumeLprogressBar_3->setValue(voices[2].volume[0]);
		this->_ui.VolumeRprogressBar_3->setValue(voices[2].volume[1]);
		this->_ui.WaveHeightprogressBar_3->setValue(latch.outx);
		this->_ui.EchoFIRCoeffprogressBar_3->setValue(echo.FIR[2]);
		this->_ui.PitchlineEdit_3->setText(Utility::to_hex(voices[2].pitch).c_str());
		this->_ui.sourceNumberLineEdit_3->setText(Utility::to_hex(voices[2].srcn).c_str());
		this->_ui.GainlineEdit_3->setText(Utility::to_hex(voices[2].gain).c_str());
		this->_ui.EnvelopelineEdit_3->setText(Utility::to_hex(voices[2].envelope).c_str());
		this->_ui.EnvelopeValueLineEdit_3->setText(Utility::to_hex(voices[2].envx).c_str());
		this->_ui.KeyOncheckBox_3->setChecked(voices[2].kon);
		this->_ui.KeyOffcheckBox_3->setChecked(voices[2].kof);
		this->_ui.NoisecheckBox_3->setChecked(voices[2].non);
		this->_ui.EchocheckBox_3->setChecked(voices[2].eon);
		this->_ui.SampleEndcheckBox_3->setChecked(voices[2].endx);
		this->_ui.PitchModulationcheckBox_3->setChecked(voices[2].pmon);

		this->_ui.VolumeLprogressBar_4->setValue(voices[3].volume[0]);
		this->_ui.VolumeRprogressBar_4->setValue(voices[3].volume[1]);
		this->_ui.WaveHeightprogressBar_4->setValue(latch.outx);
		this->_ui.EchoFIRCoeffprogressBar_4->setValue(echo.FIR[3]);
		this->_ui.PitchlineEdit_4->setText(Utility::to_hex(voices[3].pitch).c_str());
		this->_ui.sourceNumberLineEdit_4->setText(Utility::to_hex(voices[3].srcn).c_str());
		this->_ui.GainlineEdit_4->setText(Utility::to_hex(voices[3].gain).c_str());
		this->_ui.EnvelopelineEdit_4->setText(Utility::to_hex(voices[3].envelope).c_str());
		this->_ui.EnvelopeValueLineEdit_4->setText(Utility::to_hex(voices[3].envx).c_str());
		this->_ui.KeyOncheckBox_4->setChecked(voices[3].kon);
		this->_ui.KeyOffcheckBox_4->setChecked(voices[3].kof);
		this->_ui.NoisecheckBox_4->setChecked(voices[3].non);
		this->_ui.EchocheckBox_4->setChecked(voices[3].eon);
		this->_ui.SampleEndcheckBox_4->setChecked(voices[3].endx);
		this->_ui.PitchModulationcheckBox_4->setChecked(voices[3].pmon);

		this->_ui.VolumeLprogressBar_5->setValue(voices[4].volume[0]);
		this->_ui.VolumeRprogressBar_5->setValue(voices[4].volume[1]);
		this->_ui.WaveHeightprogressBar_5->setValue(latch.outx);
		this->_ui.EchoFIRCoeffprogressBar_5->setValue(echo.FIR[4]);
		this->_ui.PitchlineEdit_5->setText(Utility::to_hex(voices[4].pitch).c_str());
		this->_ui.sourceNumberLineEdit_5->setText(Utility::to_hex(voices[4].srcn).c_str());
		this->_ui.GainlineEdit_5->setText(Utility::to_hex(voices[4].gain).c_str());
		this->_ui.EnvelopelineEdit_5->setText(Utility::to_hex(voices[4].envelope).c_str());
		this->_ui.EnvelopeValueLineEdit_5->setText(Utility::to_hex(voices[4].envx).c_str());
		this->_ui.KeyOncheckBox_5->setChecked(voices[4].kon);
		this->_ui.KeyOffcheckBox_5->setChecked(voices[4].kof);
		this->_ui.NoisecheckBox_5->setChecked(voices[4].non);
		this->_ui.EchocheckBox_5->setChecked(voices[4].eon);
		this->_ui.SampleEndcheckBox_5->setChecked(voices[4].endx);
		this->_ui.PitchModulationcheckBox_5->setChecked(voices[4].pmon);

		this->_ui.VolumeLprogressBar_6->setValue(voices[5].volume[0]);
		this->_ui.VolumeRprogressBar_6->setValue(voices[5].volume[1]);
		this->_ui.WaveHeightprogressBar_6->setValue(latch.outx);
		this->_ui.EchoFIRCoeffprogressBar_6->setValue(echo.FIR[5]);
		this->_ui.PitchlineEdit_6->setText(Utility::to_hex(voices[5].pitch).c_str());
		this->_ui.sourceNumberLineEdit_6->setText(Utility::to_hex(voices[5].srcn).c_str());
		this->_ui.GainlineEdit_6->setText(Utility::to_hex(voices[5].gain).c_str());
		this->_ui.EnvelopelineEdit_6->setText(Utility::to_hex(voices[5].envelope).c_str());
		this->_ui.EnvelopeValueLineEdit_6->setText(Utility::to_hex(voices[5].envx).c_str());
		this->_ui.KeyOncheckBox_6->setChecked(voices[5].kon);
		this->_ui.KeyOffcheckBox_6->setChecked(voices[5].kof);
		this->_ui.NoisecheckBox_6->setChecked(voices[5].non);
		this->_ui.EchocheckBox_6->setChecked(voices[5].eon);
		this->_ui.SampleEndcheckBox_6->setChecked(voices[5].endx);
		this->_ui.PitchModulationcheckBox_6->setChecked(voices[5].pmon);

		this->_ui.VolumeLprogressBar_7->setValue(voices[6].volume[0]);
		this->_ui.VolumeRprogressBar_7->setValue(voices[6].volume[1]);
		this->_ui.WaveHeightprogressBar_7->setValue(latch.outx);
		this->_ui.EchoFIRCoeffprogressBar_7->setValue(echo.FIR[6]);
		this->_ui.PitchlineEdit_7->setText(Utility::to_hex(voices[6].pitch).c_str());
		this->_ui.sourceNumberLineEdit_7->setText(Utility::to_hex(voices[6].srcn).c_str());
		this->_ui.GainlineEdit_7->setText(Utility::to_hex(voices[6].gain).c_str());
		this->_ui.EnvelopelineEdit_7->setText(Utility::to_hex(voices[6].envelope).c_str());
		this->_ui.EnvelopeValueLineEdit_7->setText(Utility::to_hex(voices[6].envx).c_str());
		this->_ui.KeyOncheckBox_7->setChecked(voices[6].kon);
		this->_ui.KeyOffcheckBox_7->setChecked(voices[6].kof);
		this->_ui.NoisecheckBox_7->setChecked(voices[6].non);
		this->_ui.EchocheckBox_7->setChecked(voices[6].eon);
		this->_ui.SampleEndcheckBox_7->setChecked(voices[6].endx);
		this->_ui.PitchModulationcheckBox_7->setChecked(voices[6].pmon);

		this->_ui.VolumeLprogressBar_8->setValue(voices[7].volume[0]);
		this->_ui.VolumeRprogressBar_8->setValue(voices[7].volume[1]);
		this->_ui.WaveHeightprogressBar_8->setValue(latch.outx);
		this->_ui.EchoFIRCoeffprogressBar_8->setValue(echo.FIR[7]);
		this->_ui.PitchlineEdit_8->setText(Utility::to_hex(voices[7].pitch).c_str());
		this->_ui.sourceNumberLineEdit_8->setText(Utility::to_hex(voices[7].srcn).c_str());
		this->_ui.GainlineEdit_8->setText(Utility::to_hex(voices[7].gain).c_str());
		this->_ui.EnvelopelineEdit_8->setText(Utility::to_hex(voices[7].envelope).c_str());
		this->_ui.EnvelopeValueLineEdit_8->setText(Utility::to_hex(voices[7].envx).c_str());
		this->_ui.KeyOncheckBox_8->setChecked(voices[7].kon);
		this->_ui.KeyOffcheckBox_8->setChecked(voices[7].kof);
		this->_ui.NoisecheckBox_8->setChecked(voices[7].non);
		this->_ui.EchocheckBox_8->setChecked(voices[7].eon);
		this->_ui.SampleEndcheckBox_8->setChecked(voices[7].endx);
		this->_ui.PitchModulationcheckBox_8->setChecked(voices[7].pmon);
	}

	std::string APUDebug::_getPSWString()
	{
		std::string str;
		str += this->_internalRegisters.n ? 'n' : '-';
		str += this->_internalRegisters.v ? 'v' : '-';
		str += this->_internalRegisters.p ? 'p' : '-';
		str += this->_internalRegisters.b ? 'b' : '-';
		str += this->_internalRegisters.h ? 'h' : '-';
		str += this->_internalRegisters.i ? 'i' : '-';
		str += this->_internalRegisters.z ? 'z' : '-';
		str += this->_internalRegisters.c ? 'c' : '-';
		return str;
	}

	std::string APUDebug::_getInstructionString()
	{
		uint8_t opcode = this->_internalRead(this->_internalRegisters.pc);

		switch (opcode) {
		case 0x00:
			return "NOP";
		case 0x10:
			return "BPL";
		case 0x20:
			return "CLRP";
		case 0x30:
			return "BMI";
		case 0x40:
			return "SETP";
		case 0x50:
			return "BVC";
		case 0x60:
			return "CLRC";
		case 0x70:
			return "BVS";
		case 0x80:
			return "SETC";
		case 0x90:
			return "BCC";
		case 0xA0:
			return "EI";
		case 0xB0:
			return "BCS";
		case 0xC0:
			return "DI";
		case 0xD0:
			return "BNE";
		case 0xE0:
			return "CLRV";
		case 0xF0:
			return "BEQ";
		case 0x01:
		case 0x11:
		case 0x21:
		case 0x31:
		case 0x41:
		case 0x51:
		case 0x61:
		case 0x71:
		case 0x81:
		case 0x91:
		case 0xA1:
		case 0xB1:
		case 0xC1:
		case 0xD1:
		case 0xE1:
		case 0xF1:
			return "TCALL";
		case 0x02:
		case 0x22:
		case 0x42:
		case 0x62:
		case 0x82:
		case 0xA2:
		case 0xC2:
		case 0xE2:
			return "SET1";
		case 0x12:
		case 0x32:
		case 0x52:
		case 0x72:
		case 0x92:
		case 0xB2:
		case 0xD2:
		case 0xF2:
			return "CLR1";
		case 0x03:
		case 0x13:
		case 0x23:
		case 0x33:
		case 0x43:
		case 0x53:
		case 0x63:
		case 0x73:
		case 0x83:
		case 0x93:
		case 0xA3:
		case 0xB3:
		case 0xC3:
		case 0xD3:
		case 0xE3:
		case 0xF3:
			return "BBC";
		case 0x04 ... 0x09:
		case 0x14 ... 0x19:
			return "OR";
		case 0x24 ... 0x29:
		case 0x34 ... 0x39:
			return "AND";
		case 0x44 ... 0x49:
		case 0x54 ... 0x59:
			return "EOR";
		case 0x64 ... 0x69:
		case 0x74 ... 0x79:
		case 0xC8:
		case 0xAD:
		case 0x1E:
		case 0x3E:
		case 0x5E:
		case 0x7E:
			return "CMP";
		case 0x84 ... 0x89:
		case 0x94 ... 0x99:
			return "ADC";
		case 0xA4 ... 0xA9:
		case 0xB4 ... 0xB9:
			return "SBC";
		case 0xC4 ... 0xC7:
		case 0xCB ... 0xCD:
		case 0xD4 ... 0xD9:
		case 0xE4 ... 0xE9:
		case 0xEB ... 0xEC:
		case 0xF4 ... 0xFB:
		case 0xDB:
		case 0xC9:
		case 0x5D:
		case 0x7D:
		case 0x8D:
		case 0x9D:
		case 0xBD:
		case 0xDD:
		case 0xFD:
		case 0x8F:
		case 0xAF:
		case 0xBF:
			return "MOV";
		case 0x0A:
		case 0x2A:
			return "OR1";
		case 0x1A:
			return "DECW";
		case 0x3A:
			return "INCW";
		case 0x4A:
		case 0x6A:
			return "AND1";
		case 0x5A:
			return "CMPW";
		case 0x7A:
			return "ADDW";
		case 0x8A:
			return "EOR1";
		case 0x9A:
			return "SUBW";
		case 0xAA:
		case 0xCA:
			return "MOV1";
		case 0xBA:
		case 0xDA:
			return "MOVW";
		case 0xEA:
			return "NOT1";
		case 0x0B ... 0x0C:
		case 0x1B ... 0x1C:
			return "ASL";
		case 0x2B ... 0x2C:
		case 0x3B ... 0x3C:
			return "ROL";
		case 0x4B ... 0x4C:
		case 0x5B ... 0x5C:
			return "LSR";
		case 0x6B ... 0x6C:
		case 0x7B ... 0x7C:
			return "ROR";
		case 0x8B ... 0x8C:
		case 0x9B ... 0x9C:
		case 0xDC:
		case 0x1D:
			return "DEC";
		case 0xAB ... 0xAC:
		case 0xBB ... 0xBC:
		case 0xFC:
		case 0x3D:
			return "INC";
		case 0x0D:
		case 0x2D:
		case 0x4D:
		case 0x6D:
			return "PUSH";
		case 0xED:
			return "NOTC";
		case 0x0E:
			return "TSET1";
		case 0x2E:
		case 0xDE:
			return "CBNE";
		case 0x4E:
			return "TCLR1";
		case 0x6E:
		case 0xFE:
			return "DBNZ";
		case 0x8E:
		case 0xAE:
		case 0xCE:
		case 0xEE:
			return "POP";
		case 0x9E:
			return "DIV";
		case 0xBE:
			return "DAS";
		case 0x0F:
			return "BRK";
		case 0x1F:
		case 0x5F:
			return "JMP";
		case 0x2F:
			return "BRA";
		case 0x3F:
			return "CALL";
		case 0x4F:
			return "PCALL";
		case 0x6F:
			return "RET";
		case 0x7F:
			return "RETI";
		case 0x9F:
			return "XCN";
		case 0xCF:
			return "MUL";
		case 0xDF:
			return "DAA";
		case 0xEF:
			return "SLEEP";
		case 0xFF:
			return "STOP";
		default:
			return "Unknown";
		}
	}

	int APUDebug::_executeInstruction()
	{
		int cycles = 0;

		if (this->_isPaused)
			return 0xFF;
		if (this->_isStepping) {
			this->_isStepping = false;
			this->_isPaused = true;
		}
		this->_ui.logger->append(APUDebug::_getInstructionString().c_str());
		cycles = APU::_executeInstruction();
		this->_updatePanel();
		return cycles;
	}

	void APUDebug::update(unsigned cycles)
	{
		try {
			if (this->_isPaused)
				return;
			APU::update(cycles);
		} catch (InvalidOpcode &e) {
			this->pause();
			this->_ui.logger->append(e.what());
		}
	}

	void APUDebug::step()
	{
		this->_isStepping = true;
		this->_isPaused = false;
	}

	void APUDebug::pause()
	{
		this->_isPaused = !this->_isPaused;
		if (this->_isPaused)
			this->_ui.resumeButton->setText("Resume");
		else
			this->_ui.resumeButton->setText("Pause");
	}


	void APUDebug::disableDebugger()
	{
		this->_snes.disableAPUDebugging();
	}

	bool APUDebug::isDebugger() const
	{
		return true;
	}

	void APUDebug::focus()
	{
		this->_window->activateWindow();
	}
}