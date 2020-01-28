//
// Created by cbihan on 1/27/20.
//

#ifndef COMSQUARE_PPU_HPP
#define COMSQUARE_PPU_HPP

namespace ComSquare::PPU
{
    //! @brief The struct containing all the registers the PPU
    class PPU {
    private:
        //! @brief INIDISP Register (F-blank and Brightness)
        union {
            struct {
                bool fblank: 1;
                bool _: 3;
                unsigned char brightness: 4;
            };
            unsigned char raw;
        } inidisp;
        //! @brief OBSEL Register (Object Size and Character Address)
        union {
            struct {
                unsigned char objectSize: 3;
                bool nameSelect: 2;
                unsigned char baseSelect: 3;
            };
            unsigned char raw;
        } obsel;
        //! @brief OAMADD Register (OAM Address and Obj Priority)
        union {
            struct {
                bool objPriorityActivationBit: 1;
                unsigned char _: 6;
                unsigned short oamAddress: 9;
            };
            struct {
                unsigned char oamaddh;
                unsigned char oamaddl;
            };
            unsigned short raw;
        } oamadd;
        //! @brief OAMDATA Register (Data for OAM write)
        unsigned char oamdata;
        //! @brief BGMODE Register (OAM Address and Obj Priority)
        union {
            struct {
                bool characterSizeBg4: 1;
                bool characterSizeBg3: 1;
                bool characterSizeBg2: 1;
                bool characterSizeBg1: 1;
                bool mode1Bg3PriorityBit: 1;
                unsigned char bgMode: 3;
            };
            unsigned char raw;
        } bgmode;
        //! @brief MOSAIC Register (Screen Pixelation)
        union {
            struct {
                unsigned char pixelSize: 4;
                bool affectBg4: 1;
                bool affectBg3: 1;
                bool affectBg2: 1;
                bool affectBg1: 1;
            };
            unsigned char raw;
        } mosaic;
        //! @brief BG1SC Register (BG1 Tilemap Address and Size)
        union {
            struct {
                unsigned char tilemapAddress: 6;
                bool tilemapHorizontalMirroring: 1;
                bool tilemapVerticalMirroring: 1;
            };
            unsigned char raw;
        } bg1sc;
        //! @brief BG2SC Register (BG2 Tilemap Address and Size)
        union {
            struct {
                unsigned char tilemapAddress: 6;
                bool tilemapHorizontalMirroring: 1;
                bool tilemapVerticalMirroring: 1;
            };
            unsigned char raw;
        } bg2sc;
        //! @brief BG3SC Register (BG3 Tilemap Address and Size)
        union {
            struct {
                unsigned char tilemapAddress: 6;
                bool tilemapHorizontalMirroring: 1;
                bool tilemapVerticalMirroring: 1;
            };
            unsigned char raw;
        } bg3sc;
        //! @brief BG4SC Register (BG4 Tilemap Address and Size)
        union {
            struct {
                unsigned char tilemapAddress: 6;
                bool tilemapHorizontalMirroring: 1;
                bool tilemapVerticalMirroring: 1;
            };
            unsigned char raw;
        } bg4sc;
        //! @brief BG12NBA Register (BG1 and 2 Chr Address)
        union {
            struct {
                unsigned char baseAddressBg2a4: 4;
                unsigned char baseAddressBg1a3: 4;
            };
            unsigned char raw;
        } bg12nba;
        //! @brief BG34NBA Register (BG3 and 4 Chr Address)
        union {
            struct {
                unsigned char baseAddressBg2a4: 4;
                unsigned char baseAddressBg1a3: 4;
            };
            unsigned char raw;
        } bg34nba;
        //! @brief BG1HOFS Register (BG1 Horizontal Scroll)
        //! @brief BG1VOFS Register (BG1 Vertical Scroll)
        union {
            struct {
                unsigned char _ : 6;
                unsigned short offsetBg: 10;
            };
            unsigned char raw;
        } bg1ofs;
        //! @brief M7HOFS Register (Mode 7 BG Horizontal Scroll)
        //! @brief M7VOFS Register (Mode 7 BG Vertical Scroll)
        union {
            struct {
                unsigned char _ : 3;
                unsigned short offsetBg : 13;
            };
            unsigned char raw;
        } m7ofs;
        //! @brief BG2HOFS Register (BG2 Horizontal Scroll)
        //! @brief BG2VOFS Register (BG2 Vertical Scroll)
        union {
            struct {
                unsigned char _ : 6;
                unsigned short offsetBg: 10;
            };
            unsigned char raw;
        } bg2ofs;
        //! @brief BG3HOFS Register (BG3 Horizontal Scroll)
        //! @brief BG3VOFS Register (BG3 Vertical Scroll)
        union {
            struct {
                unsigned char _ : 6;
                unsigned short offsetBg: 10;
            };
            unsigned char raw;
        } bg3ofs;
        //! @brief BG4HOFS Register (BG4 Horizontal Scroll)
        //! @brief BG4VOFS Register (BG4 Vertical Scroll)
        union {
            struct {
                unsigned char _ : 3;
                unsigned short offsetBg: 10;
            };
            unsigned char raw;
        } bg4ofs;
        //! @brief VMAIN Register (Video Port Control)
        union {
            struct {
                bool address: 1;
                unsigned char _ : 3;
                unsigned char addressRemapping: 2;
                unsigned char incrementCount: 2;
            };
            unsigned char raw;
        } vmain;
        //! @brief VMADD Register (VRAM Address)
        union {
            struct {
                unsigned char vmaddh;
                unsigned char vmaddl;
            };
            unsigned short vmadd;
        } vmadd;
        //! @brief VMDATA Register (VRAM Data Write)
        union {
            struct {
                unsigned char vmdatah;
                unsigned char vmdatal;
            };
            unsigned short vmdata;
        } vmdata;
        //! @brief M7SEL Register (Mode 7 Settings)
        union {
            struct {
                bool playingFieldSize: 1;
                bool emptySpaceFill: 1;
                unsigned char _: 4;
                bool horizontalMirroring: 1;
                bool verticalMirroring: 1;
            };
            unsigned char raw;
        } m7sel;
        //! M7A M7B M7C M7D i didn't understand how they works so they will be add later.
        //! @brief M7X Register (Mode 7 Center X)
        //! @brief M7Y Register (Mode 7 Center Y)
        union {
            struct {
                unsigned char _: 3;
                unsigned char value;
            };
            unsigned short center;
        } M7X;
    };
}
#endif //COMSQUARE_PPU_HPP
