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
		this->_ui.logger->setRowCount(0x20);
		this->_ui.logger->setColumnCount(3);
		this->_ui.logger->horizontalHeader()->setHidden(true);
		this->_ui.logger->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		this->_ui.logger->setShowGrid(false);
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
		this->_ui.programCounterLineEdit->setText(Utility::to_hex(this->_internalRegisters.pc).c_str());
		this->_ui.bFlagCheckBox->setChecked(this->_internalRegisters.b);
		this->_ui.nFlagCheckBox->setChecked(this->_internalRegisters.n);
		this->_ui.pFlagCheckBox->setChecked(this->_internalRegisters.p);
		this->_ui.hFlagCheckBox->setChecked(this->_internalRegisters.h);
		this->_ui.vFlagCheckBox->setChecked(this->_internalRegisters.v);
		this->_ui.iFlagCheckBox->setChecked(this->_internalRegisters.i);
		this->_ui.zFlagCheckBox->setChecked(this->_internalRegisters.z);
		this->_ui.cFlagCheckBox->setChecked(this->_internalRegisters.c);

		auto voices = this->_dsp.getVoices();
		auto master = this->_dsp.getMaster();
		auto echo = this->_dsp.getEcho();
		auto noise = this->_dsp.getNoise();
		auto brr = this->_dsp.getBrr();
		auto latch = this->_dsp.getLatch();
		auto max = std::numeric_limits<int8_t>::max();

		this->_ui.mvolLprogressBar->setValue(master.volume[0] * 100 / max);
		this->_ui.mvolRprogressBar->setValue(master.volume[1] * 100 / max);
		this->_ui.evolLprogressBar->setValue(echo.volume[0] * 100 / max);
		this->_ui.evolRprogressBar->setValue(echo.volume[1] * 100 / max);
		this->_ui.echoprogressBar->setValue(echo.feedback * 100 / max);

		uint8_t flg = 0;
		flg += master.reset << 7;
		flg += master.mute << 6;
		flg += echo.enabled << 5;
		flg += noise.clock;
		this->_ui.flagslineEdit->setText(Utility::to_binary(flg).c_str());
		this->_ui.sourceDirectoryLineEdit->setText(Utility::to_hex(brr.offset).c_str());
		this->_ui.echoBufferOffsetLineEdit->setText(Utility::to_hex(echo.data).c_str());
		this->_ui.echoDelayLineEdit->setText(Utility::to_hex(echo.delay).c_str());

		this->_ui.VolumeLprogressBar->setValue(voices[0].volume[0] * 100 / max);
		this->_ui.VolumeRprogressBar->setValue(voices[0].volume[1] * 100 / max);
		this->_ui.WaveHeightprogressBar->setValue(latch.outx * 100 / max);
		this->_ui.EchoFIRCoeffprogressBar->setValue(echo.FIR[0] * 100 / max);
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

		this->_ui.VolumeLprogressBar_2->setValue(voices[1].volume[0] * 100 / max);
		this->_ui.VolumeRprogressBar_2->setValue(voices[1].volume[1] * 100 / max);
		this->_ui.WaveHeightprogressBar_2->setValue(latch.outx * 100 / max);
		this->_ui.EchoFIRCoeffprogressBar_2->setValue(echo.FIR[1] * 100 / max);
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

		this->_ui.VolumeLprogressBar_3->setValue(voices[2].volume[0] * 100 / max);
		this->_ui.VolumeRprogressBar_3->setValue(voices[2].volume[1] * 100 / max);
		this->_ui.WaveHeightprogressBar_3->setValue(latch.outx * 100 / max);
		this->_ui.EchoFIRCoeffprogressBar_3->setValue(echo.FIR[2] * 100 / max);
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

		this->_ui.VolumeLprogressBar_4->setValue(voices[3].volume[0] * 100 / max);
		this->_ui.VolumeRprogressBar_4->setValue(voices[3].volume[1] * 100 / max);
		this->_ui.WaveHeightprogressBar_4->setValue(latch.outx * 100 / max);
		this->_ui.EchoFIRCoeffprogressBar_4->setValue(echo.FIR[3] * 100 / max);
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

		this->_ui.VolumeLprogressBar_5->setValue(voices[4].volume[0] * 100 / max);
		this->_ui.VolumeRprogressBar_5->setValue(voices[4].volume[1] * 100 / max);
		this->_ui.WaveHeightprogressBar_5->setValue(latch.outx * 100 / max);
		this->_ui.EchoFIRCoeffprogressBar_5->setValue(echo.FIR[4] * 100 / max);
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

		this->_ui.VolumeLprogressBar_6->setValue(voices[5].volume[0] * 100 / max);
		this->_ui.VolumeRprogressBar_6->setValue(voices[5].volume[1] * 100 / max);
		this->_ui.WaveHeightprogressBar_6->setValue(latch.outx * 100 / max);
		this->_ui.EchoFIRCoeffprogressBar_6->setValue(echo.FIR[5] * 100 / max);
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

		this->_ui.VolumeLprogressBar_7->setValue(voices[6].volume[0] * 100 / max);
		this->_ui.VolumeRprogressBar_7->setValue(voices[6].volume[1] * 100 / max);
		this->_ui.WaveHeightprogressBar_7->setValue(latch.outx * 100 / max);
		this->_ui.EchoFIRCoeffprogressBar_7->setValue(echo.FIR[6] * 100 / max);
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

		this->_ui.VolumeLprogressBar_8->setValue(voices[7].volume[0] * 100 / max);
		this->_ui.VolumeRprogressBar_8->setValue(voices[7].volume[1] * 100 / max);
		this->_ui.WaveHeightprogressBar_8->setValue(latch.outx * 100 / max);
		this->_ui.EchoFIRCoeffprogressBar_8->setValue(echo.FIR[7] * 100 / max);
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

		this->_updateLogger();
	}

	void APUDebug::_updateLogger()
	{
		QStringList labels = QStringList();
		uint16_t offset = 0;

		if (this->_pc != 0)
		{
			auto pc = this->_internalRegisters.pc;

			this->_internalRegisters.pc = this->_pc;
			this->_appendInstruction(0);
			labels.append(Utility::to_hex(this->_pc).c_str());
			this->_internalRegisters.pc = pc;
		}
		else
			labels.append("$0000");
		for (uint16_t i = 1; i < 0x20; i++)
		{
			auto pc = this->_internalRegisters.pc;

			offset += this->_appendInstruction(i);
			labels.append(Utility::to_hex(pc).c_str());
		}
		this->_ui.logger->setVerticalHeaderLabels(labels);
		for (int i = 0; i < 3; i++)
			this->_ui.logger->item(1, i)->setData(Qt::BackgroundRole, QColor(200, 255, 148));
		this->_internalRegisters.pc -= offset;
	}

	int APUDebug::_appendInstruction(int row)
	{
		auto instruction = this->_getInstruction();
		std::string operand;

		this->_ui.logger->setItem(row, 0, new QTableWidgetItem(instruction.name.c_str()));

		operand = this->_getOperand(std::get<0>(instruction.operands));
		this->_ui.logger->setItem(row, 1, new QTableWidgetItem(operand.c_str()));
		if (operand.empty())
			this->_ui.logger->item(row, 1)->setData(Qt::BackgroundRole, QColor(220, 220, 220));

		operand = this->_getOperand(std::get<1>(instruction.operands));
		this->_ui.logger->setItem(row, 2, new QTableWidgetItem(operand.c_str()));
		if (operand.empty())
			this->_ui.logger->item(row, 2)->setData(Qt::BackgroundRole, QColor(220, 220, 220));
		return instruction.size;
	}

	std::string APUDebug::_getOperand(Operand ope)
	{
		switch (ope) {
			case None:
				return "";
			case A:
				return Utility::to_hex(this->_internalRegisters.a);
			case X:
				return Utility::to_hex(this->_internalRegisters.x);
			case Y:
				return Utility::to_hex(this->_internalRegisters.y);
			case SP:
				return Utility::to_hex(this->_internalRegisters.sp);
			case PSW:
				return Utility::to_hex(this->_internalRegisters.psw);
			case ImmediateData:
				return Utility::to_hex(this->_getImmediateData());
			case IndexXAddr:
				return Utility::to_hex(this->_getIndexXAddr());
			case IndexYAddr:
				return Utility::to_hex(this->_getIndexYAddr());
			case AbsoluteAddr:
				return Utility::to_hex(this->_getAbsoluteAddr());
			case AbsoluteBit: {
				auto pair = this->_getAbsoluteBit();
				return Utility::to_hex(std::get<0>(pair)) + Utility::to_hex(std::get<1>(pair));
			}
			case AbsoluteAddrByX:
				return Utility::to_hex(this->_getAbsoluteAddrByX());
			case AbsoluteAddrByY:
				return Utility::to_hex(this->_getAbsoluteAddrByY());
			case AbsoluteByXAddr:
				return Utility::to_hex(this->_getAbsoluteByXAddr());
			case AbsoluteDirectByXAddr:
				return Utility::to_hex(this->_getAbsoluteDirectByXAddr());
			case AbsoluteDirectAddrByY:
				return Utility::to_hex(this->_getAbsoluteDirectAddrByY());
			case DirectAddr:
				return Utility::to_hex(this->_getDirectAddr());
			case DirectAddrByX:
				return Utility::to_hex(this->_getDirectAddrByX());
			case DirectAddrByY:
				return Utility::to_hex(this->_getDirectAddrByY());
		}
		return "UNKNOWN";
	}

	Instruction &APUDebug::_getInstruction()
	{
		uint8_t opcode = this->_getImmediateData();

		return this->_instructions[opcode];
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
		cycles = APU::_executeInstruction();
		this->_updatePanel();
		return cycles;
	}

	void APUDebug::update(unsigned cycles)
	{
		this->_pc = this->_internalRegisters.pc;
		try {
			if (this->_isPaused)
				return;
			APU::update(cycles);
		} catch (InvalidOpcode &e) {
			this->pause();
			//this->_ui.logger->append(e.what());
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