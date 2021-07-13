//
// Created by cbihan on 13/07/2021.
//

#pragma once


namespace ComSquare::PPU
{
	enum PpuRegisters {
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
		//! @brief BG1HOFS (BG1 Horizontal Scroll)
		//! @brief M7HOFS (Mode 7 BG Horizontal Scroll)
		//! @info When bg mode is 7 the register is used as M7HOFS
		bg1hofs = 0x0D,
		//! @brief BG1VOFS (BG1 Vertical Scroll)
		//! @brief M7VOFS (Mode 7 BG Vertical Scroll)
		//! @info When bg mode is 7 the register is used as M7VOFS
		bg1vofs = 0x0E,
		//! @brief BG2HOFS (BG2 Horizontal Scroll)
		bg2hofs = 0x0F,
		//! @brief BG2VOFS (BG2 Vertical Scroll)
		bg2vofs = 0x10,
		//! @brief BG3HOFS (BG3 Horizontal Scroll)
		bg3hofs = 0x11,
		//! @brief BG3VOFS (BG3 Vertical Scroll)
		bg3vofs = 0x12,
		//! @brief BG4HOFS (BG4 Horizontal Scroll)
		bg4hofs = 0x13,
		//! @brief BG4VOFS (BG4 Vertical Scroll)
		bg4vofs = 0x14,
		//! @brief VMAIN (Video Port Control)
		vmain = 0x15,
		//! @brief VMADDL (VRAM Address low byte)
		vmaddl = 0x16,
		//! @brief VMADDH (VRAM Address high byte)
		vmaddh = 0x17,
		//! @brief VMDATAL (VRAM Data Write low byte)
		vmdatal = 0x18,
		//! @brief VMDATAH (VRAM Data Write high byte)
		vmdatah = 0x19,
		//! @brief M7SEL (Mode 7 Settings)
		m7sel = 0x1A,
		//! @brief M7A (Mode 7 Matrix A) also used with $2134/6
		m7a = 0x1B,
		//! @brief M7B (Mode 7 Matrix B) also used with $2134/6
		m7b = 0x1C,
		//! @brief M7C (Mode 7 Matrix C)
		m7c = 0x1D,
		//! @brief M7D (Mode 7 Matrix D)
		m7d = 0x1E,
		//! @brief M7X (Mode 7 Center X)
		m7x = 0x1F,
		//! @brief M7Y (Mode 7 Center Y)
		m7y = 0x20,
		//! @brief CGADD (CGRAM Address)
		cgadd = 0x21,
		//! @brief CGDATA (CGRAM Data write)
		cgdata = 0x22,
		//! @brief W12SEL (Window Mask Settings for BG1 and BG2)
		w12sel = 0x23,
		//! @brief W34SEL (Window Mask Settings for BG3 and BG4)
		w34sel = 0x24,
		//! @brief WOBJSEL (Window Mask Settings for OBJ and Color Window)
		wobjsel = 0x25,
		//! @brief WH0 (Window 1 Left Position)
		wh0 = 0x26,
		//! @brief WH1 (Window 1 Right Position)
		wh1 = 0x27,
		//! @brief WH2 (Window 2 Left Position)
		wh2 = 0x28,
		//! @brief WH3 (Window 2 Right Position)
		wh3 = 0x29,
		//! @brief WBGLOG (Window mask logic for BGs)
		wbjlog = 0x2A,
		//! @brief WOBJLOG (Window mask logic for OBJs and Color Window)
		wobjlog = 0x2B,
		//! @brief TM (Main Screen Designation)
		tm = 0x2C,
		//! @brief TS (Subscreen Designation)
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
		mpym = 0x35,
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
		opvct = 0x3D,
		//! @brief STAT77 (PPU Status Flag and Version)
		stat77 = 0x3E,
		//! @brief STAT78 (PPU Status Flag and Version)
		stat78 = 0x3F
	};

	struct Registers {
		//! @brief INIDISP Register (F-blank and Brightness)
		union {
			struct {
				//! @brief Store the brightness value (F = max, 0 = off)
				uint8_t brightness: 4;
				uint8_t _: 3;
				//! @brief Store the FBlank status
				uint8_t fblank: 1;
			};
			uint8_t raw = 0;
		} _inidisp;
		//! @brief OBSEL Register (Object Size and Character Address)
		union {
			struct {
				//! @brief Stores the location of the first sprite table
				uint8_t nameBaseSelect: 3;
				//! @brief Stores the offset of the second sprite table
				uint8_t nameSelect: 2;
				//! @brief Stores the resolution preset of the sprites
				uint8_t objectSize: 3;
			};
			uint8_t raw = 0;
		} _obsel;
		//! @brief OAMADD Register (OAM Address and Obj Priority)
		union {
			struct {
				//! @brief Stores the address to write with OAMDATA register
				uint16_t oamAddress: 9;
				uint16_t _: 6;
				//! @brief When Obj Priority activation bit is set, an Obj other than Sprite 0 may be given priority
				uint16_t objPriorityActivationBit: 1;
			};
			struct {
				//! @brief Stores the data written on the OAMADDL register
				uint16_t oamaddl: 8;
				//! @brief Stores the data written on the OAMADDH register
				uint16_t oamaddh: 8;
			};
			uint16_t raw = 0;
		} _oamadd;
		//! @brief OAMDATA Register (Data for OAM write)
		uint8_t _oamdata = 0;
		//! @brief BGMODE Register (OAM Address and Obj Priority)
		union {
			struct {
				//! @brief Stores the current BG Mode (0 - 7)
				uint8_t bgMode: 3;
				//! @brief When Mode 1 BG3 priority bit is set the BG3 is draw
				uint8_t mode1Bg3PriorityBit: 1;
				//! @brief When The bit is set character size will 16x16 otherwise it is 8x8
				uint8_t characterSizeBg1: 1;
				//! @brief When The bit is set character size will 16x16 otherwise it is 8x8
				uint8_t characterSizeBg2: 1;
				//! @brief When The bit is set character size will 16x16 otherwise it is 8x8
				uint8_t characterSizeBg3: 1;
				//! @brief When The bit is set character size will 16x16 otherwise it is 8x8
				uint8_t characterSizeBg4: 1;
			};
			uint8_t raw = 0;
		} _bgmode;
		//! @brief MOSAIC Register (Screen Pixelation)
		union {
			struct {
				//! @brief Apply mosaic to the BG1
				uint8_t affectBg1: 1;
				//! @brief Apply mosaic to the BG2
				uint8_t affectBg2: 1;
				//! @brief Apply mosaic to the BG3
				uint8_t affectBg3: 1;
				//! @brief Apply mosaic to the BG4
				uint8_t affectBg4: 1;
				//! @brief Stores the pixel size (0 = 1x1, F = 16x16)
				uint8_t pixelSize: 4;
			};
			uint8_t raw = 0;
		} _mosaic;
		//! @brief BGSC Registers (BG Tilemap Address and Size)
		union {
			struct {
				//! @brief When tilemap horizontal mirroring bit is set the tilemap is mirrored horizontally
				uint8_t tilemapHorizontalMirroring: 1;
				//! @brief When tilemap vertically mirroring bit is set the tilemap is mirrored vertically
				uint8_t tilemapVerticalMirroring: 1;
				//! @brief Address of the tilemap Address (0, 0)
				uint8_t tilemapAddress: 6;
			};
			uint8_t raw = 0;
		} _bgsc[4];
		//! @brief BGNBA Registers (BG1/2/3/4 Chr Address)
		union {
			struct {
				//! @brief The address let us know where to search for BG1/3 characters
				uint8_t baseAddressBg1a3: 4;
				//! @brief The address let us know where to search for BG2/4 characters
				uint8_t baseAddressBg2a4: 4;
			};
			uint8_t raw = 0;
		} _bgnba[2];
		//! @brief BGXXOFS Register (BG1/2/3/4 Horizontal and Vertical Scrolls)
		union {
			struct {
				uint16_t offsetBg: 10;
				uint16_t _ : 6;
			};
			uint16_t raw = 0;
		} _bgofs[8];
		//! @brief M7HOFS Register (Mode 7 BG Horizontal Scroll)
		//! @brief M7VOFS Register (Mode 7 BG Vertical Scroll)
		union {
			struct {
				uint16_t offsetBg : 13;
				uint16_t _ : 3;
			};
			uint16_t raw = 0;
		} _m7ofs[2];
		//! @brief VMAIN Register (Video Port Control)
		union {
			struct {
				uint8_t incrementAmount: 2;
				uint8_t addressRemapping: 2;
				uint8_t _ : 3;
				uint8_t incrementMode: 1;
			};
			uint8_t raw = 0;
		} _vmain;
		//! @brief Store the real value of the increment Amount (1, 32, 128) instead of 0,1 or 2
		uint8_t _incrementAmount = 1;
		//! @brief VMADD Register (VRAM Address)
		union {
			struct {
				uint16_t vmaddl: 8;
				uint16_t vmaddh: 8;
			};
			uint16_t vmadd = 0;
		} _vmadd;
		//! @brief VMDATA Register (VRAM Data Write)
		union {
			struct {
				uint16_t vmdatal: 8;
				uint16_t vmdatah: 8;
			};
			uint16_t vmdata = 0;
		} _vmdata;
		//! @brief TODO M7SEL Register (Mode 7 Settings)
		union {
			struct {
				uint8_t horizontalMirroring: 1;
				uint8_t verticalMirroring: 1;
				uint8_t _: 4;
				uint8_t emptySpaceFill: 1;
				uint8_t playingFieldSize: 1;
			};
			uint8_t raw = 0;
		} _m7sel;
		//! @brief M7A M7B M7C M7C registers, M7A and M7B are also used with ($2134/6) (multiplactions registers)
		union {
			struct {
				uint16_t m7l: 8;
				uint16_t m7h: 8;
			};
			uint16_t m7 = 0;
		} _m7[4];
		// <to work>

		//! @brief M7X Register (Mode 7 Center X)
		union { // Not sure if it is done correctly
			struct {
				uint16_t _: 3;
				uint16_t value: 13;
			};
			uint16_t raw = 0;
		} _m7x;
		//! @brief M7Y Register (Mode 7 Center Y)
		union { // Not sure if it is done correctly
			struct {
				uint16_t _: 3;
				uint16_t value: 13;
			};
			uint16_t raw = 0;
		} _m7y;
		//! @brief CGADD Register (CGRAM Address)
		uint8_t _cgadd = 0;
		//! @brief CGDATA Register (CGRAM Data write)
		union {
			struct {
				uint16_t red: 5;
				uint16_t green: 5;
				uint16_t blue: 5;
				uint16_t _: 1;
			};
			struct {
				uint16_t cgdatal: 8;
				uint16_t cgdatah: 8;
			};
			uint16_t raw = 0;
		} _cgdata;
		//! @brief This bool is used for writing either the low byte of the data (first call) or the high byte of the data (second call)
		//! @info This bool is set to True when writing to $2121 (CGADD)
		bool _isLowByte = false;
		//! @brief W12SEL - W34SEL Registers (Window Mask Settings for BGs) and WOBJSEL Register (Window Mask Settings for OBJ and Color Window)
		union {
			struct {
				uint8_t enableWindow2ForBg2Bg4Color: 1;
				uint8_t window2InversionForBg2Bg4Color: 1;
				uint8_t enableWindow1ForBg2Bg4Color: 1;
				uint8_t window1InversionForBg2Bg4Color: 1;
				uint8_t enableWindow2ForBg1Bg3Obj: 1;
				uint8_t window2InversionForBg1Bg3Obj: 1;
				uint8_t enableWindow1ForBg1Bg3Obj: 1;
				uint8_t window1InversionForBg1Bg3Obj: 1;
			};
			uint8_t raw = 0;
		} _wsel[3];
		//! @brief WH0 Register (Window 1 Left Position)
		//! @brief WH1 Register (Window 1 Right Position)
		//! @brief WH2 Register (Window 2 Left Position)
		//! @brief WH3 Register (Window 2 Right Position)
		uint8_t _wh[4] = {0};
		//! @brief WBGLOG Register (Window mask logic for BGs)
		union {
			struct {
				uint8_t maskLogicBg4: 2;
				uint8_t maskLogicBg3: 2;
				uint8_t maskLogicBg2: 2;
				uint8_t maskLogicBg1: 2;
			};
			uint8_t raw = 0;
		} _wbglog;
		//! @brief WOBJLOG Register (Window mask logic for OBJs and Color Window)
		union {
			struct {
				uint8_t maskLogicObj: 2;
				uint8_t maskLogicColor: 2;
				uint8_t _: 4;
			};
			uint8_t raw = 0;
		} _wobjlog;
		//! @brief TM - TS Registers (Main & Sub Screen Designation)
		union {
			struct {
				uint8_t enableWindowDisplayBg1: 1;
				uint8_t enableWindowDisplayBg2: 1;
				uint8_t enableWindowDisplayBg3: 1;
				uint8_t enableWindowDisplayBg4: 1;
				uint8_t enableWindowDisplayObj: 1;
				uint8_t _: 3;
			};
			uint8_t raw = 0;
		} _t[2];
		//! @brief TMW - TSW Registers (Window Mask Designation for the Main/Sub Screen)
		union {
			struct {
				uint8_t enableWindowMaskingBg1: 1;
				uint8_t enableWindowMaskingBg2: 1;
				uint8_t enableWindowMaskingBg3: 1;
				uint8_t enableWindowMaskingBg4: 1;
				uint8_t enableWindowMaskingObj: 1;
				uint8_t _: 3;
			};
			uint8_t raw = 0;
		} _tw[2];
		//! @brief CGWSEL Register (Color Addition Select)
		union {
			struct {
				uint8_t directColorMode: 1;
				uint8_t addSubscreen: 1;
				uint8_t _: 2;
				uint8_t preventColorMath: 2;
				uint8_t clipColorToBlackBeforeMath: 2;
			};
			uint8_t raw = 0;
		} _cgwsel;
		//! @brief CGADSUB Register (Color Math designation)
		union {
			struct {
				uint8_t enableColorMathBg1: 1;
				uint8_t enableColorMathBg2: 1;
				uint8_t enableColorMathBg3: 1;
				uint8_t enableColorMathBg4: 1;
				uint8_t enableColorMathObj: 1;
				uint8_t enableColorMathBackdrop: 1;
				uint8_t halfColorMath: 1;
				uint8_t addSubtractSelect: 1;
			};
			uint8_t raw = 0;
		} _cgadsub;
		//! @brief COLDATA Register (Fixed Color Data)
		union {
			struct {
				uint8_t colorIntensity: 5;
				uint8_t red: 1;
				uint8_t green: 1;
				uint8_t blue: 1;
			};
			uint8_t raw = 0;
		} _coldata;
		//! @brief SETINI Register (Screen Mode/Video Select)
		union {
			struct {
				uint8_t screenInterlace: 1;
				uint8_t objInterlace: 1;
				uint8_t overscanMode: 1;
				uint8_t enablePseudoHiresMode: 1;
				uint8_t _: 2;
				uint8_t mode7ExtBg: 1;
				uint8_t externalSync: 1;
			};
			uint8_t raw = 0;
		} _setini;

		// <READ registers>

		//! @brief MPYL - MPYM - MPYH Registers (Multiplication Result)
		union {
			struct {
				uint32_t mpyl: 8;
				uint32_t mpym: 8;
				uint32_t mpyh: 8;
				uint32_t _: 8;
			};
			uint32_t mpy = 0;
		} _mpy;
		//! @brief SLHV - Software Latch for H/V Counter
		uint8_t _slhv = 0;
		//! @brief OAMDATAREAD - Data for OAM read
		uint8_t _oamdataread = 0;
		//! @brief VMDATALREAD/VMDATAHREAD - VRAM Data Read low/high byte
		union {
			struct {
				uint16_t vmDataLRead: 8;
				uint16_t vmDataHRead: 8;
			};
			uint16_t raw = 0;
		} _vmdataread;
		//! @brief CGRAM Data read
		union {
			struct {
				uint16_t cgDataLRead: 8;
				uint16_t cgDataHRead: 8;
			};
			uint16_t raw = 0;
		} _cgdataread;
		//! @brief OPHCT/OPVCT - Horizontal/Vertical Scanline Location
		union {
			struct {
				uint16_t opct: 9;
				uint16_t _: 7;
			};
			uint16_t raw = 0;
		} _opct;
		//! @brief STAT77 - PPU Status Flag and Version
		union {
			struct {
				uint8_t chipVersionNumber: 4;
				uint8_t _: 1;
				uint8_t modeSelect: 1;
				uint8_t rangeOverFlag: 1;
				uint8_t timeOverFlag: 1;
			};
			uint8_t raw = 0;
		} _stat77;
		//! @brief STAT78 - PPU Status Flag and Version
		union {
			struct {
				uint8_t chipVersionNumber: 4;
				uint8_t mode: 1;
				uint8_t _: 1;
				uint8_t externalLatchFlag: 1;
				uint8_t interlaceField: 1;
			};
			uint8_t raw = 0;
		} _stat78;
	};
}