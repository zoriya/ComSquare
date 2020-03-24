//
// Created by anonymus-raccoon on 3/20/20.
//

#ifndef COMSQUARE_MEMORYBUSDEBUG_HPP
#define COMSQUARE_MEMORYBUSDEBUG_HPP

#include <QtWidgets/QMainWindow>
#include "../Memory/MemoryBus.hpp"
#include "../../ui/ui_busView.h"
#include "ClosableWindow.hpp"

namespace ComSquare::Debugger
{
	//! @brief The struct used to represent memory bus logs.
	struct BusLog {
		BusLog(bool write, uint24_t addr, std::shared_ptr<Memory::AMemory> &accessor, uint8_t oldData, uint8_t newData);

		bool write;
		uint24_t addr;
		std::shared_ptr<Memory::AMemory> accessor;
		uint8_t oldData;
		uint8_t newData;
	};
}


//! @brief The qt model that bind the logs to the view.
class BusLogModel : public QAbstractTableModel
{
Q_OBJECT
private:
	//! @brief The logs to display.
	std::vector<ComSquare::Debugger::BusLog> _logs;
	//! @brief The number of column;
	const int _columnCount = 6;
public:
	BusLogModel() = default;
	BusLogModel(const BusLogModel &) = delete;
	const BusLogModel &operator=(const BusLogModel &) = delete;
	~BusLogModel() override = default;

	//! @brief Add a log to the model
	void log(ComSquare::Debugger::BusLog log);

	//! @brief The number of row the table has.
	int rowCount(const QModelIndex &parent) const override;
	//! @brief The number of column the table has.
	int columnCount(const QModelIndex &parent) const override;
	//! @brief Return a data representing the table cell.
	QVariant data(const QModelIndex &index, int role) const override;
	//! @brief Override the headers to use hex values.
	QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
};


namespace ComSquare::Debugger
{
	//! @brief window that allow the user to view all data going through the memory bus.
	class MemoryBusDebug : public Memory::MemoryBus {
	private:
		//! @brief The QT window for this debugger.
		ClosableWindow<MemoryBusDebug> *_window;
		//! @brief A reference to the snes (to disable the debugger).
		SNES &_snes;
		//! @brief A widget that contain the whole UI.
		Ui::BusView _ui;
		//! @brief The Log visualizer model for QT.
		BusLogModel _model;
	public:
		//! @brief Called when the window is closed. Turn off the debugger and revert to a basic CPU.
		void disableViewer();
	public:
		explicit MemoryBusDebug(SNES &snes, const Memory::MemoryBus &bus);
		MemoryBusDebug(const MemoryBusDebug &) = delete;
		MemoryBusDebug &operator=(const MemoryBusDebug &) = delete;
		~MemoryBusDebug() = default;

		//! @brief Read data at a global address and log it to the debugger.
		//! @param addr The address to read from.
		//! @return The value that the component returned for this address. If the address was mapped to ram, it simply returned the value. If the address was mapped to a register the component returned the register.
		uint8_t read(uint24_t addr) override;

		//! @brief Write a data to a global address and log it to the debugger.
		//! @param addr The address to write to.
		//! @param data The data to write.
		void write(uint24_t addr, uint8_t data) override;

		//! @brief Focus the debugger's window.
		void focus();

		//! @brief Return true if the Bus is overloaded with debugging features.
		bool isDebugger() override;
	};
}

#endif //COMSQUARE_MEMORYBUSDEBUG_HPP
