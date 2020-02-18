//
// Created by anonymus-raccoon on 2/18/20.
//

#include "HeaderViewer.hpp"
#include "../Utility/Utility.hpp"

namespace ComSquare::Debugger
{
	HeaderViewer::HeaderViewer(ComSquare::Cartridge::Cartridge &cartridge) :
		_cartridge(cartridge),
		_ui()
	{
		this->setContextMenuPolicy(Qt::NoContextMenu);
		this->setAttribute(Qt::WA_QuitOnClose, false);

		this->_ui.setupUi(this);
		this->_ui.nameLineEdit->setText(cartridge.header.gameName.c_str());
		std::string memType;
		if (cartridge.header.mappingMode & Cartridge::LoRom)
			memType += "LoRom ";
		if (cartridge.header.mappingMode & Cartridge::HiRom)
			memType += "HiRom ";
		if (cartridge.header.mappingMode & Cartridge::SlowRom)
			memType += "SlowRom ";
		if (cartridge.header.mappingMode & Cartridge::FastRom)
			memType += "FastRom ";
		if (cartridge.header.mappingMode & Cartridge::ExRom)
			memType += "ExRom ";
		this->_ui.mappingLineEdit->setText(memType.c_str());
		this->_ui.romSizeLineEdit->setText(ComSquare::Utility::to_hex(cartridge.header.romSize).c_str());
		this->_ui.sRamSizeLineEdit->setText(ComSquare::Utility::to_hex(cartridge.header.sramSize).c_str());
		this->_ui.versionLineEdit->setText(std::to_string(cartridge.header.version).c_str());
		this->_ui.creatorIDLineEdit->setText(std::to_string(cartridge.header.creatorID).c_str());
		this->_ui.checksumLineEdit->setText(ComSquare::Utility::to_hex(cartridge.header.checksum).c_str());
		this->_ui.checksumComplementLineEdit->setText(ComSquare::Utility::to_hex(cartridge.header.checksumComplement).c_str());

		this->_ui.coProcessorLineEdit->setText(ComSquare::Utility::to_hex(cartridge.header.emulationInterrupts.cop).c_str());
		this->_ui.breakLineEdit->setText(ComSquare::Utility::to_hex(cartridge.header.emulationInterrupts.brk).c_str());
		this->_ui.abortLineEdit->setText(ComSquare::Utility::to_hex(cartridge.header.emulationInterrupts.abort).c_str());
		this->_ui.nMInteruptLineEdit->setText(ComSquare::Utility::to_hex(cartridge.header.emulationInterrupts.nmi).c_str());
		this->_ui.resetLineEdit->setText(ComSquare::Utility::to_hex(cartridge.header.emulationInterrupts.reset).c_str());
		this->_ui.interruptRequestLineEdit->setText(ComSquare::Utility::to_hex(cartridge.header.emulationInterrupts.irq).c_str());

		this->_ui.coProcessorLineEditNat->setText(ComSquare::Utility::to_hex(cartridge.header.nativeInterrupts.cop).c_str());
		this->_ui.breakLineEditNat->setText(ComSquare::Utility::to_hex(cartridge.header.nativeInterrupts.brk).c_str());
		this->_ui.abortLineEditNat->setText(ComSquare::Utility::to_hex(cartridge.header.nativeInterrupts.abort).c_str());
		this->_ui.nMInteruptLineEditNat->setText(ComSquare::Utility::to_hex(cartridge.header.nativeInterrupts.nmi).c_str());
		this->_ui.resetLineEditNat->setText(ComSquare::Utility::to_hex(cartridge.header.nativeInterrupts.reset).c_str());
		this->_ui.interruptRequestLineEditNat->setText(ComSquare::Utility::to_hex(cartridge.header.nativeInterrupts.irq).c_str());
		this->show();
	}
}