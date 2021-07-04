//
// Created by anonymus-raccoon on 2/17/20.
//

#pragma once

#include <QtWidgets/QMainWindow>
#include "ui/ui_ramView.h"
#include "ui/ui_gotoDialog.h"
#include "Ram/Ram.hpp"
#include "Memory/MemoryBus.hpp"
#include "ClosableWindow.hpp"
#include <memory>
#include <QAbstractTableModel>

namespace ComSquare
{
	class SNES;
	namespace Debugger
	{
		//! @brief The qt model that bind the ram to the view.
		class MemoryViewerModel : public QAbstractTableModel
		{
		Q_OBJECT
		private:
			//! @brief The ram to watch.
			std::reference_wrapper<Ram::Ram> _memory;
			//! @brief The number of char inside the left header number.
			int _headerIndentSize = 3;
		public:
			//! @brief Change the ram currently watched.
			void setMemory(Ram::Ram &memory);

			explicit MemoryViewerModel(Ram::Ram &memory, QObject *parent = nullptr);
			MemoryViewerModel(const MemoryViewerModel &) = delete;
			const MemoryViewerModel &operator=(const MemoryViewerModel &) = delete;
			~MemoryViewerModel() override = default;

			//! @brief The number of row the table has.
			[[nodiscard]] int rowCount(const QModelIndex &parent) const override;
			//! @brief The number of column the table has.
			[[nodiscard]] int columnCount(const QModelIndex &parent) const override;
			//! @brief Return a data representing the table cell.
			[[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
			//! @brief Override the headers to use hex values.
			[[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
		};

		//! @brief Class responsible of the Memory Viewer.
		class MemoryViewer : public QObject
		{
		Q_OBJECT
		private:
			//! @brief The QT window for this debugger.
			ClosableWindow *_window;
			//! @brief SNES containing all rams to view.
			SNES &_snes;
			//! @brief The memory bus used to get the view for a given address.
			Memory::MemoryBus &_bus;
			//! @brief The layout of the viewer.
			Ui::RamView _ui;
			//! @brief The Ram visualizer model for QT.
			MemoryViewerModel _model;

			//! @brief Helper function to create the goto dialog.
			void _internalGoto(bool isAbsolute);
		public:
			//! @brief Select the memory tab corresponding to a 24 bit address (map the address via the bus).
			//! @return The address converted to the new tab's locale space.
			unsigned switchToAddrTab(uint24_t addr);

			//! @brief Callback called when a memory tab is selected.
			void changeRam(int id);
			//! @brief Create a popup asking you where you want to jump to.
			void gotoAddr();
			//! @brief Create a popup asking you where you want to jump to with the absolute mode selected.
			void gotoAbsoluteAddr();
			//! @brief Focus the memory viewer's window.
			void focus();

			explicit MemoryViewer(SNES &snes, Memory::MemoryBus &bus);
			MemoryViewer(const MemoryViewer &) = delete;
			MemoryViewer &operator=(const MemoryViewer &) = delete;
			~MemoryViewer() override = default;
		};
	}
}