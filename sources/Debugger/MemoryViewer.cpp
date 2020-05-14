//
// Created by anonymus-raccoon on 2/17/20.
//

#include <iostream>
#include <cmath>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QSpinBox>
#include "MemoryViewer.hpp"
#include "../SNES.hpp"
#include "../Memory/MemoryShadow.hpp"
#include "../Exceptions/InvalidAddress.hpp"

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
	if (role == Qt::TextAlignmentRole)
		return Qt::AlignCenter;
	if (role != Qt::DisplayRole)
		return QVariant();
	char buf[3];
	snprintf(buf, 3, "%02X", this->_memory->read_internal((index.row() << 4u) + index.column()));
	return QString(buf);
}

QVariant MemoryViewerModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role != Qt::DisplayRole)
		return QVariant();
	if (orientation == Qt::Horizontal) {
		char buf[2];
		snprintf(buf, 2, "%1X", section);
		return QString(buf);
	} else {
		char buf[6];
		snprintf(buf, 6, "%0*Xx", this->_headerIndentSize, section);
		return QString(buf);
	}
}

void MemoryViewerModel::setMemory(std::shared_ptr<Ram> memory)
{
	this->_memory = std::move(memory);
	this->_headerIndentSize = this->_memory->getSize() >= 0x10000 ? 4 : 3;
	emit this->layoutChanged();
}

namespace ComSquare::Debugger
{
	MemoryViewer::MemoryViewer(ComSquare::SNES &snes, Memory::MemoryBus &bus) :
		_window(new ClosableWindow<MemoryViewer>(*this, &MemoryViewer::disableViewer)),
		_snes(snes),
		_bus(bus),
		_ui(),
		_model(snes.wram)
	{
		this->_window->setContextMenuPolicy(Qt::NoContextMenu);
		this->_window->setAttribute(Qt::WA_QuitOnClose, false);
		this->_window->setAttribute(Qt::WA_DeleteOnClose);

		this->_ui.setupUi(this->_window);
		this->_ui.tableView->setModel(&this->_model);
		this->_ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		this->_ui.tabs->addTab("&WRam");
		this->_ui.tabs->addTab("&SRam");
		this->_ui.tabs->addTab("&Rom");
		this->_ui.tabs->addTab("&CGRam");
		this->_ui.tabs->addTab("&VRam");
		this->_ui.tabs->addTab("&OAMRam");
		QMainWindow::connect(this->_ui.actionGoto, &QAction::triggered, this, &MemoryViewer::gotoAddr);
		QMainWindow::connect(this->_ui.actionGoto_Absolute, &QAction::triggered, this, &MemoryViewer::gotoAbsoluteAddr);
		QObject::connect(this->_ui.tabs, &QTabBar::currentChanged, this, &MemoryViewer::changeRam);
		this->_window->show();
	}

	void MemoryViewer::disableViewer()
	{
		this->_snes.disableRamViewer();
	}

	void MemoryViewer::changeRam(int id)
	{
		switch (id) {
		default:
		case 0:
			this->_model.setMemory(this->_snes.wram);
			break;
		case 1:
			this->_model.setMemory(this->_snes.sram);
			break;
		case 2:
			this->_model.setMemory(this->_snes.cartridge);
			break;
		case 3:
			this->_model.setMemory(this->_snes.ppu->cgram);
			break;
		case 4:
			this->_model.setMemory(this->_snes.ppu->vram);
			break;
		case 5:
			this->_model.setMemory(this->_snes.ppu->oamram);
			break;
		}
	}

	void MemoryViewer::gotoAddr()
	{
		this->_internalGoto(false);
	}

	void MemoryViewer::gotoAbsoluteAddr()
	{
		this->_internalGoto(true);
	}

	void MemoryViewer::_internalGoto(bool isAbsolute)
	{
		QDialog dialog(this->_window);
		dialog.setWindowModality(Qt::WindowModal);
		Ui::GotoDialog dialogUI;
		dialogUI.setupUi(&dialog);
		QFont font = dialogUI.spinBox->font();
		font.setCapitalization(QFont::AllUppercase);
		dialogUI.spinBox->setFont(font);
		dialogUI.spinBox->selectAll();
		dialogUI.checkBox->setChecked(isAbsolute);

		if (dialog.exec() != QDialog::Accepted)
			return;
		long value = std::strtol(dialogUI.spinBox->text().toStdString().c_str() + 1, nullptr, 16);
		if (dialogUI.checkBox->isChecked()) {
			try {
				value = this->switchToAddrTab(value);
			} catch (InvalidAddress &) {}
		}
		QModelIndex index = this->_ui.tableView->model()->index(value >> 4, value & 0x0000000F);
		this->_ui.tableView->scrollTo(index);
		this->_ui.tableView->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
	}

	unsigned MemoryViewer::switchToAddrTab(uint24_t addr)
	{
		std::shared_ptr<Memory::AMemory> accessor = this->_bus.getAccessor(addr);
		if (!accessor)
			throw InvalidAddress("Memory viewer switch to address", addr);
		Memory::AMemory *ptr;
		if (accessor->isMirror())
			ptr = accessor->getMirrored().get();
		else
			ptr = accessor.get();

		if (ptr == this->_snes.wram.get())
			this->_ui.tabs->setCurrentIndex(0);
		else if (ptr == this->_snes.sram.get())
			this->_ui.tabs->setCurrentIndex(1);
		else if (ptr == this->_snes.cartridge.get())
			this->_ui.tabs->setCurrentIndex(2);
		else
			throw InvalidAddress("Memory viewer switch to address", addr);
		return addr - accessor->getStart();
	}

	void MemoryViewer::focus()
	{
		this->_window->activateWindow();
	}
}