//
// Created by anonymus-raccoon on 3/20/20.
//

#include "MemoryBusDebug.hpp"
#include "../SNES.hpp"
#include "../Utility/Utility.hpp"
#include "../Exceptions/InvalidAction.hpp"
#include "../Exceptions/InvalidAddress.hpp"

namespace ComSquare::Debugger
{
	MemoryBusDebug::MemoryBusDebug(SNES &snes, const Memory::MemoryBus &bus)
		: MemoryBus(bus),
		_window(new ClosableWindow(*this, &MemoryBusDebug::disableViewer)),
		_snes(snes),
		_ui(),
		_model()
	{
		this->_window->setContextMenuPolicy(Qt::NoContextMenu);
		this->_window->setAttribute(Qt::WA_QuitOnClose, false);
		this->_window->setAttribute(Qt::WA_DeleteOnClose);

		this->_ui.setupUi(this->_window);
		this->_ui.log->setModel(&this->_model);
		this->_ui.log->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
		this->_ui.log->horizontalHeader()->setStretchLastSection(true);
		this->_ui.log->horizontalHeader()->setSectionsMovable(true);
		for (int i = 0; i < 5; i++)
			this->_ui.log->setColumnWidth(i, this->_ui.log->width());
		this->_window->show();
	}

	void MemoryBusDebug::disableViewer()
	{
		this->_snes.disableMemoryBusDebugging();
	}

	void MemoryBusDebug::focus()
	{
		this->_window->activateWindow();
	}

	bool MemoryBusDebug::isDebugger()
	{
		return true;
	}

	uint8_t MemoryBusDebug::read(uint24_t addr)
	{
		auto accessor = this->getAccessor(addr);
		uint8_t value = accessor->read(addr - accessor->getStart());
		this->_model.log(BusLog(false, addr, accessor, value, value));

		return MemoryBus::read(addr);
	}

	void MemoryBusDebug::write(uint24_t addr, uint8_t data)
	{
		auto accessor = this->getAccessor(addr);
		uint8_t value;
		try {
			value = accessor->read(addr - accessor->getStart());
		} catch (InvalidAddress &) {
			value = 0;
		}
		this->_model.log(BusLog(true, addr, accessor, value, data));
		MemoryBus::write(addr, data);
	}

	BusLog::BusLog(bool _write, uint24_t _addr, std::shared_ptr<Memory::AMemory> &_accessor, uint8_t _oldData, uint8_t _newData) :
		write(_write), addr(_addr), accessor(std::move(_accessor)), oldData(_oldData), newData(_newData)
	{}
}

int BusLogModel::rowCount(const QModelIndex &) const
{
	return this->_logs.size();
}

int BusLogModel::columnCount(const QModelIndex &) const
{
	return this->_columnCount;
}

QVariant BusLogModel::data(const QModelIndex &index, int role) const
{
	if (role == Qt::TextAlignmentRole)
		return Qt::AlignCenter;
	if (role != Qt::DisplayRole)
		return QVariant();
	ComSquare::Debugger::BusLog log = this->_logs[index.row()];
	switch (index.column()) {
	case 0:
		return QString(log.write ? "Write" : "Read");
	case 1:
		return QString(ComSquare::Utility::to_hex(log.addr).c_str());
	case 2:
		return QString(log.accessor ? log.accessor->getName().c_str() : "Bus");
	case 3:
		return QString(log.accessor ? log.accessor->getValueName(log.addr - log.accessor->getStart()).c_str() : "Open bus");
	case 4:
		return QString(ComSquare::Utility::to_hex(log.oldData).c_str());
	case 5:
		return QString(ComSquare::Utility::to_hex(log.newData).c_str());
	default:
		return QVariant();
	}
}

QVariant BusLogModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role != Qt::DisplayRole || orientation == Qt::Vertical)
		return QVariant();
	switch (section) {
	case 0:
		return QString("Type");
	case 1:
		return QString("Address");
	case 2:
		return QString("Component");
	case 3:
		return QString("Data Name");
	case 4:
		return QString("Old Data");
	case 5:
		return QString("New Data");
	default:
		return QString("");
	}
}

void BusLogModel::log(ComSquare::Debugger::BusLog log)
{
	this->_logs.push_back(log);
	int row = this->_logs.size();
	this->insertRow(row);
	emit this->layoutChanged();
	// The row may be inserted but items are not displayed.
}
