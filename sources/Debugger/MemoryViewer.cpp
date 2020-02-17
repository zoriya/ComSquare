//
// Created by anonymus-raccoon on 2/17/20.
//

#include <iostream>
#include "MemoryViewer.hpp"
#include "../SNES.hpp"
#include "../Utility/Utility.hpp"

MemoryViewerModel::MemoryViewerModel(std::shared_ptr<Ram> memory, QObject *parent) :
	QAbstractTableModel(parent),
	_memory(std::move(memory))
{ }

int MemoryViewerModel::rowCount(const QModelIndex &) const
{
	return this->_memory->getSize() / 16u;
}

int MemoryViewerModel::columnCount(const QModelIndex &parent) const
{
	if (parent.row() == this->rowCount(parent) - 1)
		return this->_memory->getSize() - (parent.row() << 8u);
	return 16;
}

QVariant MemoryViewerModel::data(const QModelIndex &index, int role) const
{
	if (role != Qt::DisplayRole)
		return QVariant();
	return QString(ComSquare::Utility::to_hex(this->_memory->read_internal((index.row() << 4u) + index.column())).c_str());
}

QVariant MemoryViewerModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role != Qt::DisplayRole)
		return QVariant();
	if (orientation == Qt::Horizontal)
		return QString(ComSquare::Utility::to_hex(static_cast<uint8_t>(section)).c_str());
	else
		return QString(ComSquare::Utility::to_hex(static_cast<uint16_t>(section)).c_str());
}


namespace ComSquare::Debugger
{
	MemoryViewer::MemoryViewer(ComSquare::SNES &snes) :
		QMainWindow(), _snes(snes), _ui(), _model(snes.wram)
	{
		this->setContextMenuPolicy(Qt::NoContextMenu);
		this->setAttribute(Qt::WA_QuitOnClose, false);

		this->_ui.setupUi(this);
		this->_ui.tableView->setModel(&this->_model);
		this->_ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		this->show();
	}
}