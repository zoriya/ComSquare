//
// Created by cbihan on 1/27/20.
//

#ifndef COMSQUARE_PPU_HPP
#define COMSQUARE_PPU_HPP

#include <stdint-gcc.h>
#include "../Memory/IMemory.hpp"

namespace ComSquare::PPU
{
	//! @brief The struct containing all the registers the PPU
	class PPU : public IMemory {
	private:
		//! @brief INIDISP Register (F-blank and Brightness)
		union {
			struct {
				bool fblank: 1;
				bool _: 3;
				uint8_t brightness: 4;
			};
			uint8_t raw;
		} inidisp;
		//! @brief OBSEL Register (Object Size and Character Address)
		union {
			struct {
				uint8_t objectSize: 3;
				bool nameSelect: 2;
				uint8_t baseSelect: 3;
			};
			uint8_t raw;
		} obsel;
		//! @brief OAMADD Register (OAM Address and Obj Priority)
		union {
			struct {
				bool objPriorityActivationBit: 1;
				uint8_t _: 6;
				uint32_t oamAddress: 9;
			};
			struct {
				uint8_t oamaddh;
				uint8_t oamaddl;
			};
			uint32_t raw;
		} oamadd;
		//! @brief OAMDATA Register (Data for OAM write)
		uint8_t oamdata;
		//! @brief BGMODE Register (OAM Address and Obj Priority)
		union {
			struct {
				bool characterSizeBg4: 1;
				bool characterSizeBg3: 1;
				bool characterSizeBg2: 1;
				bool characterSizeBg1: 1;
				bool mode1Bg3PriorityBit: 1;
				uint8_t bgMode: 3;
			};
			uint8_t raw;
		} bgmode;
		//! @brief MOSAIC Register (Screen Pixelation)
		union {
			struct {
				uint8_t pixelSize: 4;
				bool affectBg4: 1;
				bool affectBg3: 1;
				bool affectBg2: 1;
				bool affectBg1: 1;
			};
			uint8_t raw;
		} mosaic;
		//! @brief BG1SC Register (BG1 Tilemap Address and Size)
		union {
			struct {
				uint8_t tilemapAddress: 6;
				bool tilemapHorizontalMirroring: 1;
				bool tilemapVerticalMirroring: 1;
			};
			uint8_t raw;
		} bg1sc;	
		//! @brief BG2SC Register (BG2 Tilemap Address and Size)
		union {
			struct {
				uint8_t tilemapAddress: 6;
				bool tilemapHorizontalMirroring: 1;
				bool tilemapVerticalMirroring: 1;
			};
			uint8_t raw;
		} bg2sc;
		//! @brief BG3SC Register (BG3 Tilemap Address and Size)
		union {
			struct {
				uint8_t tilemapAddress: 6;
				bool tilemapHorizontalMirroring: 1;
				bool tilemapVerticalMirroring: 1;
			};
			uint8_t raw;
		} bg3sc;
		//! @brief BG4SC Register (BG4 Tilemap Address and Size)
		union {
			struct {
				uint8_t tilemapAddress: 6;
				bool tilemapHorizontalMirroring: 1;
				bool tilemapVerticalMirroring: 1;
			};
			uint8_t raw;
		} bg4sc;
		//! @brief BG12NBA Register (BG1 and 2 Chr Address)
		union {
			struct {
				uint8_t baseAddressBg2a4: 4;
				uint8_t baseAddressBg1a3: 4;
			};
			uint8_t raw;
		} bg12nba;
		//! @brief BG34NBA Register (BG3 and 4 Chr Address)
		union {
			struct {
				uint8_t baseAddressBg2a4: 4;
				uint8_t baseAddressBg1a3: 4;
			};
			uint8_t raw;
		} bg34nba;
		//! @brief BG1HOFS Register (BG1 Horizontal Scroll)
		//! @brief BG1VOFS Register (BG1 Vertical Scroll)
		union {
			struct {
				uint8_t _ : 6;
				uint32_t offsetBg: 10;
			};
			uint16_t raw;
		} bg1ofs;
		//! @brief M7HOFS Register (Mode 7 BG Horizontal Scroll)
		//! @brief M7VOFS Register (Mode 7 BG Vertical Scroll)
		union {
			struct {
				uint8_t _ : 3;
				uint32_t offsetBg : 13;
			};
			uint8_t raw;
		} m7ofs;
		//! @brief BG2HOFS Register (BG2 Horizontal Scroll)
		//! @brief BG2VOFS Register (BG2 Vertical Scroll)
		union {
			struct {
				uint8_t _ : 6;
				uint32_t offsetBg: 10;
			};
			uint8_t raw;
		} bg2ofs;
		//! @brief BG3HOFS Register (BG3 Horizontal Scroll)
		//! @brief BG3VOFS Register (BG3 Vertical Scroll)
		union {
			struct {
				uint8_t _ : 6;
				uint32_t offsetBg: 10;
			};
			uint8_t raw;
		} bg3ofs;
		//! @brief BG4HOFS Register (BG4 Horizontal Scroll)
		//! @brief BG4VOFS Register (BG4 Vertical Scroll)
		union {
			struct {
				uint8_t _ : 3;
				uint32_t offsetBg: 10;
			};
			uint8_t raw;
		} bg4ofs;
		//! @brief VMAIN Register (Video Port Control)
		union {
			struct {
				bool address: 1;
				uint8_t _ : 3;
				uint8_t addressRemapping: 2;
				uint8_t incrementCount: 2;
			};
			uint8_t raw;
		} vmain;
		//! @brief VMADD Register (VRAM Address)
		union {
			struct {
				uint8_t vmaddh;
				uint8_t vmaddl;
			};
			uint32_t vmadd;
		} vmadd;
		//! @brief VMDATA Register (VRAM Data Write)
		union {
			struct {
				uint8_t vmdatah;
				uint8_t vmdatal;
			};
			uint32_t vmdata;
		} vmdata;
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
		} m7sel;
		//! M7A M7B M7C M7D i didn't understand how they works so they will be added later.
		//! @brief M7X Register (Mode 7 Center X)
		union {
			struct {
				uint8_t _: 3;
				uint8_t value;
			};
			uint32_t center;
		} m7x;
		//! @brief M7Y Register (Mode 7 Center Y)
		union {
			struct {
				uint8_t _: 3;
				uint8_t value;
			};
			uint32_t center;
		} m7y;
		//! @brief CGADD Register (CGRAM Address)
		uint8_t cgadd;
		//! @brief CGDATA Register (CGRAM Data write)
		union {
			struct {
				bool _: 1;
				uint8_t blue: 5;
				uint8_t green: 5;
				uint8_t red: 5;
			};
			uint16_t raw;
		} cgdata;
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
		} wsel;
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
	};
}
#endif //COMSQUARE_PPU_HPP
