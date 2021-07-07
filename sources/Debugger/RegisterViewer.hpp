//
// Created by anonymus-raccoon on 5/28/20.
//

#pragma once

#include "ClosableWindow.hpp"
#include "Memory/MemoryBus.hpp"
#include "Models/Ints.hpp"
#include "ui/ui_registersView.h"
#include <QAbstractTableModel>
#include <QtCore/QObject>

namespace ComSquare
{
	class SNES;

	namespace Debugger
	{
		struct Register;

		class RegistersViewerModel : public QAbstractTableModel
		{
		Q_OBJECT
		private:
			//! @brief The list of registers to display / update.
			std::vector<ComSquare::Debugger::Register> _registers;
			//! @brief Reference to the snes to get information from registers.
			ComSquare::SNES &_snes;
		public:
			//! @brief Add a register.
			void addRegister(const ComSquare::Debugger::Register &reg);

			explicit RegistersViewerModel(ComSquare::SNES &snes, QObject *parent = nullptr);
			RegistersViewerModel(const RegistersViewerModel &) = delete;
			const RegistersViewerModel &operator=(const RegistersViewerModel &) = delete;
			~RegistersViewerModel() override = default;

			//! @brief The number of row the table has.
			[[nodiscard]] int rowCount(const QModelIndex &parent) const override;
			//! @brief The number of column the table has.
			[[nodiscard]] int columnCount(const QModelIndex &parent) const override;
			//! @brief Return a data representing the table cell.
			[[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
			//! @brief Override the headers to use hex values.
			[[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
		};

		class RegisterViewer : public QObject
		{
		private:
			//! @brief The QT window for this debugger.
			ClosableWindow *_window;
			//! @brief A widget that contain the whole UI.
			Ui::RegistersView _ui;
			//! @brief The list of models used by different panels.
			std::vector<RegistersViewerModel *> _models;

			//! @brief The snes instance to read/write to.
			SNES &_snes;

			//! @brief Set models to the different tables and initialize them.
			void _setupUi();
		public:
			explicit RegisterViewer(SNES &snes);
			RegisterViewer(const RegisterViewer &) = delete;
			RegisterViewer &operator=(const RegisterViewer &) = delete;
			~RegisterViewer() override;

			//! @brief Focus the debugger's window.
			void focus();
		};

		//! @brief The types of registers
		enum RegisterType
		{
			//! @brief This type display a checkbox
			Boolean,
			//! @brief A 8 bits hexadecimal value.
			EightBits,
			//! @brief A 16 bits hexadecimal value.
			SixteenBits,
			//! @brief A 24 bits hexadecimal value.
			TwentyFourBits,
			//! @brief Just like printf(%d, myVar);
			Integer,
			//! @brief a simple string
			String
		};

		//! @brief Struct containing information about a register.
		struct Register
		{
			Register(uint24_t addr,
			         std::string usedBits,
			         std::string regName,
			         std::function<unsigned int(SNES &)> getValue,
			         std::function<void(SNES &, unsigned int)> setValue,
			         RegisterType regType);

			//! @brief Where this register is located on the bus.
			uint24_t address;
			//! @brief Specify witch bits are concerned if not all bytes are concerned.
			std::string bits;
			//! @brief The name of this register.
			std::string name;
			//! @brief How to get this value.
			std::function<unsigned(SNES &)> get;
			//! @brief How to set this value.
			std::function<void(SNES &, unsigned)> set;
			//! @brief How this value should be displayed/asked for input.
			RegisterType type;
		};
	}// namespace Debugger
}// namespace ComSquare