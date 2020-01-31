//
// Created by Melefo on 28/01/2020.
//

#ifndef COMSQUARE_DSP_HPP
#define COMSQUARE_DSP_HPP

#include <cstdint>
#include <SFML/Audio.hpp>
#include "../Memory/IMemory.hpp"

namespace ComSquare::APU::DSP
{
	//! @brief All the registers of the DSP
	struct Registers {
		//! @brief Left channel volume register
		uint8_t volL[8];
		//! @brief Left channel volume register
		uint8_t volR[8];

		//! @brief Lower 8 bits of pitch register
		uint8_t pitchL[8];
		//! @brief Higher 8 bits of pitch register
		uint8_t pitchH[8];

		//! @brief Source number register
		uint8_t srcn[8];

		//! @brief Envelope register
		uint8_t adsr1[8];
		//! @brief Envelope controllers register
		uint8_t adsr2[8];
		//! @brief Gain register
		uint8_t gain[8];
		//! @brief Envelope value register
		uint8_t envx[8];
		//! @brief Wave height register
		uint8_t outx[8];

		//! @brief Left output of the Main Volume register
		uint8_t mvolL;
		//! @brief Right output of the Main Volume register
		uint8_t mvolR;

		//! @brief Left output of the Echo Volume register
		uint8_t evolL;
		//! @brief Right output of the Echo Volume register
		uint8_t evolR;

		//! @brief Key On register
		union {
			struct {
				bool kon7 : 1;
				bool kon6 : 1;
				bool kon5 : 1;
				bool kon4 : 1;
				bool kon3 : 1;
				bool kon2 : 1;
				bool kon1 : 1;
				bool kon0 : 1;
			};
			uint8_t kon;
		};
		//! @brief Key Off register
		union {
			struct {
				bool kof7 : 1;
				bool kof6 : 1;
				bool kof5 : 1;
				bool kof4 : 1;
				bool kof3 : 1;
				bool kof2 : 1;
				bool kof1 : 1;
				bool kof0 : 1;
			};
			uint8_t kof;
		};

		//! @brief Flags register
		uint8_t flg;

		//! @brief Sample end register
		union {
			struct {
				bool endx7 : 1;
				bool endx6 : 1;
				bool endx5 : 1;
				bool endx4 : 1;
				bool endx3 : 1;
				bool endx2 : 1;
				bool endx1 : 1;
				bool endx0 : 1;
			};
			uint8_t endx;
		};

		//! @brief Echo feedback register
		uint8_t efb;

		//! @brief Not used register
		uint8_t unused;

		//! @brief Pitch modulation register
		union {
			struct {
				bool pmon7 : 1;
				bool pmon6 : 1;
				bool pmon5 : 1;
				bool pmon4 : 1;
				bool pmon3 : 1;
				bool pmon2 : 1;
				bool pmon1 : 1;
				bool __ : 1;
			};
			uint8_t pmon;
		};

		//! @brief Noise enable register
		union {
			struct {
				bool non7 : 1;
				bool non6 : 1;
				bool non5 : 1;
				bool non4 : 1;
				bool non3 : 1;
				bool non2 : 1;
				bool non1 : 1;
				bool non0 : 1;
			};
			uint8_t non;
		};
		//! @brief Echo enable register
		union {
			struct {
				bool eon7 : 1;
				bool eon6 : 1;
				bool eon5 : 1;
				bool eon4 : 1;
				bool eon3 : 1;
				bool eon2 : 1;
				bool eon1 : 1;
				bool eon0 : 1;
			};
			uint8_t eon;
		};
		//! @brief Source Directory offset register
		uint8_t dir;

		//! @brief Echo data start register
		uint8_t esa;
		//! @brief Echo delay size register
		uint8_t edl;
		//! @brief Echo FIR filter coefficients
		uint8_t coeff[8];
	};

	class DSP : public Memory::IMemory {
	private:
		//! @brief All registers of the DSP
		Registers _registers{};

		//! @brief 8x channels of sample used to make sound
		sf::Sound _channels[8];

		//! @brief A buffer containing current wave
		sf::SoundBuffer _soundBuffer;
	public:
		explicit DSP();

		//! @brief Read from the internal DSP register.
		//! @param addr The address to read from. The address 0x0 should refer to the first byte of the register.
		//! @throw InvalidAddress will be thrown if the address is more than $7F (the number of register).
		//! @return Return the value of the register.
		uint8_t read(uint24_t addr) override;
		//! @brief Write data to the internal DSP register.
		//! @param addr The address to write to. The address 0x0 should refer to the first byte of register.
		//! @param data The new value of the register.
		//! @throw InvalidAddress will be thrown if the address is more than $7F (the number of register).
		void write(uint24_t addr, uint8_t data) override;
	};
}

#endif //COMSQUARE_DSP_HPP
