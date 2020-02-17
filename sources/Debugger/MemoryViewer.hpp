//
// Created by anonymus-raccoon on 2/17/20.
//

#ifndef COMSQUARE_MEMORYVIEWER_HPP
#define COMSQUARE_MEMORYVIEWER_HPP

#include <QtWidgets/QMainWindow>
#include "../../ui/ui_ramView.h"
#include "../Ram/Ram.hpp"

using ComSquare::Ram::Ram;

//! @brief The qt model that bind the ram to the view.
class MemoryViewerModel : public QAbstractTableModel
{
Q_OBJECT
private:
	std::shared_ptr<Ram> _memory;
public:
	explicit MemoryViewerModel(std::shared_ptr<Ram> memory, QObject *parent = nullptr);
	MemoryViewerModel(const MemoryViewerModel &) = delete;
	const MemoryViewerModel &operator=(const MemoryViewerModel &) = delete;
	~MemoryViewerModel() override = default;
	int rowCount(const QModelIndex &parent) const override;
	int columnCount(const QModelIndex &parent) const override;
	QVariant data(const QModelIndex &index, int role) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
};


namespace ComSquare
{
	class SNES;
	namespace Debugger
	{
		//! @brief Class responsible of the Memory Viewer.
		class MemoryViewer : public QMainWindow {
		private:
			//! @brief SNES containing all rams to view.
			SNES &_snes;
			//! @brief The layout of the viewer.
			Ui::RamView _ui;
			//! @brief The Ram visualizer model for QT.
			MemoryViewerModel _model;
		public:
			explicit MemoryViewer(SNES &snes);
			MemoryViewer(const MemoryViewer &) = delete;
			MemoryViewer &operator=(const MemoryViewer &) = delete;
			~MemoryViewer() override = default;
		};
	}
}

#endif //COMSQUARE_MEMORYVIEWER_HPP
