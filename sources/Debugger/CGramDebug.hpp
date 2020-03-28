//
// Created by cbihan on 3/27/20.
//

#ifndef COMSQUARE_CGRAMDEBUG_HPP
#define COMSQUARE_CGRAMDEBUG_HPP

#include <QtWidgets/QMainWindow>
#include "../PPU/PPU.hpp"
#include "../../ui/ui_cgramView.h"
#include <QtCore/QSortFilterProxyModel>
#include "ClosableWindow.hpp"


/*namespace ComSquare
{
	class SNES;
	namespace Debugger
	{
		//! @brief Window that show the header of the currently running game.
		class cgramViewer {
		private:
			//! @brief The QT window for this debugger.
			ClosableWindow<cgramViewer> *_window{};
			//! @brief A reference to the snes (to disable the debugger).
			SNES &_snes;
			//! @brief The layout of the viewer.
			Ui::CgramView _ui;
			//! @brief The ppu containing the cgram.
			ComSquare::PPU::PPU &_ppu;
		public slots:
			//! @brief Called when the window is closed. Turn off the debugger and revert to a basic CPU.
			void disableDebugger();
		public:
			//! @brief Focus the debugger's window.
			void focus();

			explicit cgramViewer(SNES &snes, ComSquare::PPU::PPU &ppu);
			cgramViewer(const cgramViewer &) = delete;
			cgramViewer &operator=(const cgramViewer &) = delete;
			~cgramViewer() = default;
		};
	}
}*/

/*namespace ComSquare::cgramDebugger
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
}*/


//! @brief The qt model that bind the logs to the view.
class CGramModel : public QAbstractTableModel
{
Q_OBJECT
private:
	//! @brief The ppu to log the cgram.
	ComSquare::PPU::PPU &_ppu;
public:
	//! @brief The number of columns
	const int column = 16;
	//! @brief The number of rows
	const int rows = 16;
	explicit CGramModel(ComSquare::PPU::PPU &ppu);
	CGramModel(const CGramModel &) = delete;
	const CGramModel &operator=(const CGramModel &) = delete;
	~CGramModel() override = default;

	//! @brief The number of row the table has.
	int rowCount(const QModelIndex &parent) const override;
	//! @brief The number of column the table has.
	int columnCount(const QModelIndex &parent) const override;
	//! @brief Return a data representing the table cell.
	QVariant data(const QModelIndex &index, int role) const override;
};


namespace ComSquare::Debugger
{
	//! @brief window that allow the user to view all data going through the memory bus.
	class CGramDebug {
	private:
		//! @brief The QT window for this debugger.
		ClosableWindow<CGramDebug> *_window;
		//! @brief A reference to the snes (to disable the debugger).
		SNES &_snes;
		//! @brief A widget that contain the whole UI.
		Ui::CgramView _ui;
		//! @brief The Log visualizer model for QT.
		CGramModel _model;
		//! @brief A reference to the ppu
		ComSquare::PPU::PPU &_ppu;
	public:
		//! @brief Called when the window is closed. Turn off the debugger.
		void disableViewer();
	public:
		explicit CGramDebug(SNES &snes, ComSquare::PPU::PPU &ppu);
		CGramDebug(const CGramDebug &) = delete;
		CGramDebug &operator=(const CGramDebug &) = delete;
		~CGramDebug() = default;

		//! @brief Read data at the CGRAM address send it to the debugger.
		//! @param addr The address to read from.
		//! @return The color value in BGR, looks like this xbbbbbgggggrrrrr.
		uint16_t read(uint8_t addr);

		//! @brief Focus the debugger's window.
		void focus();

		//! @brief Return true if the Bus is overloaded with debugging features.
		bool isDebugger();
	};
}

#endif //COMSQUARE_CGRAMDEBUG_HPP
