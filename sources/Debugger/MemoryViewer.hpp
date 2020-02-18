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
	//! @brief The ram to watch.
	std::shared_ptr<Ram> _memory;
public:
	//! @brief Change the ram currently watched.
	void setMemory(std::shared_ptr<Ram> memory);

	explicit MemoryViewerModel(std::shared_ptr<Ram> memory, QObject *parent = nullptr);
	MemoryViewerModel(const MemoryViewerModel &) = delete;
	const MemoryViewerModel &operator=(const MemoryViewerModel &) = delete;
	~MemoryViewerModel() override = default;

	//! @brief The number of row the table has.
	int rowCount(const QModelIndex &parent) const override;
	//! @brief The number of column the table has.
	int columnCount(const QModelIndex &parent) const override;
	//! @brief Return a data represneting the table cell.
	QVariant data(const QModelIndex &index, int role) const override;
	//! @brief Override the headers to use hex values.
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
			//! @brief Callback called when a memory tab is selected.
			void changeRam(int id);

			explicit MemoryViewer(SNES &snes);
			MemoryViewer(const MemoryViewer &) = delete;
			MemoryViewer &operator=(const MemoryViewer &) = delete;
			~MemoryViewer() override = default;
		};
	}
}

#endif //COMSQUARE_MEMORYVIEWER_HPP
