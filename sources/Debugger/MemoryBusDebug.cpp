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
		_window(new ClosableWindow<MemoryBusDebug>(*this, &MemoryBusDebug::disableViewer)),
		_snes(snes),
		_ui(),
		_model(),
		_proxy(this->_model)
	{
		this->_window->setContextMenuPolicy(Qt::NoContextMenu);
		this->_window->setAttribute(Qt::WA_QuitOnClose, false);
		this->_window->setAttribute(Qt::WA_DeleteOnClose);

		this->_ui.setupUi(this->_window);
		this->_proxy.setSourceModel(&this->_model);
		this->_ui.log->setModel(&this->_proxy);
		this->_ui.log->setAlternatingRowColors(true);
		this->_ui.log->verticalHeader()->hide();
		this->_ui.log->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
		this->_ui.log->horizontalHeader()->setStretchLastSection(true);
		this->_ui.log->horizontalHeader()->setSectionsMovable(true);
		for (int i = 0; i < this->_model.column; i++)
			this->_ui.log->setColumnWidth(i, this->_ui.log->width());

		QMainWindow::connect(this->_ui.fromAPU, &QCheckBox::toggled, [this](bool checked) {
			this->_proxy.filters[0].apu = checked;
			this->_proxy.refresh();
		});
		QMainWindow::connect(this->_ui.fromCPU, &QCheckBox::toggled, [this](bool checked) {
			this->_proxy.filters[0].cpu = checked;
			this->_proxy.refresh();
		});
		QMainWindow::connect(this->_ui.fromOAM, &QCheckBox::toggled, [this](bool checked) {
			this->_proxy.filters[0].oamram = checked;
			this->_proxy.refresh();
		});
		QMainWindow::connect(this->_ui.fromPPU, &QCheckBox::toggled, [this](bool checked) {
			this->_proxy.filters[0].ppu = checked;
			this->_proxy.refresh();
		});
		QMainWindow::connect(this->_ui.fromROM, &QCheckBox::toggled, [this](bool checked) {
			this->_proxy.filters[0].rom = checked;
			this->_proxy.refresh();
		});
		QMainWindow::connect(this->_ui.fromSRAM, &QCheckBox::toggled, [this](bool checked) {
			this->_proxy.filters[0].sram = checked;
			this->_proxy.refresh();
		});
		QMainWindow::connect(this->_ui.fromVRAM, &QCheckBox::toggled, [this](bool checked) {
			this->_proxy.filters[0].vram = checked;
			this->_proxy.refresh();
		});
		QMainWindow::connect(this->_ui.fromWRAM, &QCheckBox::toggled, [this](bool checked) {
			this->_proxy.filters[0].wram = checked;
			this->_proxy.refresh();
		});
		QMainWindow::connect(this->_ui.fromCG, &QCheckBox::toggled, [this](bool checked) {
			this->_proxy.filters[0].cgram = checked;
			this->_proxy.refresh();
		});
		QMainWindow::connect(this->_ui.fromToggle, &QPushButton::pressed, [this]() {
			this->_ui.fromWRAM->setChecked(!this->_proxy.filters[0].wram);
			this->_ui.fromCPU->setChecked(!this->_proxy.filters[0].cpu);
			this->_ui.fromAPU->setChecked(!this->_proxy.filters[0].apu);
			this->_ui.fromPPU->setChecked(!this->_proxy.filters[0].ppu);
			this->_ui.fromVRAM->setChecked(!this->_proxy.filters[0].vram);
			this->_ui.fromOAM->setChecked(!this->_proxy.filters[0].oamram);
			this->_ui.fromCG->setChecked(!this->_proxy.filters[0].cgram);
			this->_ui.fromROM->setChecked(!this->_proxy.filters[0].rom);
			this->_ui.fromSRAM->setChecked(!this->_proxy.filters[0].sram);
		});

		QMainWindow::connect(this->_ui.toAPU, &QCheckBox::toggled, [this](bool checked) {
			this->_proxy.filters[1].apu = checked;
			this->_proxy.refresh();
		});
		QMainWindow::connect(this->_ui.toCPU, &QCheckBox::toggled, [this](bool checked) {
			this->_proxy.filters[1].cpu = checked;
			this->_proxy.refresh();
		});
		QMainWindow::connect(this->_ui.toOAM, &QCheckBox::toggled, [this](bool checked) {
			this->_proxy.filters[1].oamram = checked;
			this->_proxy.refresh();
		});
		QMainWindow::connect(this->_ui.toPPU, &QCheckBox::toggled, [this](bool checked) {
			this->_proxy.filters[1].ppu = checked;
			this->_proxy.refresh();
		});
		QMainWindow::connect(this->_ui.toSRAM, &QCheckBox::toggled, [this](bool checked) {
			this->_proxy.filters[1].sram = checked;
			this->_proxy.refresh();
		});
		QMainWindow::connect(this->_ui.toVRAM, &QCheckBox::toggled, [this](bool checked) {
			this->_proxy.filters[1].vram = checked;
			this->_proxy.refresh();
		});
		QMainWindow::connect(this->_ui.toWRAM, &QCheckBox::toggled, [this](bool checked) {
			this->_proxy.filters[1].wram = checked;
			this->_proxy.refresh();
		});
		QMainWindow::connect(this->_ui.toCG, &QCheckBox::toggled, [this](bool checked) {
			this->_proxy.filters[1].cgram = checked;
			this->_proxy.refresh();
		});

		QMainWindow::connect(this->_ui.clearBtn, &QPushButton::pressed, [this]() {
			this->_model.clearLogs();
			this->_proxy.refresh();
		});
		QMainWindow::connect(this->_ui.toToggle, &QPushButton::pressed, [this]() {
			this->_ui.toWRAM->setChecked(!this->_proxy.filters[1].wram);
			this->_ui.toCPU->setChecked(!this->_proxy.filters[1].cpu);
			this->_ui.toAPU->setChecked(!this->_proxy.filters[1].apu);
			this->_ui.toPPU->setChecked(!this->_proxy.filters[1].ppu);
			this->_ui.toVRAM->setChecked(!this->_proxy.filters[1].vram);
			this->_ui.toOAM->setChecked(!this->_proxy.filters[1].oamram);
			this->_ui.toCG->setChecked(!this->_proxy.filters[1].cgram);
			this->_ui.toROM->setChecked(!this->_proxy.filters[1].rom);
			this->_ui.toSRAM->setChecked(!this->_proxy.filters[1].sram);
		});

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

	uint8_t MemoryBusDebug::read(uint24_t addr, bool silence)
	{
		if (!silence && !forceSilence) {
			auto accessor = this->getAccessor(addr);
			if (!accessor) {
				this->_model.log(BusLog(true, addr, accessor, this->_openBus, this->_openBus));
			} else {
				uint8_t value = accessor->read(addr - accessor->getStart());
				this->_model.log(BusLog(false, addr, accessor, value, value));
			}
		}
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
		if (!forceSilence)
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
	return this->column;
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

void BusLogModel::log(const ComSquare::Debugger::BusLog& log)
{
	int row = this->_logs.size();
	this->beginInsertRows(QModelIndex(), row, row);
	this->_logs.push_back(log);
	this->insertRow(row);
	this->endInsertRows();
}

ComSquare::Debugger::BusLog BusLogModel::getLogAt(int index)
{
	return this->_logs[index];
}

void BusLogModel::clearLogs()
{
	this->beginResetModel();
	this->_logs.clear();
	this->endResetModel();
}

BusLoggerProxy::BusLoggerProxy(BusLogModel &parent) : QSortFilterProxyModel(), _parent(parent) {}

bool BusLoggerProxy::filterAcceptsRow(int sourceRow, const QModelIndex &) const
{
	ComSquare::Debugger::BusLog log = this->_parent.getLogAt(sourceRow);

	if (!log.accessor)
		return true;
	ComSquare::Component component = log.accessor->getComponent();
	switch (component) {
	case ComSquare::Component::Cpu:
		return this->filters[log.write].cpu;
	case ComSquare::Component::Ppu:
		return this->filters[log.write].ppu;
	case ComSquare::Component::Apu:
		return this->filters[log.write].apu;
	case ComSquare::Component::Rom:
		return this->filters[log.write].rom;
	case ComSquare::Component::WRam:
		return this->filters[log.write].wram;
	case ComSquare::Component::VRam:
		return this->filters[log.write].vram;
	case ComSquare::Component::CGRam:
		return this->filters[log.write].cgram;
	case ComSquare::Component::OAMRam:
		return this->filters[log.write].oamram;
	case ComSquare::Component::SRam:
		return this->filters[log.write].sram;
	default:
		return true;
	}
}

void BusLoggerProxy::refresh()
{
	this->invalidateFilter();
}
