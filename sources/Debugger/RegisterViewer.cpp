//
// Created by anonymus-raccoon on 5/28/20.
//

#include "RegisterViewer.hpp"
#include "../SNES.hpp"
#include "../Utility/Utility.hpp"
#include <sstream>

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
		RegistersViewerModel *model;
		for (int i = 0; i < 8; i++) {
			model = new RegistersViewerModel(this->_snes);
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

		// ppuRegisters
		model = new RegistersViewerModel(this->_snes);
		const PPU::Registers &ppuRegisters = this->_snes.ppu->getWriteRegisters();

		//INIDISP 0X2100
		model->addRegister(Register(0x2100, "", "INIDISP", [ppuRegisters](SNES &) {
			return ppuRegisters._inidisp.raw;
		}, nullptr, EightBits));
		model->addRegister(Register(0x2100, ":0-3", "Screen brightness", [ppuRegisters](SNES &) {
			return ppuRegisters._inidisp.brightness;
		}, nullptr, Integer));
		model->addRegister(Register(0x2100, ":7", "F-Blank", [ppuRegisters](SNES &) {
			return ppuRegisters._inidisp.fblank;
		}, nullptr, Boolean));

		//OBSEL 0x2101
		model->addRegister(Register(0x2101, "", "OBSEL", [ppuRegisters](SNES &) {
			return ppuRegisters._obsel.raw;
		}, nullptr, EightBits));
		model->addRegister(Register(0x2101, ":0-2", "Name base select", [ppuRegisters](SNES &) {
			return ppuRegisters._obsel.nameBaseSelect;
		}, nullptr, EightBits));
		model->addRegister(Register(0x2101, ":3-4", "Name select", [ppuRegisters](SNES &) {
			return ppuRegisters._obsel.nameSelect;
		}, nullptr, EightBits));
		model->addRegister(Register(0x2101, ":5-7", "Object Size", [ppuRegisters](SNES &) {
			return ppuRegisters._obsel.objectSize;
		}, nullptr, EightBits));

		// OAMADDL - OAMADDH 0x2102 0x2103
		model->addRegister(Register(0x2102, "", "OAMADDL", [ppuRegisters](SNES &) {
			return ppuRegisters._oamadd.oamaddl;
		}, nullptr, EightBits));
		model->addRegister(Register(0x2103, "", "OAMADDH", [ppuRegisters](SNES &) {
			return ppuRegisters._oamadd.oamaddh;
		}, nullptr, EightBits));
		model->addRegister(Register(0x2102, "", "OAM Address", [ppuRegisters](SNES &) {
			return ppuRegisters._oamadd.oamAddress;
		}, nullptr, SixteenBits));
		model->addRegister(Register(0x2103, ":7", "Obj Priority bit", [ppuRegisters](SNES &) {
			return ppuRegisters._oamadd.objPriorityActivationBit;
		}, nullptr, Boolean));

		//OAMDATA 0x2104
		model->addRegister(Register(0x2104, "", "OAMDATA", [ppuRegisters](SNES &) {
			return ppuRegisters._oamdata;
		}, nullptr, EightBits));

		//BGMODE 0x2105
		model->addRegister(Register(0x2105, "", "BGMODE", [ppuRegisters](SNES &) {
			return ppuRegisters._bgmode.raw;
		}, nullptr, EightBits));
		model->addRegister(Register(0x2105, ":0-2", "BG Mode", [ppuRegisters](SNES &) {
			return ppuRegisters._bgmode.bgMode;
		}, nullptr, Integer));
		model->addRegister(Register(0x2105, ":3", "BG3 Priority Bit", [ppuRegisters](SNES &) {
			return ppuRegisters._bgmode.mode1Bg3PriorityBit;
		}, nullptr, Boolean));
		for (int i = 0; i < 4; i++) {
			model->addRegister(Register(0x2105, ":" + std::to_string(i + 4), "BG"+ std::to_string(i + 1) + " 16x16 Tiles", [ppuRegisters, i](SNES &) {
				return (ppuRegisters._bgmode.raw >> (i + 4)) & 1;
			}, nullptr, Boolean));
		}

		//MOSAIC 0x2106
		model->addRegister(Register(0x2106, "", "MOSAIC", [ppuRegisters](SNES &) {
			return ppuRegisters._mosaic.raw;
		}, nullptr, EightBits));
		for (int i = 0; i < 4; i++) {
			model->addRegister(Register(0x2106, ":" + std::to_string(i), "BG"+ std::to_string(i + 1) + " Mosaic", [ppuRegisters, i](SNES &) {
				return (ppuRegisters._mosaic.raw >> i) & 1;
			}, nullptr, Boolean));
		}
		model->addRegister(Register(0x2106, ":4-7", "Size", [ppuRegisters](SNES &) {
			return ppuRegisters._mosaic.pixelSize;
		}, nullptr, Integer));
	/*	model->addRegister(Register(0x2106, ":4-7", "Value", [](SNES &) {
			return "A lot";
		}, nullptr, String)); */

		// BGNSC 0x2107 è 0x210A
		for (int i = 0; i < 4; i++) {
			model->addRegister(Register(0x2107 + i, "", "BG" + std::to_string(i + 1) + "SC", [ppuRegisters, i](SNES &) {
				return ppuRegisters._bgsc[i].raw;
			}, nullptr, EightBits));
			model->addRegister(Register(0x2107 + i, ":0", "BG" + std::to_string(i + 1) + " Tilemap H mirroring", [ppuRegisters, i](SNES &) {
				return ppuRegisters._bgsc[i].tilemapHorizontalMirroring;
			}, nullptr, Boolean));
			model->addRegister(Register(0x2107 + i, ":1", "BG" + std::to_string(i + 1) + " Tilemap V mirroring", [ppuRegisters, i](SNES &) {
				return ppuRegisters._bgsc[i].tilemapVerticalMirroring;
			}, nullptr, Boolean));
			model->addRegister(Register(0x2107 + i, ":2-7", "BG" + std::to_string(i + 1) + " Tilemap addr", [ppuRegisters, i](SNES &) {
				return ppuRegisters._bgsc[i].tilemapAddress;
			}, nullptr, EightBits));
		}

		// BGnxNBA 0x210B 0x210C
		for (int i = 0; i < 2; i++) {
			model->addRegister(Register(0x210B + i, "", "BG" + std::string(i ? "34" : "12") + "NBA", [ppuRegisters, i](SNES &) {
				return ppuRegisters._bgnba[i].raw;
			}, nullptr, EightBits));
			model->addRegister(Register(0x210B + i, ":0-3", "BG" + std::string((i ? "3" : "1")) + " Base addr", [ppuRegisters, i](SNES &) {
				return ppuRegisters._bgnba[i].baseAddressBg1a3;
			}, nullptr, EightBits));
			model->addRegister(Register(0x210B + i, ":4-7", "BG" + std::string((i ? "4" : "2")) + " Base addr", [ppuRegisters, i](SNES &) {
				return ppuRegisters._bgnba[i].baseAddressBg2a4;
			}, nullptr, EightBits));
		}

		// BGnxOFS M7nOFS 0x210D - 0x2114
		for (int i = 0; i < 4; i++) {
			int tmp = i * 2;
			model->addRegister(Register(0x210D + tmp, "", "BG" + std::to_string(i + 1) + "HOFS", [ppuRegisters, tmp](SNES &) {
				return ppuRegisters._bgofs[tmp].offsetBg;
			}, nullptr, SixteenBits));
			tmp++;
			model->addRegister(Register(0x210D + tmp, "", "BG" + std::to_string(i + 1) + "VOFS", [ppuRegisters, tmp](SNES &) {
				return ppuRegisters._bgofs[tmp].offsetBg;
			}, nullptr, SixteenBits));
		}

		// VMAIN 0x2115
		model->addRegister(Register(0x2115, "", "VMAIN", [ppuRegisters](SNES &) {
			return ppuRegisters._vmain.raw;
		}, nullptr, EightBits));
		model->addRegister(Register(0x2115, ":0-1", "Increment amount", [ppuRegisters](SNES &) {
			return ppuRegisters._vmain.incrementAmount;
		}, nullptr, Integer));
		model->addRegister(Register(0x2115, ":3-4", "Addr remapping", [ppuRegisters](SNES &) {
			return ppuRegisters._vmain.addressRemapping;
		}, nullptr, Integer));
		model->addRegister(Register(0x2115, ":7", "Increment on high byte", [ppuRegisters](SNES &) {
			return ppuRegisters._vmain.incrementMode;
		}, nullptr, Boolean));

		// VMADDL VMADDH 0x2116 0x2117
		model->addRegister(Register(0x2116, "/7", "VMADD", [ppuRegisters](SNES &) {
			return ppuRegisters._vmadd.vmadd;
		}, nullptr, SixteenBits));

		// VMDATAL VMDATAH 0x2118 0x2119
		model->addRegister(Register(0x2118, "/9", "VMDATA", [ppuRegisters](SNES &) {
			return ppuRegisters._vmdata.vmdata;
		}, nullptr, SixteenBits));

		// M7SEL 0x211a
		model->addRegister(Register(0x211a, "", "M7SEL", [ppuRegisters](SNES &) {
			return ppuRegisters._m7sel.raw;
		}, nullptr, EightBits));
		model->addRegister(Register(0x211a, ":0", "M7 H mirroring", [ppuRegisters](SNES &) {
			return ppuRegisters._m7sel.horizontalMirroring;
		}, nullptr, Boolean));
		model->addRegister(Register(0x211a, ":1", "M7 V mirroring", [ppuRegisters](SNES &) {
			return ppuRegisters._m7sel.verticalMirroring;
		}, nullptr, Boolean));
		model->addRegister(Register(0x211a, ":6", "Empty space fill", [ppuRegisters](SNES &) {
			return ppuRegisters._m7sel.emptySpaceFill;
		}, nullptr, Boolean));
		model->addRegister(Register(0x211a, ":7", "Large Tilemap", [ppuRegisters](SNES &) {
			return ppuRegisters._m7sel.playingFieldSize;
		}, nullptr, Boolean));

		// M7x 0x211B - 0x211E
		for (int i = 0; i < 4; i++) {
			std::stringstream tmpStr;
			tmpStr << "M7" << static_cast<char>(i + 'A');
			model->addRegister(Register(0x211B + i, "", tmpStr.str(), [ppuRegisters, i](SNES &) {
				return ppuRegisters._m7[i].m7;
			}, nullptr, SixteenBits));
		}

		// M7X 0x211F
		model->addRegister(Register(0x211F, "","M7X", [ppuRegisters](SNES &) {
			return ppuRegisters._m7x.value;
		}, nullptr, SixteenBits));

		// M7Y 0x2120
		model->addRegister(Register(0x2120, "","M7Y", [ppuRegisters](SNES &) {
			return ppuRegisters._m7y.value;
		}, nullptr, SixteenBits));

		// CGADD 0x2121
		model->addRegister(Register(0x2121, "","CGADD", [ppuRegisters](SNES &) {
			return ppuRegisters._cgadd;
		}, nullptr, EightBits));

		// CGDATA 0x2122
		model->addRegister(Register(0x2122, "","CGDATA", [ppuRegisters](SNES &) {
			return ppuRegisters._cgdata.raw;
		}, nullptr, SixteenBits));

		// W12SEL W34SEL WOBJSEL 0x2123 - 0X2125

		for (int i = 0; i < 3; i++) {
			std::array<std::string, 3> arr;
			switch (i) {
			case 0:
				arr[0] = "W12SEL";
				arr[1] = "BG1";
				arr[2] = "BG2";
				break;
			case 1:
				arr[0] = "W34SEL";
				arr[1] = "BG3";
				arr[2] = "BG4";
				break;
			case 2:
			default:
				arr[0] = "WOBJSEL";
				arr[1] = "OAM";
				arr[2] = "Color";
				break;
			}
			model->addRegister(Register(0x2123 + i, "",arr[0], [ppuRegisters, i](SNES &) {
				return ppuRegisters._wsel[i].raw;
			}, nullptr, EightBits));
			model->addRegister(Register(0x2123 + i, ":0",arr[1] + " Window 1 inverted", [ppuRegisters, i](SNES &) {
				return ppuRegisters._wsel[i].window1InversionForBg1Bg3Obj;
			}, nullptr, Boolean));
			model->addRegister(Register(0x2123 + i, ":1",arr[1] + " Window 1 enabled", [ppuRegisters, i](SNES &) {
				return ppuRegisters._wsel[i].enableWindow1ForBg1Bg3Obj;
			}, nullptr, Boolean));
			model->addRegister(Register(0x2123 + i, ":2",arr[1] + " Window 2 inverted", [ppuRegisters, i](SNES &) {
				return ppuRegisters._wsel[i].window2InversionForBg1Bg3Obj;
			}, nullptr, Boolean));
			model->addRegister(Register(0x2123 + i, ":3",arr[1] + " Window 2 enabled", [ppuRegisters, i](SNES &) {
				return ppuRegisters._wsel[i].enableWindow2ForBg1Bg3Obj;
			}, nullptr, Boolean));

			model->addRegister(Register(0x2123 + i, ":4",arr[2] + " Window 1 inverted", [ppuRegisters, i](SNES &) {
				return ppuRegisters._wsel[i].window1InversionForBg2Bg4Color;
			}, nullptr, Boolean));
			model->addRegister(Register(0x2123 + i, ":5",arr[2] + " Window 1 enabled", [ppuRegisters, i](SNES &) {
				return ppuRegisters._wsel[i].enableWindow1ForBg2Bg4Color;
			}, nullptr, Boolean));
			model->addRegister(Register(0x2123 + i, ":6",arr[2] + " Window 2 inverted", [ppuRegisters, i](SNES &) {
				return ppuRegisters._wsel[i].window2InversionForBg2Bg4Color;
			}, nullptr, Boolean));
			model->addRegister(Register(0x2123 + i, ":7",arr[2] + " Window 2 enabled", [ppuRegisters, i](SNES &) {
				return ppuRegisters._wsel[i].enableWindow1ForBg2Bg4Color;
			}, nullptr, Boolean));
		}

		// WHx 0x2126 - 0x2129
		for (int tmp = 0; tmp < 2; tmp++) {
			int i = tmp * 2;
			model->addRegister(Register(0x2126 + i, "", "Window " + std::to_string(tmp + 1) + " Left", [ppuRegisters, i](SNES &) {
				return ppuRegisters._wh[i];
			}, nullptr, EightBits));
			model->addRegister(Register(0x2126 + i + 1, "", "Window " + std::to_string(tmp + 1) + " Right", [ppuRegisters, i](SNES &) {
				return ppuRegisters._wh[i + 1];
			}, nullptr, EightBits));
		}

		// WBGLOG 0x212A
		model->addRegister(Register(0x212A, "", "WBGLOG", [ppuRegisters](SNES &) {
			return ppuRegisters._wbglog.raw;
		}, nullptr, EightBits));
		model->addRegister(Register(0x212B, "", "WOBJLOG", [ppuRegisters](SNES &) {
			return ppuRegisters._wobjlog.raw;
		}, nullptr, EightBits));
		model->addRegister(Register(0x212A, ":0-1", "BG1 Window mask", [ppuRegisters](SNES &) {
			return ppuRegisters._wbglog.maskLogicBg1;
		}, nullptr, Integer));
		model->addRegister(Register(0x212A, ":2-3", "BG2 Window mask", [ppuRegisters](SNES &) {
			return ppuRegisters._wbglog.maskLogicBg2;
		}, nullptr, Integer));
		model->addRegister(Register(0x212A, ":4-5", "BG3 Window mask", [ppuRegisters](SNES &) {
			return ppuRegisters._wbglog.maskLogicBg3;
		}, nullptr, Integer));
		model->addRegister(Register(0x212A, ":6-7", "BG4 Window mask", [ppuRegisters](SNES &) {
			return ppuRegisters._wbglog.maskLogicBg4;
		}, nullptr, Integer));
		model->addRegister(Register(0x212B, ":0-1", "OBJ Window mask", [ppuRegisters](SNES &) {
			return ppuRegisters._wobjlog.maskLogicObj;
		}, nullptr, Integer));
		model->addRegister(Register(0x212B, ":2-3", "Color Window mask", [ppuRegisters](SNES &) {
			return ppuRegisters._wobjlog.maskLogicColor;
		}, nullptr, Integer));

		// TM TS 0x212c 0x212d
		for (int j = 0; j < 2; j++) {
			model->addRegister(Register(0x212C + j, "", std::string((j ? "TS" : "TM")), [ppuRegisters, j](SNES &) {
				return ppuRegisters._t[j].raw;
			}, nullptr, EightBits));
			for (int i = 0; i < 4; i ++) {
				model->addRegister(Register(0x212C + j, ":" + std::to_string(i), "BG" + std::to_string(i + 1) + " Enabled",
					[ppuRegisters, i, j](SNES &) {
						return (ppuRegisters._t[j].raw >> i) & 1;
					}, nullptr, Boolean));
			}
			model->addRegister(Register(0x212C + j, ":4", "OBJ Enabled", [ppuRegisters, j](SNES &) {
				return ppuRegisters._t[j].enableWindowDisplayObj;
			}, nullptr, Boolean));
		}

		// TMW TSW 0x212e 0x212f
		for (int j = 0; j < 2; j++) {
			model->addRegister(Register(0x212e + j, "", std::string((j ? "TSW" : "TMW")), [ppuRegisters, j](SNES &) {
				return ppuRegisters._tw[j].raw;
			}, nullptr, EightBits));
			for (int i = 0; i < 4; i ++) {
				model->addRegister(Register(0x212e + j, ":" + std::to_string(i), "BG" + std::to_string(i + 1) + " Window Mask Enabled",
											[ppuRegisters, i, j](SNES &) {
												return (ppuRegisters._tw[j].raw >> i) & 1;
											}, nullptr, Boolean));
			}
			model->addRegister(Register(0x212e + j, ":4", "OBJ Window Mask Enabled", [ppuRegisters, j](SNES &) {
				return ppuRegisters._tw[j].enableWindowMaskingObj;
			}, nullptr, Boolean));
		}

		this->_ui.ppuRegisters->setModel(model);
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
		case String:
			return QString(reg.get(this->_snes));
		case Integer:
			return QString::number(reg.get(this->_snes));
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