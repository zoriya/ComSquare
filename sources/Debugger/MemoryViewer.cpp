//
// Created by anonymus-raccoon on 2/17/20.
//

#include <iostream>
#include <cmath>
#include <QtWidgets/QSpinBox>
#include <QMessageBox>
#include "MemoryViewer.hpp"
#include "SNES.hpp"
#include "Exceptions/InvalidAddress.hpp"
#include "Utility/Utility.hpp"

namespace ComSquare::Debugger
{
	MemoryViewerModel::MemoryViewerModel(Ram::Ram &memory, QObject *parent)
		: QAbstractTableModel(parent),
		  _memory(memory)
	{}

	int MemoryViewerModel::rowCount(const QModelIndex &) const
	{
		return static_cast<int>(this->_memory.get().getSize() / 16u);
	}

	int MemoryViewerModel::columnCount(const QModelIndex &parent) const
	{
		if (parent.row() == this->rowCount(parent) - 1)
			return static_cast<int>(this->_memory.get().getSize() - (parent.row() << 8u));
		return 16;
	}

	QVariant MemoryViewerModel::data(const QModelIndex &index, int role) const
	{
		if (role == Qt::TextAlignmentRole)
			return Qt::AlignCenter;
		if (role != Qt::DisplayRole)
			return QVariant();
		auto value = this->_memory.get().read((index.row() << 4u) + index.column());
		return QString(Utility::to_hex(value, Utility::NoPrefix).c_str());
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

	void MemoryViewerModel::setMemory(Ram::Ram &memory)
	{
		this->_memory = memory;
		this->_headerIndentSize = this->_memory.get().getSize() >= 0x10000 ? 4 : 3;
		emit this->layoutChanged();
	}

	MemoryViewer::MemoryViewer(ComSquare::SNES &snes, Memory::MemoryBus &bus)
		: _window(new ClosableWindow([&snes] { snes.disableRamViewer(); })),
		  _snes(snes),
		  _bus(bus),
		  _ui(),
		  _model(snes.wram)
	{
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
			this->_model.setMemory(this->_snes.ppu.cgram);
			break;
		case 4:
			this->_model.setMemory(this->_snes.ppu.vram);
			break;
		case 5:
			this->_model.setMemory(this->_snes.ppu.oamram);
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
			} catch (const InvalidAddress &) {
				QMessageBox msgBox;
				msgBox.setIcon(QMessageBox::Critical);
				msgBox.setText("This address is not mapped. Reading it will result in OpenBus.");
				msgBox.exec();
				return;
			}
		}
		QModelIndex index = this->_ui.tableView->model()->index(value >> 4, value & 0x0F);
		this->_ui.tableView->scrollTo(index);
		this->_ui.tableView->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
		this->_ui.tableView->setCurrentIndex(index);
	}

	unsigned MemoryViewer::switchToAddrTab(uint24_t addr)
	{
		Memory::IMemory *accessor = this->_bus.getAccessor(addr);
		if (!accessor)
			throw InvalidAddress("Memory viewer switch to address", addr);
		switch (accessor->getComponent()) {
		case WRam:
			this->_ui.tabs->setCurrentIndex(0);
			break;
		case SRam:
			this->_ui.tabs->setCurrentIndex(1);
			break;
		case Rom:
			this->_ui.tabs->setCurrentIndex(2);
			break;
		default:
			throw InvalidAddress("Memory viewer switch to address", addr);
		}
		addr = accessor->getRelativeAddress(addr);
		if (addr > accessor->getSize()) {
			QMessageBox msgBox;
			msgBox.setIcon(QMessageBox::Critical);
			msgBox.setText(
				(std::string("The ") + accessor->getName() + " is too small to contain this address.").c_str());
			msgBox.exec();
			return 0;
		}
		return addr;
	}

	void MemoryViewer::focus()
	{
		this->_window->activateWindow();
	}
}