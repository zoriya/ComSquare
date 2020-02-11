//
// Created by cbihan on 1/27/20.
//

#ifndef COMSQUARE_PPU_HPP
#define COMSQUARE_PPU_HPP

#include <stdint-gcc.h>
#include "../Memory/IMemory.hpp"

//#define max2BitTiles		4096
//#define max4BitTiles		2048
//#define max8BitTiles		1024



namespace ComSquare::PPU
{

	enum ppuRegisters {
	//! @brief INIDISP Register (F-blank and Brightness)
	inidisp = 0x00,
	//! @brief OBSEL Register (Object Size and Character Address)
	obsel = 0x01,
	//! @brief OAMADDL (OAM Address low byte)
	oamaddl = 0x02,
	//! @brief OAMADDH (OAM Address high bit and Obj Priority)
	oamaddh = 0x03,
	//! @brief OAMDATA (Data for OAM write)
	oamdata = 0x04,
	//! @brief BGMODE (BG Mode and Character Size)
	bgmode = 0x05,
	//! @brief MOSAIC (Screen Pixelation)
	mosaic = 0x06,
	//! @brief BG1SC (BG1 Tilemap Address and Size)
	bg1sc = 0x07,
	//! @brief BG2SC (BG2 Tilemap Address and Size)
	bg2sc = 0x08,
	//! @brief BG3SC (BG3 Tilemap Address and Size)
	bg3sc = 0x09,
	//! @brief BG4SC (BG4 Tilemap Address and Size)
	bg4sc = 0x0A,
	//! @brief BG12NBA (BG1 and 2 Chr Address)
	bg12nba = 0x0B,
	//! @brief BG34NBA (BG3 and 4 Chr Address)
	bg34nba = 0x0C,
	//! @info When bg mode is 7 the register is used as M7HOFS
	bg1hofs = 0x0D,
	//! @info When bg mode is 7 the register is used as M7VOFS
	bg1vofs = 0x0E,
	bg2hofs = 0x0F,
	bg2vofs = 0x10,
	bg3hofs = 0x11,
	bg3vofs = 0x12,
	bg4hofs = 0x13,
	bg4vofs = 0x14,
	vmain = 0x15,
	vmaddl = 0x16,
	vamddh = 0x17,
	vmdatal = 0x18,
	vmdatah = 0x19,
	m7sel = 0x1A,
	m7a = 0x1B,
	m7b = 0x1C,
	m7c = 0x1D,
	m7d = 0x1E,
	m7x = 0x1F,
	m7y = 0x20,
	cgadd = 0x21,
	cgdata = 0x22,
	w12sel = 0x23,
	w34sel = 0x24,
	wobjsel = 0x25,
	wh0 = 0x26,
	wh1 = 0x27,
	wh2 = 0x28,
	wh3 = 0x29,
	wbjlog = 0x2A,
	wobjlog = 0x2B,
	tm = 0x2C,
	ts = 0x2D,
	//! @brief TMW (Window Mask Designation for the Main Screen)
	tmw = 0x2E,
	//! @brief TSW (Window Mask Designation for the Subscreen)
	tsw = 0x2F,
	//! @brief CGWSEL (Color Addition Select)
	cgwsel = 0x30,
	//! @brief CGADSUB (Color math designation)
	cgadsub = 0x31,
	//! @brief COLDATA (Fixed Color Data)
	coldata = 0x32,
	//! @brief SETINI (Screen Mode/Video Select)
	setini = 0x33,
	//! @brief MPYL (Multiplication Result low byte)
	mpyl = 0x34,
	//! @brief MPYM (Multiplication Result middle byte)
	mpum = 0x35,
	//! @brief MPYH (Multiplication Result high byte)
	mpyh = 0x36,
	//! @brief SLHV (Software Latch for H/V Counter)
	slhv = 0x37,
	//! @brief OAMDATAREAD (Data for OAM read)
	oamdataread = 0x38,
	//! @brief VMDATALREAD (VRAM Data Read low byte)
	vmdatalread = 0x39,
	//! @brief VMDATAHREAD (VRAM Data Read high byte)
	vmdatahread = 0x3A,
	//! @brief CGDATAREAD (CGRAM Data read)
	cgdataread = 0x3B,
	//! @brief OPHCT (Horizontal Scanline Location)
	ophct = 0x3C,
	//! @brief OPVCT (Vertical Scanline Location)
	opcvt = 0x3D,
	//! @brief STAT77 (PPU Status Flag and Version)
	stat77 = 0x3E,
	//! @brief STAT78 (PPU Status Flag and Version)
	stat78 = 0x3F
	};

	//! @brief The class containing all the registers the PPU
	class PPU : public Memory::IMemory {
	private:
	/*	struct _layerInfo {
			bool _characterSize;
		};

		struct {
			unsigned int height;
			unsigned int width;
			bool verticalMirroring;
			bool horizontalMirroring;
			int verticalOffset;
			int horizontalOffset;
			//! @brief A Character is the base unit of the background it can be 16x16 or 8x8 (16x8 under certain circumstances)
			unsigned char characterHeight;
			unsigned char characterWidth;
		} _BG[4];

		struct object {
			bool verticalMirroring;
			bool horizontalMirroring;
			bool priority;
			unsigned short graphicAddress;
		};

		//! @brief INIDISP variables (F-blank and Brightness)
		struct {
			bool _fBlank;
			//! @brief F=max, 0="off".
			unsigned short _brightness;
		} _inidisp;

		//! @brief OBSEL variables (Object Size and Character Address)
		struct {
			//! @brief "OamMode" this contains the size of the Objects (ex: 8x8 and 16x16)
			unsigned char _objectSize;
			//! @brief "OamBaseAddress"
			unsigned char _baseSelect;
			//! @brief "OamAddressOffset"
			unsigned char _nameSelect;
		} _obsel;

		//! @brief OAMADD variables (OAM Address and Obj Priority)
		struct {
			uint16_t _oamAddress;
			bool _objPriority;
		} _oamadd;

		//! @brief BGMODE (BG Mode and Character Size)
		struct {
			unsigned char _bgMode;
			bool _mode1Bg3Priority;
			_layerInfo layers[4];
		} _bgmode;*/




		//! @brief INIDISP Register (F-blank and Brightness)
		union {
			struct {
				uint8_t brightness: 4;
				bool _: 3;
				bool fblank: 1;
			};
			uint8_t raw;
		} _inidisp;
		//! @brief OBSEL Register (Object Size and Character Address)
		union {
			struct {
				uint8_t baseSelect: 3;
				bool nameSelect: 2;
				uint8_t objectSize: 3;
			};
			uint8_t raw;
		} _obsel;
		//! @brief OAMADD Register (OAM Address and Obj Priority)
		union {
			struct {
				uint32_t oamAddress: 9;
				uint8_t _: 6;
				bool objPriorityActivationBit: 1;
			};
			struct {
				uint8_t oamaddl;
				uint8_t oamaddh;
			};
			uint32_t raw;
		} _oamadd;
		//! @brief OAMDATA Register (Data for OAM write)
		uint8_t _oamdata;
		//! @brief BGMODE Register (OAM Address and Obj Priority)
		union {
			struct {
				uint8_t bgMode: 3;
				bool mode1Bg3PriorityBit: 1;
				bool characterSizeBg1: 1;
				bool characterSizeBg2: 1;
				bool characterSizeBg3: 1;
				bool characterSizeBg4: 1;
			};
			uint8_t raw;
		} _bgmode;
		//! @brief MOSAIC Register (Screen Pixelation)
		union {
			struct {
				bool affectBg1: 1;
				bool affectBg2: 1;
				bool affectBg3: 1;
				bool affectBg4: 1;
				uint8_t pixelSize: 4;
			};
			uint8_t raw;
		} _mosaic;
		//! @brief BGSC Registers (BG Tilemap Address and Size)
		union {
			struct {
				bool tilemapHorizontalMirroring: 1;
				bool tilemapVerticalMirroring: 1;
				uint8_t tilemapAddress: 6;
			};
			uint8_t raw;
		} _bgsc[4];
		//! @brief BGNBA Registers (BG1/2/3/4 Chr Address)
		union {
			struct {
				uint8_t baseAddressBg1a3: 4;
				uint8_t baseAddressBg2a4: 4;
			};
			uint8_t raw;
		} _bgnba[2];
		//! @brief BG1HOFS Register (BG1 Horizontal Scroll)
		//! @brief BG1VOFS Register (BG1 Vertical Scroll)
		union {
			struct {
				uint16_t offsetBg: 10;
				uint8_t _ : 6;
			};
			uint16_t raw;
		} _bgofs[4];
		//! @brief M7HOFS Register (Mode 7 _BG Horizontal Scroll)
		//! @brief M7VOFS Register (Mode 7 _BG Vertical Scroll)
		union {
			struct {
				uint16_t offsetBg : 13;
				uint8_t _ : 3;
			};
			uint8_t raw;
		} _m7ofs;
		//! @brief BG2HOFS Register (BG2 Horizontal Scroll)
		//! @brief BG2VOFS Register (BG2 Vertical Scroll)
		union {
			struct {
				uint8_t _ : 6;
				uint32_t offsetBg: 10;
			};
			uint8_t raw;
		} _bg2ofs;
		//! @brief BG3HOFS Register (BG3 Horizontal Scroll)
		//! @brief BG3VOFS Register (BG3 Vertical Scroll)
		union {
			struct {
				uint8_t _ : 6;
				uint32_t offsetBg: 10;
			};
			uint8_t raw;
		} _bg3ofs;
		//! @brief BG4HOFS Register (BG4 Horizontal Scroll)
		//! @brief BG4VOFS Register (BG4 Vertical Scroll)
		union {
			struct {
				uint8_t _ : 3;
				uint32_t offsetBg: 10;
			};
			uint8_t raw;
		} _bg4ofs;

		// <check>

		//! @brief VMAIN Register (Video Port Control)
		union {
			struct {
				uint8_t incrementAmount: 2;
				uint8_t addressRemapping: 2;
				uint8_t _ : 3;
				bool incrementMode: 1;
			};
			uint8_t raw;
		} _vmain;
		//! @brief VMADD Register (VRAM Address)
		union {
			struct {
				uint8_t vmaddh;
				uint8_t vmaddl;
			};
			uint32_t vmadd;
		} _vmadd;
		//! @brief VMDATA Register (VRAM Data Write)
		union {
			struct {
				uint8_t vmdatah;
				uint8_t vmdatal;
			};
			uint32_t vmdata;
		} _vmdata;
		//! @brief M7SEL Register (Mode 7 Settings)
		union {
			struct {
				bool playingFieldSize: 1;
				bool emptySpaceFill: 1;
				uint8_t _: 4;
				bool horizontalMirroring: 1;
				bool verticalMirroring: 1;
			};
			uint8_t raw;
		} _m7sel;
		//! M7A M7B M7C M7D i didn't understand how they works so they will be added later.
		//! @brief M7X Register (Mode 7 Center X)
		union {
			struct {
				uint8_t _: 3;
				uint8_t value;
			};
			uint32_t center;
		} _m7x;
		//! @brief M7Y Register (Mode 7 Center Y)
		union {
			struct {
				uint8_t _: 3;
				uint8_t value;
			};
			uint32_t center;
		} _m7y;
		//! @brief CGADD Register (CGRAM Address)
		uint8_t _cgadd;
		//! @brief CGDATA Register (CGRAM Data write)
		union {
			struct {
				bool _: 1;
				uint8_t blue: 5;
				uint8_t green: 5;
				uint8_t red: 5;
			};
			uint16_t raw;
		} _cgdata;
		//! @brief W12SEL - W34SEL Registers (Window Mask Settings for BGs) and WOBJSEL Register (Window Mask Settings for OBJ and Color Window)
		union {
			struct {
				bool enableWindow2ForBg2Bg4Color: 1;
				bool window2InversionForBg2Bg4Color: 1;
				bool enableWindow1ForBg2Bg4Color: 1;
				bool window1InversionForBg2Bg4Color: 1;
				bool enableWindow2ForBg1Bg3Obj: 1;
				bool window2InversionForBg1Bg3Obj: 1;
				bool enableWindow1ForBg1Bg2Obj: 1;
				bool window1InversionForBg1Bg2Obj: 1;
			};
			uint8_t raw;
		} _wsel;
		//! @brief WH0 Register (CWindow 1 Left Position)
		uint8_t wh0;
		//! @brief WH1 Register (CWindow 1 Right Position)
		uint8_t wh1;
		//! @brief WH2 Register (CWindow 2 Left Position)
		uint8_t wh2;
		//! @brief WH3 Register (CWindow 2 Right Position)
		uint8_t wh3;
		//! @brief WBGLOG Register (Window mask logic for BGs)
		union {
			struct {
				uint8_t maskLogicBg1: 2;
				uint8_t maskLogicBg2: 2;
				uint8_t maskLogicBg3: 2;
				uint8_t maskLogicBg4: 2;
			};
			uint8_t raw;
		} wbglog;
		//! @brief WOBJLOG Register (Window mask logic for OBJs and Color Window)
		union {
			struct {
				uint8_t _: 4;
				uint8_t maskLogicObj: 2;
				uint8_t maskLogicColor: 2;
			};
			uint8_t raw;
		} wobjlog;
		//! @brief TM Register (Main Screen Designation)
		union {
			struct {
				uint8_t _: 3;
				bool enableWindowDisplayObj: 1;
				bool enableWindowDisplayBg4: 1;
				bool enableWindowDisplayBg3: 1;
				bool enableWindowDisplayBg2: 1;
				bool enableWindowDisplayBg1: 1;
			};
			uint8_t raw;
		} tm;
		//! @brief TS Register (Sub Screen Designation)
		union {
			struct {
				uint8_t _: 3;
				bool enableWindowDisplayObj: 1;
				bool enableWindowDisplayBg4: 1;
				bool enableWindowDisplayBg3: 1;
				bool enableWindowDisplayBg2: 1;
				bool enableWindowDisplayBg1: 1;
			};
			uint8_t raw;
		} ts;
		//! @brief TMW Register (Window Mask Designation for the Main Screen)
		union {
			struct {
				uint8_t _: 3;
				bool enableWindowMaskingObj: 1;
				bool enableWindowMaskingBg4: 1;
				bool enableWindowMaskingBg3: 1;
				bool enableWindowMaskingBg2: 1;
				bool enableWindowMaskingBg1: 1;
			};
			uint8_t raw;
		} tmw;
		//! @brief TSW Register (Window Mask Designation for the Sub Screen)
		union {
			struct {
				uint8_t _: 3;
				bool enableWindowMaskingObj: 1;
				bool enableWindowMaskingBg4: 1;
				bool enableWindowMaskingBg3: 1;
				bool enableWindowMaskingBg2: 1;
				bool enableWindowMaskingBg1: 1;
			};
			uint8_t raw;
		} tsw;
		//! @brief CGWSEL Register (Color Addition Select)
		union {
			struct {
				uint8_t clipColorToBlackBeforeMath: 2;
				uint8_t preventColorMath: 2;
				uint8_t _: 2;
				bool addSubscreen: 1;
				bool directColorMode: 1;
			};
			uint8_t raw;
		} cgwsel;
		//! @brief CGADSUB Register (Color Math designation)
		union {
			struct {
				bool addSubtractSelect: 1;
				bool halfColorMath: 1;
				bool enableColorMathBackdrop: 1;
				bool enableColorMathObj: 1;
				bool enableColorMathBg4: 1;
				bool enableColorMathBg3: 1;
				bool enableColorMathBg2: 1;
				bool enableColorMathBg1: 1;
			};
			uint8_t raw;
		} cgadsub;
		//! @brief COLDATA Register (Fixed Color Data)
		union {
			struct {
				bool blue: 1;
				bool green: 1;
				bool red: 1;
				uint8_t colorIntensity: 5;
			};
			uint8_t raw;
		} coldata;
		//! @brief SETINI Register (Screen Mode/Video Select)
		union {
			struct {
				bool externalSync: 1;
				bool mode7ExtBg: 1;
				uint8_t _: 2;
				bool enablePseudoHiresMode: 1;
				bool overscanMode: 1;
				bool objInterlace: 1;
				bool screenInterlace: 1;
			};
			uint8_t raw;
		} setini;
		//! @brief MPYL - MPYM - MPYH Registers (Multiplication Result)
		union {
			struct {
				uint8_t mpyl;
				uint8_t mpym;
				uint8_t mpyh;
			};
			uint32_t  mpy;
		} mpy;
	public:
		explicit PPU() = default;
		//! @brief Read data from the component.
		//! @param addr The local address to read from (0x0 should refer to the first byte of this component).
		//! @throw This function should thrown an InvalidAddress for address that are not mapped to the component.
		//! @return Return the data at the address given as parameter.
		uint8_t read(uint24_t addr) override;
		//! @brief Write data to this component.
		//! @param addr The local address to write data (0x0 should refer to the first byte of this component).
		//! @param data The new data to write.
		//! @throw This function should thrown an InvalidAddress for address that are not mapped to the component.
		void write(uint24_t addr, uint8_t data) override;
		//! @brief Update the PPU of n cycles.
		//! @param The number of cycles to update.
		void update(unsigned cycles);
	};
}
#endif //COMSQUARE_PPU_HPP
