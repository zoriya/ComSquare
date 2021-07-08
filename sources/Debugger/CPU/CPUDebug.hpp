//
// Created by anonymus-raccoon on 2/14/20.
//

#pragma once

#include "Debugger/ClosableWindow.hpp"
#include "Exceptions/DebuggableError.hpp"
#include "ui/ui_cpuView.h"
#include "Models/Ints.hpp"
#include "Memory/MemoryBus.hpp"
#include "CPU/Instruction.hpp"
#include <QtWidgets/QStyledItemDelegate>
#include <filesystem>
#include <optional>
#include <QTimer>

namespace ComSquare
{
	class SNES;

	namespace CPU
	{
		class CPU;
	}

	namespace Debugger::CPU
	{
		class CPUDebug;

		//! @brief An instruction that has already been executed. Used for the history viewer
		struct ExecutedInstruction
		{
			//! @brief Opcode of the instruction
			uint8_t opcode;
			//! @brief The name of the instruction
			std::string name;
			//! @brief Readable parameters (disassembly style)
			std::string params;
			//! @brief The address to read from after processing the parameter.
			std::string proceededParams;
		};

		//! @brief The qt model that show the stack.
		class StackModel : public QAbstractTableModel
		{
		Q_OBJECT
		private:
			Memory::IMemoryBus &_bus;
			CPUDebug &_cpu;

		public:
			explicit StackModel(Memory::IMemoryBus &bus, CPUDebug &cpu);
			StackModel(const StackModel &) = delete;
			const StackModel &operator=(const StackModel &) = delete;
			~StackModel() override = default;

			//! @brief The number of row the table has.
			[[nodiscard]] int rowCount(const QModelIndex &parent) const override;
			//! @brief The number of column the table has.
			[[nodiscard]] int columnCount(const QModelIndex &parent) const override;
			//! @brief Return a data representing the table cell.
			[[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
			//! @brief Override the headers to use hex values.
			[[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
		};

		//! @brief The qt model that show the history.
		class HistoryModel : public QAbstractTableModel
		{
		Q_OBJECT
		private:
			std::vector<ExecutedInstruction> _instructions = {};

		public:
			HistoryModel();
			HistoryModel(const HistoryModel &) = delete;
			const HistoryModel &operator=(const HistoryModel &) = delete;
			~HistoryModel() override = default;

			//! @brief Log a new instruction
			void log(const ExecutedInstruction &);
			//! @brief Remove every instructions of the history.
			void clear();

			//! @brief The number of row the table has.
			[[nodiscard]] int rowCount(const QModelIndex &parent) const override;
			//! @brief The number of column the table has.
			[[nodiscard]] int columnCount(const QModelIndex &parent) const override;
			//! @brief Return a data representing the table cell.
			[[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
			//! @brief Override the headers to use hex values.
			[[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
		};

		//! @brief The qt model that show the disassembly.
		class DisassemblyModel : public QAbstractTableModel
		{
		Q_OBJECT
		private:
			CPUDebug &_cpu;

		public:
			explicit DisassemblyModel(CPUDebug &cpu);
			DisassemblyModel(const DisassemblyModel &) = delete;
			const DisassemblyModel &operator=(const DisassemblyModel &) = delete;
			~DisassemblyModel() override = default;

			//! @brief The number of row the table has.
			[[nodiscard]] int rowCount(const QModelIndex &parent) const override;
			//! @brief The number of column the table has.
			[[nodiscard]] int columnCount(const QModelIndex &parent) const override;
			//! @brief Return a data representing the table cell.
			[[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
			//! @brief Override the headers to use hex values.
			[[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
		};

		//! @brief The qt class that highlight breakpoints and the PC's position
		class RowPainter : public QStyledItemDelegate
		{
		Q_OBJECT
		private:
			//! @brief The CPU to get PC and breakpoints from.
			CPUDebug &_cpu;

		public:
			explicit RowPainter(CPUDebug &cpu, QObject *parent = nullptr);
			RowPainter &operator=(const RowPainter &) = delete;
			~RowPainter() override = default;

		protected:
			[[nodiscard]] QSize sizeHint(const QStyleOptionViewItem &options, const QModelIndex &index) const override;
			void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
		};

		enum TrustLevel
		{
			Safe,
			Unsafe,
			Compromised
		};

		//! @brief Struct used to emulate the state of the processor during the disassembly since instructions take a different amount of space depending on some flags.
		struct DisassemblyContext
		{
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
		struct DisassembledInstruction : public ComSquare::CPU::Instruction
		{
			//! @brief The address of the instruction
			uint24_t address;
			//! @brief A string representing the argument with the right addressing mode.
			std::string argument;
			//! @brief The opcode of the instruction
			uint8_t opcode;
			//! @brief Are we sure that this instruction has been correctly disassembled?
			TrustLevel level;

			DisassembledInstruction(const ComSquare::CPU::Instruction &instruction,
									uint24_t address,
									std::string argument,
			                        uint8_t opcode);
			DisassembledInstruction(const DisassembledInstruction &) = default;
			DisassembledInstruction &operator=(const DisassembledInstruction &) = default;
			~DisassembledInstruction() = default;
		};

		//! @brief Struct representing a breakpoint set by the user or by the app
		struct Breakpoint
		{
			//! @brief The address of the breakpoint
			uint24_t address;
			//! @brief If this is true, the breakpoint will be deleted on first hit and won't be shown on the disassembly view.
			bool oneTime;
		};

		//! @brief Struct representing a label.
		struct Label
		{
			//! @brief The address of this label
			uint24_t address;
			//! @brief The name of this label
			std::string name;
			//! @brief The size of the definition related to this label
			std::optional<unsigned> size;
		};

		//! @brief A window that show registers and the disassembly of a CPU.
		class CPUDebug : public QObject
		{
		Q_OBJECT
		private:
			//! @brief The basic CPU to debug.
			ComSquare::CPU::CPU &_cpu;
			//! @brief The QT window for this debugger.
			ClosableWindow *_window;
			//! @brief Internal timer used for update intervals.
			QTimer _timer;
			//! @brief A widget that contain the whole UI.
			Ui::CPUView _ui;
			//! @brief The disassembly viewer's model.
			DisassemblyModel _model;
			//! @brief A custom painter that highlight breakpoints and the PC's position.
			RowPainter _painter;
			//! @brief The stack viewer's model.
			StackModel _stackModel;
			//! @brief The history model.
			HistoryModel _historyModel;
			//! @brief If this is set to true, the execution of the CPU will be paused.
			bool _isPaused = true;
			//! @brief If this is set to true, the CPU will execute one instruction and pause itself.
			bool _isStepping = false;
			//! @brief A reference to the snes (to disable the debugger).
			SNES &_snes;
			//! @brief A list of labels and their size.
			std::vector<Label> _labels;
			//! @brief The callback ID for the on reset of the CPU.
			int _callback;

			//! @brief Load labels from a symbol file.
			void _loadLabels(std::filesystem::path romPath);
			//! @brief Add the instruction under the PC to the history log.
			void _logInstruction();
			//! @brief Return a disassembly context representing the current state of the processor.
			DisassemblyContext _getDisassemblyContext();
			//! @brief Disassemble part of the memory (using the bus) and parse it to a map of address and disassembled instruction.
			//! @param ctx The initial context of the processor before the disassembly begin.
			std::vector<DisassembledInstruction> _disassemble(uint24_t startAddr, uint24_t size,
			                                                  DisassemblyContext &ctx);
			//! @brief Update disassembly with the new state of the processor.
			void _updateDisassembly(uint24_t start, uint24_t refreshSize = 0xFF);
			//! @brief Parse the instruction at the program counter given to have human readable information.
			DisassembledInstruction _parseInstruction(uint24_t pc, DisassemblyContext &ctx) const;
			//! @brief Get the parameter of the instruction as an hexadecimal string.
			std::string _getInstructionParameter(const ComSquare::CPU::Instruction &instruction,
												 uint24_t pc,
			                                     DisassemblyContext &ctx) const;
			//! @brief Update the register's panel (accumulator, stack pointer...)
			void _updateRegistersPanel();

			//! @brief Return a printable string corresponding to the value of a 8 or 16 bits immediate addressing mode.
			//! @param dual Set this to true if the instruction take 16bits and not 8. (used for the immediate by a when the flag m is not set or the immediate by x if the flag x is not set).
			[[nodiscard]] std::string _getImmediateValue(uint24_t pc, bool dual) const;
			//! @brief Return a printable string corresponding to the value of an absolute addressing mode.
			[[nodiscard]] std::string _getAbsoluteValue(uint24_t pc) const;
			//! @brief Return a printable string corresponding to the value of an absolute long addressing mode.
			[[nodiscard]] std::string _getAbsoluteLongValue(uint24_t pc) const;
			//! @brief Return a printable string corresponding to the value of a direct addressing mode.
			[[nodiscard]] std::string _getDirectValue(uint24_t pc) const;
			//! @brief  Return a printable string corresponding to the value of a direct indirect addressing mode.
			[[nodiscard]] std::string _getDirectIndirectValue(uint24_t pc) const;
			//! @brief  Return a printable string corresponding to the value of a direct indirect long addressing mode.
			[[nodiscard]] std::string _getDirectIndirectLongValue(uint24_t pc) const;
			//! @brief  Return a printable string corresponding to the value of a absolute indexed by x addressing mode.
			[[nodiscard]] std::string _getAbsoluteIndexByXValue(uint24_t pc) const;
			//! @brief  Return a printable string corresponding to the value of a absolute indexed by x long addressing mode.
			[[nodiscard]] std::string _getAbsoluteIndexByXLongValue(uint24_t pc) const;
			//! @brief  Return a printable string corresponding to the value of a absolute indexed by y addressing mode.
			[[nodiscard]] std::string _getAbsoluteIndexByYValue(uint24_t pc) const;
			//! @brief Return a printable string corresponding to the value of a direct index by x addressing mode.
			[[nodiscard]] std::string _getDirectIndexedByXValue(uint24_t pc) const;
			//! @brief Return a printable string corresponding to the value of a direct index by y addressing mode.
			[[nodiscard]] std::string _getDirectIndexedByYValue(uint24_t pc) const;
			//! @brief Return a printable string corresponding to the value of a direct indirect index by x addressing mode.
			[[nodiscard]] std::string _getDirectIndexedByXIndirectValue(uint24_t pc) const;
			//! @brief Return a printable string corresponding to the value of a direct indirect index by y addressing mode.
			[[nodiscard]] std::string _getDirectIndirectIndexedByYValue(uint24_t pc) const;
			//! @brief Return a printable string corresponding to the value of a direct indirect index by y long addressing mode.
			[[nodiscard]] std::string _getDirectIndirectIndexedByYLongValue(uint24_t pc) const;
			//! @brief Return a printable string corresponding to the value of a stack relative addressing mode.
			[[nodiscard]] std::string _getStackRelativeValue(uint24_t pc) const;
			//! @brief Return a printable string corresponding to the value of a stack relative indirect indexed by y addressing mode.
			[[nodiscard]] std::string _getStackRelativeIndirectIndexedByYValue(uint24_t pc) const;
			//! @brief Return a printable string corresponding to the value of a absolute indirect addressing mode.
			[[nodiscard]] std::string _getAbsoluteIndirectValue(uint24_t pc) const;
			//! @brief Return a printable string corresponding to the value of a absolute indirect indexed by x addressing mode.
			[[nodiscard]] std::string _getAbsoluteIndirectIndexedByXValue(uint24_t pc) const;
			//! @brief Return a printable string corresponding to the value of a absolute indirect long addressing mode.
			[[nodiscard]] std::string _getAbsoluteIndirectLongValue(uint24_t pc) const;

		public slots:
			//! @brief Update the current debugger and the underlying CPU.
			unsigned update();
		public:
			//! @brief Show an error dialog related to an exception.
			static void showError(const DebuggableError &error);
			//! @brief Pause/Resume the CPU.
			//! @param forcePause True if you want to set the set the state of the CPU to paused. False if you want to toggle the pause status.
			void pause(bool forcePause = false);
			//! @brief Step - Execute a single instruction.
			void step();
			//! @brief Next - Continue running instructions until the next line is reached.
			void next();
			//! @brief Clear the history panel.
			void clearHistory();
			//! @brief Called when the user clicks on a section header. It enable/disable a breakpoint for this address.
			void toggleBreakpoint(int logicalIndex);
			//! @brief The list of disassembled instructions to show on the debugger.
			std::vector<DisassembledInstruction> disassembled;
			//! @brief The list of breakpoints the user has set.
			std::vector<Breakpoint> breakpoints;
			//! @brief Get a string representing the actual value of the arguments of the next instruction to execute.
			[[nodiscard]] std::string getProceededParameters() const;
			//! @brief Return the current program counter of this CPU.
			[[nodiscard]] uint24_t getPC() const;
			//! @brief Return the current stack pointer.
			[[nodiscard]] uint16_t getStackPointer() const;
			//! @brief The stack pointer before the execution of any instructions.
			uint16_t initialStackPointer;

			//! @brief Convert a basic CPU to a debugging CPU.
			CPUDebug(ComSquare::CPU::CPU &cpu, SNES &snes);
			CPUDebug(const CPUDebug &) = delete;
			CPUDebug &operator=(const CPUDebug &) = delete;
			~CPUDebug() override;

			//! @brief Focus the debugger's window.
			void focus();
		};
	}// namespace Debugger
}// namespace ComSquare
