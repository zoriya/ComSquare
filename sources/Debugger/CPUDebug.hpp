//
// Created by anonymus-raccoon on 2/14/20.
//

#ifndef COMSQUARE_CPUDEBUG_HPP
#define COMSQUARE_CPUDEBUG_HPP

#include "../CPU/CPU.hpp"
#include "../Renderer/SFRenderer.hpp"
#include "../SNES.hpp"
#include "../../ui/ui_cpu.h"
#include "ClosableWindow.hpp"

namespace ComSquare::Debugger
{
	class CPUDebug;
}

//! @brief The qt model that show the disassembly.
class DisassemblyModel : public QAbstractTableModel
{
Q_OBJECT
private:
	ComSquare::Debugger::CPUDebug &_cpu;
public:
	explicit DisassemblyModel(ComSquare::Debugger::CPUDebug &cpu);
	DisassemblyModel(const DisassemblyModel &) = delete;
	const DisassemblyModel &operator=(const DisassemblyModel &) = delete;
	~DisassemblyModel() override = default;

	//! @brief The number of row the table has.
	int rowCount(const QModelIndex &parent) const override;
	//! @brief The number of column the table has.
	int columnCount(const QModelIndex &parent) const override;
	//! @brief Return a data representing the table cell.
	QVariant data(const QModelIndex &index, int role) const override;
};

namespace ComSquare::Debugger
{
	//! @brief A custom CPU with a window that show it's registers and the disassembly.
	class CPUDebug : public CPU::CPU, public QObject {
	private:
		//! @brief The QT window for this debugger.
		ClosableWindow<CPUDebug> *_window;
		//! @brief A widget that contain the whole UI.
		Ui::CPUView _ui;
		//! @brief The disassembly viewer's model.
		DisassemblyModel _model;
		//! @brief If this is set to true, the execution of the CPU will be paused.
		bool _isPaused = true;
		//! @brief If this is set to true, the CPU will execute one instruction and pause itself.
		bool _isStepping = false;
		//! @brief A reference to the snes (to disable the debugger).
		SNES &_snes;
		//! @brief Reimplement the basic instruction execution method to log instructions inside the logger view.
		unsigned _executeInstruction(uint8_t opcode) override;
		//! @brief Get a printable string representing an instruction at the program counter given as parameter.
		std::string _getInstructionString(uint24_t pc);
		//! @brief Get a printable string representing the flags.
		std::string _getFlagsString();
		//! @brief Update the register's panel (accumulator, stack pointer...)
		void _updateRegistersPanel();

		//! @brief Return a printable string corresponding to the value of an immediate addressing mode for a.
		std::string _getImmediateValueForA(uint24_t pc);
		//! @brief Return a printable string corresponding to the value of an immediate addressing mode for x.
		std::string _getImmediateValueForX(uint24_t pc);
		//! @brief Return a printable string corresponding to the value of a 8bits immediate addressing mode (used only with SEP and REP).
		std::string _getImmediateValue8Bits(uint24_t pc);
		//! @brief Return a printable string corresponding to the value of a 16bits immediate addressing mode.
		std::string _getImmediateValue16Bits(uint24_t pc);
		//! @brief Return a printable string corresponding to the value of a direct addressing mode.
		std::string _getDirectValue(uint24_t pc);
		//! @brief Return a printable string corresponding to the value of an absolute addressing mode.
		std::string _getAbsoluteValue(uint24_t pc);
		//! @brief Return a printable string corresponding to the value of an absolute long addressing mode.
		std::string _getAbsoluteLongValue(uint24_t pc);
		//! @brief Return a printable string corresponding to the value of a direct index by x addressing mode.
		std::string _getDirectIndexedByXValue(uint24_t pc);

	public:
		//! @brief Pause/Resume the CPU.
		void pause();
		//! @brief Step - Execute a single instruction.
		void step();
		//! @brief Clear the history panel.
		void clearHistory();
		//! @brief Called when the window is closed. Turn off the debugger and revert to a basic CPU.
		void disableDebugger();
	public:
		//! @brief Update the UI when reseting the CPU.
		int RESB(uint24_t) override;
		//! @brief Convert a basic CPU to a debugging CPU.
		explicit CPUDebug(ComSquare::CPU::CPU &cpu, SNES &snes);
		CPUDebug(const CPUDebug &) = delete;
		CPUDebug &operator=(const CPUDebug &) = delete;
		~CPUDebug() override = default;

		//! @brief Return true if the CPU is overloaded with debugging features.
		bool isDebugger() override;

		//! @brief Focus the debugger's window.
		void focus();

		//! @brief Override the basic cpu's update to allow pausing of the CPU only.
		unsigned update() override;
	};
}

#endif //COMSQUARE_CPUDEBUG_HPP
