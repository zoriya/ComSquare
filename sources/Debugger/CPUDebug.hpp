//
// Created by anonymus-raccoon on 2/14/20.
//

#ifndef COMSQUARE_CPUDEBUG_HPP
#define COMSQUARE_CPUDEBUG_HPP

#include <QtWidgets/QStyledItemDelegate>
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
	//! @brief Override the headers to use hex values.
	QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
};

//! @brief The qt class that highlight breakpoints and the PC's position
class RowPainter : public QStyledItemDelegate {
	Q_OBJECT
private:
	//! @brief The CPU to get PC and breakpoints from.
	ComSquare::Debugger::CPUDebug &_cpu;
public:
	explicit RowPainter(ComSquare::Debugger::CPUDebug &cpu, QObject *parent = nullptr);
	RowPainter &operator=(const RowPainter &) = delete;
	~RowPainter() override = default;
protected:
	QSize sizeHint(const QStyleOptionViewItem &options, const QModelIndex &index) const override;
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

namespace ComSquare::Debugger
{
	enum TrustLevel {
		Safe,
		Unsafe,
		Compromised
	};

	//! @brief Struct used to emulate the state of the processor during the disassembly since instructions take a different amount of space depending on some flags.
	struct DisassemblyContext {
		//! @brief The accumulator and Memory width flag (in native mode only) - 0 = 16 bits mode, 1 = 8 bits mode.
		//! @info If this flag is set to false, instructions that have the ImmediateByA addressing mode take 1 more bit of space.
		bool mFlag = true;
		//!	@brief The indeX register width flag (in native mode only) - 0 = 16 bits mode, 1 = 8 bits mode OR the Break flag (in emulation mode only)
		//! @info If this flag is set to false, instructions that have the ImmediateByX addressing mode take 1 more bit of space.
		bool xFlag = true;
		//! @brief Is the CPU emulating a 6502? If yes, some instructions don't change flags the same way.
		bool isEmulationMode = true;
		//! @brief Sometimes, the flags can't be tracked correctly after an instruction so the next instructions may not be correctly disassembled.
		TrustLevel level = Safe;
	};

	//! @brief Struct representing an instruction in an human readable way (created by disassembling the rom).
	struct DisassembledInstruction : public CPU::Instruction {
		//! @brief The address of the instruction
		uint24_t address;
		//! @brief A string representing the argument with the right addressing mode.
		std::string argument;
		//! @brief The opcode of the instruction
		uint8_t opcode;
		//! @brief Are we sure that this instruction has been correctly disassembled?
		TrustLevel level;

		DisassembledInstruction(const CPU::Instruction &instruction, uint24_t address, std::string argument, uint8_t opcode);
		DisassembledInstruction(const DisassembledInstruction &) = default;
		DisassembledInstruction &operator=(const DisassembledInstruction &) = default;
		~DisassembledInstruction() = default;

		std::string toString();
	};

	//! @brief A custom CPU with a window that show it's registers and the disassembly.
	class CPUDebug : public CPU::CPU, public QObject {
	private:
		//! @brief The QT window for this debugger.
		ClosableWindow<CPUDebug> *_window;
		//! @brief A widget that contain the whole UI.
		Ui::CPUView _ui;
		//! @brief The disassembly viewer's model.
		DisassemblyModel _model;
		//! @brief A custom painter that highlight breakpoints and the PC's position.
		RowPainter _painter;
		//! @brief If this is set to true, the execution of the CPU will be paused.
		bool _isPaused = true;
		//! @brief If this is set to true, the CPU will execute one instruction and pause itself.
		bool _isStepping = false;
		//! @brief A reference to the snes (to disable the debugger).
		SNES &_snes;
		//! @brief Reimplement the basic instruction execution method to log instructions inside the logger view.
		unsigned _executeInstruction(uint8_t opcode) override;
		//! @brief Return a disassembly context representing the current state of the processor.
		DisassemblyContext _getDisassemblyContext();
		//! @brief Disassemble part of the memory (using the bus) and parse it to a map of address and disassembled instruction.
		//! @param ctx The initial context of the processor before the disassembly begin.
		std::vector<DisassembledInstruction> _disassemble(uint24_t startAddr, uint24_t size, DisassemblyContext &ctx);
		//! @brief Update disassembly with the new state of the processor.
		void _updateDisassembly(uint24_t refreshSize = 0xFF);
		//! @brief Parse the instruction at the program counter given to have human readable information.
		DisassembledInstruction _parseInstruction(uint24_t pc, DisassemblyContext &ctx);
		//! @brief Get the parameter of the instruction as an hexadecimal string.
		std::string _getInstructionParameter(ComSquare::CPU::Instruction &instruction, uint24_t pc, DisassemblyContext &ctx);
		//! @brief Get a printable string representing the flags.
		std::string _getFlagsString();
		//! @brief Update the register's panel (accumulator, stack pointer...)
		void _updateRegistersPanel();

		//! @brief Return a printable string corresponding to the value of a 8 or 16 bits immediate addressing mode.
		//! @param dual Set this to true if the instruction take 16bits and not 8. (used for the immediate by a when the flag m is not set or the immediate by x if the flag x is not set).
		std::string _getImmediateValue(uint24_t pc, bool dual);
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
		//! @brief The list of disassembled instructions to show on the debugger.
		std::vector<DisassembledInstruction> disassembledInstructions;
		//! @brief Return the current program counter of this CPU.
		uint24_t getPC();
		//! @brief Update the UI when resetting the CPU.
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
