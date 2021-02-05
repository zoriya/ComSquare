//
// Created by anonymus-raccoon on 5/28/20.
//

#include "RegisterViewer.hpp"
#include "../SNES.hpp"
#include "../Utility/Utility.hpp"

namespace ComSquare::Debugger
{
	RegisterViewer::RegisterViewer(SNES &snes)
		: _window(new ClosableWindow<RegisterViewer>(*this, &RegisterViewer::disableDebugger)),
		_ui(),
		_snes(snes)
	{
		this->_window->setContextMenuPolicy(Qt::NoContextMenu);
		this->_window->setAttribute(Qt::WA_QuitOnClose, false);
		this->_window->setAttribute(Qt::WA_DeleteOnClose);

		this->_ui.setupUi(this->_window);
		this->_setupUi();
		this->_window->show();
	}

	void RegisterViewer::_setupUi()
	{
		this->_models.clear();

		std::array<QTableView *, 8> channels = {
			this->_ui.dmaChannel1,
			this->_ui.dmaChannel2,
			this->_ui.dmaChannel3,
			this->_ui.dmaChannel4,
			this->_ui.dmaChannel5,
			this->_ui.dmaChannel6,
			this->_ui.dmaChannel7,
			this->_ui.dmaChannel8
		};
		for (int i = 0; i < 8; i++) {
			RegistersViewerModel *model = new RegistersViewerModel(this->_snes);
			model->addRegister(Register(0x420B, std::string(":") + std::to_string(i), "Enabled", [i](SNES &snes) {
				return snes.cpu->_dmaChannels[i].enabled;
			}, nullptr, Boolean));
			model->addRegister(Register(0x4302 + (i << 4u), "-4", "A address", [i](SNES &snes) {
				return snes.cpu->_dmaChannels[i]._aAddress.raw;
			}, nullptr, TwentyFourBits));
			model->addRegister(Register(0x4301 + (i << 4u), "", "B address", [i](SNES &snes) {
				return 0x2100 | snes.cpu->_dmaChannels[i]._port;
			}, nullptr, SixteenBits));
			model->addRegister(Register(0x4305 + (i << 4u), "-6", "Count", [i](SNES &snes) {
				return snes.cpu->_dmaChannels[i]._count.raw;
			}, nullptr, SixteenBits));
			model->addRegister(Register(0x4300 + (i << 4u), ":7", "B To A", [i](SNES &snes) {
				return snes.cpu->_dmaChannels[i]._controlRegister.direction;
			}, nullptr, Boolean));
			model->addRegister(Register(0x4300 + (i << 4u), ":3", "Fixed", [i](SNES &snes) {
				return snes.cpu->_dmaChannels[i]._controlRegister.fixed;
			}, nullptr, Boolean));
			model->addRegister(Register(0x4300 + (i << 4u), ":4", "Decrement", [i](SNES &snes) {
				return snes.cpu->_dmaChannels[i]._controlRegister.increment;
			}, nullptr, Boolean));
			model->addRegister(Register(0x4300 + (i << 4u), ":0-2", "Mode", [i](SNES &snes) {
				return snes.cpu->_dmaChannels[i]._controlRegister.increment;
			}, nullptr, EightBits));
			channels[i]->setModel(model);
			this->_models.push_back(model);
		}
	}

	void RegisterViewer::focus()
	{
		this->_window->activateWindow();
	}

	void RegisterViewer::disableDebugger()
	{
		this->_snes.disableRegisterDebugging();
	}

	RegisterViewer::~RegisterViewer()
	{
		for (auto &model : this->_models)
			delete model;
	}

	Register::Register(uint24_t addr,
		const std::string &usedBits,
		const std::string &regName,
		const std::function<unsigned int(SNES &)> &getValue,
		const std::function<void(SNES &, unsigned int)> &setValue,
		RegisterType regType)
			: address(addr),
			bits(usedBits),
			name(regName),
			get(getValue),
			set(setValue),
			type(regType) {}
}

using namespace ComSquare;
using namespace ComSquare::Debugger;

RegistersViewerModel::RegistersViewerModel(SNES &snes, QObject *parent) : QAbstractTableModel(parent), _snes(snes) { }


void RegistersViewerModel::addRegister(Register reg)
{
	int row = this->_registers.size();
	this->beginInsertRows(QModelIndex(), row, row);
	this->_registers.push_back(reg);
	this->insertRow(row);
	this->endInsertRows();
}

int RegistersViewerModel::rowCount(const QModelIndex &) const
{
	return this->_registers.size();
}

int RegistersViewerModel::columnCount(const QModelIndex &) const
{
	return 3;
}

QVariant RegistersViewerModel::data(const QModelIndex &index, int role) const
{
	Register reg = this->_registers[index.row()];

	if (role == Qt::CheckStateRole && reg.type == Boolean && index.column() == 2)
		return reg.get(this->_snes) ? Qt::Checked : Qt::Unchecked;

	if (role != Qt::DisplayRole)
		return QVariant();

	switch (index.column()) {
	case 0:
		return QString((Utility::to_hex(reg.address) + reg.bits).c_str());
	case 1:
		return QString(reg.name.c_str());
	case 2:
		switch (reg.type) {
		case Boolean:
			return QString(reg.get(this->_snes) ? "True" : "False");
		case EightBits:
			return QString(Utility::to_hex(static_cast<uint8_t>(reg.get(this->_snes))).c_str());
		case SixteenBits:
			return QString(Utility::to_hex(static_cast<uint16_t>(reg.get(this->_snes))).c_str());
		case TwentyFourBits:
			return QString(Utility::to_hex(static_cast<uint24_t>(reg.get(this->_snes))).c_str());
		}
	}
	return QVariant();
}

QVariant RegistersViewerModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Vertical || role != Qt::DisplayRole)
		return QVariant();
	switch (section) {
	case 0:
		return QString("Address");
	case 1:
		return QString("Name");
	case 2:
		return QString("Value");
	default:
		return QVariant();
	}
}