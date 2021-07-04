//
// Created by anonymus-raccoon on 3/20/20.
//

#pragma once

#include <QtWidgets/QMainWindow>
#include <QtCore/QSortFilterProxyModel>
#include "Memory/MemoryBus.hpp"
#include "Memory/IMemory.hpp"
#include "ui/ui_busView.h"
#include "ClosableWindow.hpp"
#include <optional>

namespace ComSquare::Debugger
{
	//! @brief The struct used to represent memory bus logs.
	struct BusLog
	{
		BusLog(bool write,
		       uint24_t addr,
		       Memory::IMemory *accessor,
		       std::optional<uint8_t> oldData,
		       uint8_t newData);

		bool write;
		uint24_t addr;
		Memory::IMemory *accessor;
		std::optional<uint8_t> oldData;
		uint8_t newData;
	};

	//! @brief The struct representing filters of the memory bus's logger.
	struct BusLoggerFilters
	{
		bool cpu = true;
		bool apu = true;
		bool ppu = true;
		bool rom = true;
		bool wram = true;
		bool sram = true;
		bool vram = true;
		bool oamram = true;
		bool cgram = true;
	};

	//! @brief The qt model that bind the logs to the view.
	class BusLogModel : public QAbstractTableModel
	{
	Q_OBJECT
	private:
		//! @brief The logs to display.
		std::vector<ComSquare::Debugger::BusLog> _logs;
	public:
		BusLogModel() = default;
		BusLogModel(const BusLogModel &) = delete;
		const BusLogModel &operator=(const BusLogModel &) = delete;
		~BusLogModel() override = default;

		//! @brief The number of column;
		const int column = 6;

		//! @brief Add a log to the model
		void log(const ComSquare::Debugger::BusLog &log);

		//! @brief Get a log at an index.
		ComSquare::Debugger::BusLog getLogAt(int index);
		//! @brief Clear all the logs
		void clearLogs();

		//! @brief The number of row the table has.
		[[nodiscard]] int rowCount(const QModelIndex &parent) const override;
		//! @brief The number of column the table has.
		[[nodiscard]] int columnCount(const QModelIndex &parent) const override;
		//! @brief Return a data representing the table cell.
		[[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
		//! @brief Override the headers to use hex values.
		[[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
	};

	//! @brief A class to filter logs from the memory bus's debugger.
	class BusLoggerProxy : public QSortFilterProxyModel
	{
	Q_OBJECT
	private:
		//! @brief The parent to get the original data for filters
		BusLogModel &_parent;
	protected:
		//! @brief Function that filter logs.
		[[nodiscard]] bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
	public:
		//! @brief Currently enabled filters, index 0 is for reads, index 1 for writes.
		ComSquare::Debugger::BusLoggerFilters filters[2] = {
			ComSquare::Debugger::BusLoggerFilters(),
			ComSquare::Debugger::BusLoggerFilters()
		};

		//! @brief Refresh the view after a change of filters.
		void refresh();

		explicit BusLoggerProxy(BusLogModel &parent);
		BusLoggerProxy(const BusLoggerProxy &) = delete;
		const BusLoggerProxy &operator=(const BusLoggerProxy &) = delete;
		~BusLoggerProxy() override = default;
	};

	//! @brief window that allow the user to view all data going through the memory bus.
	class MemoryBusDebug : public Memory::IMemoryBus
	{
	private:
		//! @brief The QT window for this debugger.
		ClosableWindow *_window;
		//! @brief A reference to the underlying bus..
		Memory::IMemoryBus &_bus;
		//! @brief A widget that contain the whole UI.
		Ui::BusView _ui;
		//! @brief The Log visualizer model for QT.
		BusLogModel _model;
		//! @brief A QT proxy to filter the logs.
		BusLoggerProxy _proxy;
	public:
		explicit MemoryBusDebug(SNES &snes, Memory::IMemoryBus &bus);
		MemoryBusDebug(const MemoryBusDebug &) = delete;
		MemoryBusDebug &operator=(const MemoryBusDebug &) = delete;
		~MemoryBusDebug() override = default;

		//! @brief Read data at a global address. This form allow read to be silenced.
		//! @param addr The address to read from.
		//! @throws InvalidAddress If the address is not mapped to the bus, this exception is thrown.
		//! @return The value that the component returned for this address. If the address was mapped to ram, it simply returned the value. If the address was mapped to a register the component returned the register.
		uint8_t read(uint24_t addr) override;

		//! @brief This as the same purpose as a read but it does not change the open bus and won't throw an exception.
		//! @param addr The address to read from.
		//! @return The value that the component returned for this address. If the address was mapped to ram, it simply returned the value. If the address was mapped to a register the component returned the register.
		std::optional<uint8_t> peek(uint24_t addr) override;

		//! @brief Write a data to a global address.
		//! @param addr The address to write to.
		//! @param data The data to write.
		void write(uint24_t addr, uint8_t data) override;

		//! @brief Helper function to get the components that is responsible of read/write at an address.
		//! @param addr The address you want to look for.
		//! @return The components responsible for the address param or nullptr if none was found.
		Memory::IMemory *getAccessor(uint24_t addr) override;

		//! @brief Focus the debugger's window.
		void focus();
	};
}
