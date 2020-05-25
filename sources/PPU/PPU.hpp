//
// Created by cbihan on 1/27/20.
//

#ifndef COMSQUARE_PPU_HPP
#define COMSQUARE_PPU_HPP

#include <stdint-gcc.h>
#include "../Memory/AMemory.hpp"
#include "../Memory/MemoryBus.hpp"
#include "../Renderer/IRenderer.hpp"
//#include "../Ram/ExtendedRam.hpp"
#include "../Ram/Ram.hpp"
#include "../Models/Vector2.hpp"
#include "Background.hpp"

//#define max2BitTiles		4096
//#define max4BitTiles		2048
//#define max8BitTiles		1024

namespace ComSquare::PPU
{
	class Background;
	enum bgName {
		bg1NoPriority = 0,
		bg1Priority,
		bg2NoPriority,
		bg2Priority,
		bg3NoPriority,
		bg3Priority,
		bg4NoPriority,
		bg4Priority
	};

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
				bool _: 3;
				//! @brief Store the FBlank status
				bool fblank: 1;
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
				uint32_t oamAddress: 9;
				uint8_t _: 6;
				//! @brief When Obj Priority activation bit is set, an Obj other than Sprite 0 may be given priority
				bool objPriorityActivationBit: 1;
			};
			struct {
				//! @brief Stores the data written on the OAMADDL register
				uint8_t oamaddl;
				//! @brief Stores the data written on the OAMADDH register
				uint8_t oamaddh;
			};
			uint32_t raw = 0;
		} _oamadd;
		//! @brief OAMDATA Register (Data for OAM write)
		uint8_t _oamdata;
		//! @brief BGMODE Register (OAM Address and Obj Priority)
		union {
			struct {
				//! @brief Stores the current BG Mode (0 - 7)
				uint8_t bgMode: 3;
				//! @brief When Mode 1 BG3 priority bit is set the BG3 is draw
				bool mode1Bg3PriorityBit: 1;
				//! @brief When The bit is set character size will 16x16 otherwise it is 8x8
				bool characterSizeBg1: 1;
				//! @brief When The bit is set character size will 16x16 otherwise it is 8x8
				bool characterSizeBg2: 1;
				//! @brief When The bit is set character size will 16x16 otherwise it is 8x8
				bool characterSizeBg3: 1;
				//! @brief When The bit is set character size will 16x16 otherwise it is 8x8
				bool characterSizeBg4: 1;
			};
			uint8_t raw = 0;
		} _bgmode;
		//! @brief MOSAIC Register (Screen Pixelation)
		union {
			struct {
				//! @brief Apply mosaic to the BG1
				bool affectBg1: 1;
				//! @brief Apply mosaic to the BG2
				bool affectBg2: 1;
				//! @brief Apply mosaic to the BG3
				bool affectBg3: 1;
				//! @brief Apply mosaic to the BG4
				bool affectBg4: 1;
				//! @brief Stores the pixel size (0 = 1x1, F = 16x16)
				uint8_t pixelSize: 4;
			};
			uint8_t raw = 0;
		} _mosaic;
		//! @brief BGSC Registers (BG Tilemap Address and Size)
		union {
			struct {
				//! @brief When tilemap horizontal mirroring bit is set the tilemap is mirrored horizontally
				bool tilemapHorizontalMirroring: 1;
				//! @brief When tilemap vertically mirroring bit is set the tilemap is mirrored vertically
				bool tilemapVerticalMirroring: 1;
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
				uint8_t _ : 6;
			};
			uint16_t raw;
		} _bgofs[8];
		//! @brief M7HOFS Register (Mode 7 BG Horizontal Scroll)
		//! @brief M7VOFS Register (Mode 7 BG Vertical Scroll)
		union {
			struct {
				uint16_t offsetBg : 13;
				uint8_t _ : 3;
			};
			uint8_t raw;
		} _m7ofs[2];
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
		//! @brief Store the real value of the increment Amount (1, 32, 128) instead of 0,1 or 2
		uint8_t _incrementAmount = 1;
		//! @brief VMADD Register (VRAM Address)
		union {
			struct {
				uint8_t vmaddl;
				uint8_t vmaddh;
			};
			uint16_t vmadd;
		} _vmadd;
		//! @brief VMDATA Register (VRAM Data Write)
		union {
			struct {
				uint8_t vmdatal;
				uint8_t vmdatah;
			};
			uint16_t vmdata;
		} _vmdata;
		//! @brief TODO M7SEL Register (Mode 7 Settings)
		union {
			struct {
				bool horizontalMirroring: 1;
				bool verticalMirroring: 1;
				uint8_t _: 4;
				bool emptySpaceFill: 1;
				bool playingFieldSize: 1;
			};
			uint8_t raw;
		} _m7sel;
		//! @brief M7A M7B M7C M7C registers, M7A and M7B are also used with ($2134/6) (multiplactions registers)
		union {
			struct {
				uint8_t m7l;
				uint8_t m7h;
			};
			uint16_t m7;
		} _m7[4];
		// <to work>

		//! @brief M7X Register (Mode 7 Center X)
		union { // Not sure if it is done correctly
			struct {
				uint8_t _: 3;
				uint8_t value;
			};
			uint32_t center;
		} _m7x;
		//! @brief M7Y Register (Mode 7 Center Y)
		union { // Not sure if it is done correctly
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
				uint8_t red: 5;
				uint8_t green: 5;
				uint8_t blue: 5;
				bool _: 1;
			};
			struct {
				uint8_t cgdatal;
				uint8_t cgdatah;
			};
			uint16_t raw;
		} _cgdata;
		//! @brief This bool is used for writing either the low byte of the data (first call) or the high byte of the data (second call)
		//! @info This bool is set to True when writing to $2121 (CGADD)
		bool _isLowByte;
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
		} _wsel[3];
		//! @brief WH0 Register (Window 1 Left Position)
		uint8_t _wh0;
		//! @brief WH1 Register (Window 1 Right Position)
		uint8_t _wh1;
		//! @brief WH2 Register (Window 2 Left Position)
		uint8_t _wh2;
		//! @brief WH3 Register (Window 2 Right Position)
		uint8_t _wh3;
		//! @brief WBGLOG Register (Window mask logic for BGs)
		union {
			struct {
				uint8_t maskLogicBg4: 2;
				uint8_t maskLogicBg3: 2;
				uint8_t maskLogicBg2: 2;
				uint8_t maskLogicBg1: 2;
			};
			uint8_t raw;
		} _wbglog;
		//! @brief WOBJLOG Register (Window mask logic for OBJs and Color Window)
		union {
			struct {
				uint8_t maskLogicObj: 2;
				uint8_t maskLogicColor: 2;
				uint8_t _: 4;
			};
			uint8_t raw;
		} _wobjlog;
		//! @brief TM - TS Registers (Main & Sub Screen Designation)
		union {
			struct {
				bool enableWindowDisplayBg1: 1;
				bool enableWindowDisplayBg2: 1;
				bool enableWindowDisplayBg3: 1;
				bool enableWindowDisplayBg4: 1;
				bool enableWindowDisplayObj: 1;
				uint8_t _: 3;
			};
			uint8_t raw;
		} _t[2];
		//! @brief TMW - TSW Registers (Window Mask Designation for the Main/Sub Screen)
		union {
			struct {
				bool enableWindowMaskingBg1: 1;
				bool enableWindowMaskingBg2: 1;
				bool enableWindowMaskingBg3: 1;
				bool enableWindowMaskingBg4: 1;
				bool enableWindowMaskingObj: 1;
				uint8_t _: 3;
			};
			uint8_t raw;
		} _tw[2];
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
		} _tsw;
		//! @brief CGWSEL Register (Color Addition Select)
		union {
			struct {
				bool directColorMode: 1;
				bool addSubscreen: 1;
				uint8_t _: 2;
				uint8_t preventColorMath: 2;
				uint8_t clipColorToBlackBeforeMath: 2;
			};
			uint8_t raw;
		} _cgwsel;
		//! @brief CGADSUB Register (Color Math designation)
		union {
			struct {
				bool enableColorMathBg1: 1;
				bool enableColorMathBg2: 1;
				bool enableColorMathBg3: 1;
				bool enableColorMathBg4: 1;
				bool enableColorMathObj: 1;
				bool enableColorMathBackdrop: 1;
				bool halfColorMath: 1;
				bool addSubtractSelect: 1;
			};
			uint8_t raw;
		} _cgadsub;
		//! @brief COLDATA Register (Fixed Color Data)
		union {
			struct {
				uint8_t colorIntensity: 5;
				bool red: 1;
				bool green: 1;
				bool blue: 1;
			};
			uint8_t raw;
		} _coldata;
		//! @brief SETINI Register (Screen Mode/Video Select)
		union {
			struct {
				bool screenInterlace: 1;
				bool objInterlace: 1;
				bool overscanMode: 1;
				bool enablePseudoHiresMode: 1;
				uint8_t _: 2;
				bool mode7ExtBg: 1;
				bool externalSync: 1;
			};
			uint8_t raw;
		} _setini;

		// <READ registers>

		//! @brief MPYL - MPYM - MPYH Registers (Multiplication Result)
		union {
			struct {
				uint8_t mpyl;
				uint8_t mpym;
				uint8_t mpyh;
			};
			uint32_t mpy;
		} _mpy;
		//! @brief SLHV - Software Latch for H/V Counter
		uint8_t _slhv;
		//! @brief OAMDATAREAD - Data for OAM read
		uint8_t _oamdataread;
		//! @brief VMDATALREAD/VMDATAHREAD - VRAM Data Read low/high byte
		union {
			struct {
				uint8_t vmDataLRead;
				uint8_t vmDataHRead;
			};
			uint16_t raw;
		} _vmdataread;
		//! @brief CGRAM Data read
		union {
			struct {
				uint8_t cgDataLRead;
				uint8_t cgDataHRead;
			};
			uint16_t raw;
		} _cgdataread;
		//! @brief OPHCT/OPVCT - Horizontal/Vertical Scanline Location
		union {
			struct {
				uint16_t opct: 9;
				uint8_t _: 7;
			};
			uint16_t raw;
		} _opct;
		//! @brief STAT77 - PPU Status Flag and Version
		union {
			struct {
				uint8_t chipVersionNumber: 4;
				bool _: 1;
				bool modeSelect: 1;
				bool rangeOverFlag: 1;
				bool timeOverFlag: 1;
			};
			uint8_t raw;
		} _stat77;
		//! @brief STAT78 - PPU Status Flag and Version
		union {
			struct {
				uint8_t chipVersionNumber: 4;
				bool mode: 1;
				bool _: 1;
				bool externalLatchFlag: 1;
				bool interlaceField: 1;
			};
			uint8_t raw;
		} _stat78;
	};

	//! @brief The class containing all the registers of the PPU
	class PPU : public Memory::AMemory {
	public:
		//! @brief Rams
		std::shared_ptr<Ram::Ram> vram;
		std::shared_ptr<Ram::Ram> oamram;
		std::shared_ptr<Ram::Ram> cgram;
	private:
		//! @brief Init ppuRegisters
		Registers _registers{};
		Renderer::IRenderer &_renderer;
		//! @brief Backgrounds buffers
		Background _backgrounds[8];
		//! @brief Main Screen buffer
		std::array<std::array<uint32_t, 1024>, 1024> _mainScreen;
		//! @brief Sub Screen buffer
		std::array<std::array<uint32_t, 1024>, 1024> _subScreen;
		//! @brief Final Screen buffer
		std::array<std::array<uint32_t, 1024>, 1024> _screen;
	public:

		explicit PPU(Renderer::IRenderer &renderer);
		PPU(const PPU &) = delete;
		PPU &operator=(const PPU &) = delete;
		~PPU() override = default;

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
		//! @brief Get the name of this accessor (used for debug purpose)
		std::string getName() override;
		//! @brief Get the component of this accessor (used for debug purpose)
		Component getComponent() override;

		//! @brief Update the PPU of n cycles.
		//! @param The number of cycles to update.
		virtual void update(unsigned cycles);
		//! @brief Give the Vram Address with the right Address remapping
		uint16_t getVramAddress();
		//! @brief Give the name of the Address register (used for debug)
		std::string getValueName(uint24_t addr);
		//! @brief Return true if the CPU is overloaded with debugging features.
		virtual bool isDebugger();
		//! @brief Allow others components to read the CGRAM
		uint16_t cgramRead(uint16_t addr);
		//! @brief get the bpp depending of the bgNumber and the Bgmode
		int getBPP(int bgNumber);
		//! @brief Give the correct character size depending of the bgMode
		Vector2<int> getCharacterSize(int bgNumber);
		//! @brief Give the address where the tilemap starts
		uint16_t getTileMapStartAddress(int bgNumber);
		//! @brief Give the address to find the correct tileset for a given x and y
		uint16_t getTileSetAddress(int bgNumber);
		//! @brief Give the number of tilemaps to be rendered
		Vector2<int> getBackgroundSize(int bgNumber);
		//! @brief Render the Main and sub screen correctly
		void renderMainAndSubScreen(void);
		//! @brief Add a bg buffer to another buffer
		template <std::size_t DEST_SIZE, std::size_t SRC_SIZE>
		void add_buffer(std::array<std::array<uint32_t, DEST_SIZE>, DEST_SIZE> &bufferDest, std::array<std::array<uint32_t, SRC_SIZE>, SRC_SIZE> &bufferSrc);
		//! @brief Add a bg to the sub and/or main screen
		void addToMainSubScreen(Background &bg);
	};
}
#endif //COMSQUARE_PPU_HPP
