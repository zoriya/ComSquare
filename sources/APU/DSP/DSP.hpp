//
// Created by Melefo on 28/01/2020.
//

#ifndef COMSQUARE_DSP_HPP
#define COMSQUARE_DSP_HPP

#include <cstdint>
#include <array>
#include "../../Memory/AMemory.hpp"

namespace ComSquare::APU::DSP
{
	//! @brief All the registers of the DSP
	struct Registers {
		//! @brief Left output of the Main Volume register
		uint8_t mvolL;
		//! @brief Right output of the Main Volume register
		uint8_t mvolR;

		//! @brief Left output of the Echo Volume register
		uint8_t evolL;
		//! @brief Right output of the Echo Volume register
		uint8_t evolR;

		//! @brief Flags register
		uint8_t flg;

		//! @brief Echo feedback register
		uint8_t efb;

		//! @brief Not used register
		uint8_t unused;

		//! @brief Source Directory offset register
		uint8_t dir;

		//! @brief Echo data start register
		uint8_t esa;
		//! @brief Echo delay size register
		uint8_t edl;
	};

	struct Channel {
		//! @brief BRR Header
		union {
			struct {
				//! @brief Shift value range
				unsigned range : 4;
				//! @brief Decompression filter
				unsigned filter : 2;
				//! @brief Flag if the sample loops
				bool loop : 1;
				//! @brief Stop the sample (or restart from loop point)
				bool end : 1;
			};
			uint8_t brrHead;
		};
		//! @brief Sample data inside BRR
		uint64_t brrData;

		//! @brief Left channel volume register
		uint8_t volL;
		//! @brief Left channel volume register
		uint8_t volR;

		union {
			struct {
				//! @brief Lower 8 bits of pitch register
				uint8_t pitchL;
				//! @brief Higher 8 bits of pitch register
				uint8_t pitchH;
			};
			uint16_t pitch;
		};

		//! @brief Key On register
		bool kon : 1;
		//! @brief Key Off register
		bool kof : 1;

		//! @brief Sample end register
		bool endx : 1;

		//! @brief Noise enable register
		bool non : 1;
		//! @brief Echo enable register
		bool eon : 1;

		//! @brief Pitch modulation register
		bool pmon : 1;

		//! @brief Source number register
		uint8_t srcn;

		union {
			struct {
				//! @brief Envelope register
				uint8_t adsr1;
				//! @brief Envelope controllers register
				uint8_t adsr2;
			};
			uint16_t envelope;
		};
		//! @brief Gain register
		uint8_t gain;
		//! @brief Envelope value register
		uint8_t envx;
		//! @brief Wave height register
		uint8_t outx;

		//! @brief Echo FIR filter coefficients
		uint8_t coeff;
	};

	class DSP : public Memory::AMemory {
	private:
		//! @brief All registers of the DSP
		Registers _registers{};

		//! @brief 8x channels of sample used to make sound
		std::array<Channel, 8> _channels{};
	public:
		DSP() = default;
		DSP(const DSP &) = default;
		DSP &operator=(const DSP &) = default;
		~DSP() override = default;

		Registers getRegisters();

		std::array<Channel, 8> getChannels();

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

		//! @brief Get the name of this accessor (used for debug purpose)
		std::string getName() override;
	};
}

#endif //COMSQUARE_DSP_HPP
