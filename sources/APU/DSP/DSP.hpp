//
// Created by Melefo on 28/01/2020.
//

#pragma once

#include <cstdint>
#include <array>
#include "Renderer/IRenderer.hpp"
#include "Memory/AMemory.hpp"

namespace ComSquare::APU
{
	class APU;
	struct MemoryMap;
}

namespace ComSquare::APU::DSP
{
	//! @brief The 4 states of volume envelope adjustment
	enum Envelope : uint {
		//! @brief The voice is keyed off or a BRR end-without-loop block is reached
		Release,
		//! @brief The voice is keyed on
		Attack,
		//! @brief When the Envelope adjustment method bits exceeds 0x7ff
		Decay,
		//! @brief When the upper 3 bits of Envelope adjustment method bits equal the Sustain Level
		Sustain
	};

	struct Master {
		//! @brief Main Volume register (MVOL)
		std::array<uint8_t, 2> volume;
		//! @brief Mutes all channel (6th bit FLG)
		bool mute : 1;
		//! @brief Soft reset DSP (7th bit FLG)
		bool reset : 1;
		//! @brief Current sound produced
		std::array<uint16_t, 2> output;
		//! @brief Not used register
		uint8_t unused;
	};

	struct Echo {
		//! @brief Echo Volume register (EVOL)
		std::array<uint8_t , 2> volume;
		//! @brief Echo feedback register (EFB)
		uint8_t feedback;
		//! @brief Echo FIR filter coefficients (COEF)
		std::array<uint8_t, 8> FIR;
		//! @brief Echo data start register (ESA)
		uint8_t data;
		//! @brief Offset position after data start
		uint16_t offset;
		//! @brief offset maximum
		uint16_t length;
		//! @brief Echo delay size register (EDL)
		uint8_t delay;
		//! @brief Echo enabled (5th bit FLG)
		bool enabled = true;
		//! @brief Application of enabled to channels.
		bool toggle;
		//! @brief Last sound produced for each voice in each channel
		std::array<std::array<int16_t, 8>, 2> history;
		//! @brief Current position inside history
		uint8_t historyOffset;
		//! @brief Address of the current echo
		uint16_t address;
		//! @brief Current of value of the echo
		uint8_t value;
		//! @brief Current sound to echo
		std::array<uint16_t, 2> input;
		//! @brief Current sound echoed produced
		std::array<uint16_t, 2> output;
	};

	struct Noise {
		//! @brief Frequency of white noise (the first 4 bits FLG)
		uint8_t clock : 5;
		//! @brief Linear feedback shift register used to shift final output
		uint16_t lfsr = 0x4000;
	};

	struct BRR {
		//! @brief Offset pointing to sample directory in external RAM (DIR)
		uint8_t offset;
		//! @brief Address of the offset
		uint8_t offsetAddr;
		//! @brief Current address of the BRR in APU's RAM
		uint16_t address;
		//! @brief Next address of the BRR in APU's RAM
		uint16_t nextAddress;
		//! @brief Current value inside BRR
		uint8_t value;
		//! @brief Current header of BRR
		uint8_t header;
		//! @brief Current value of Voice ADSR1 loaded
		uint8_t source;
	};

	struct Latch {
		//! @brief Current voice's adsr1 in use
		uint8_t adsr1;
		//! @brief Envelope value register (ENVX)
		uint8_t envx;
		//! @brief Wave height register (OUTX)
		uint8_t outx;
		//! @brief Current voice's pitch in use
		uint16_t pitch;
		//! @brief Output currently being modified
		uint16_t output;
	};

	struct Voice {
		//! @brief Volume register (VOL)
		std::array<int8_t, 2> volume;
		//! @brief Pitch register (P)
		union {
			struct {
				//! @brief Lower 8 bits of pitch register
				uint8_t pitchL;
				//! @brief Higher 8 bits of pitch register
				uint8_t pitchH;
			};
			uint16_t pitch;
		};
		//! @brief Source number register (SRCN)
		uint8_t srcn;
		union {
			struct {
				//! @brief Envelope register (ADSR)
				uint8_t adsr1;
				//! @brief Envelope controllers register (ADSR)
				uint8_t adsr2;
			};
			uint16_t adsr;
		};
		//! @brief Gain register (GAIN)
		uint8_t gain;
		//! @brief envelope associated with this voice
		uint8_t envx;
		//! @brief Wave height associated with this voice
		uint8_t outx;
		//! @brief Sample end register (ENDX)
		bool endx : 1;

		//! @brief Key On register (KON)
		bool kon : 1;
		//! @brief Key Off register (KOF)
		bool kof : 1;
		//! @brief Pitch modulation register (PMON)
		bool pmon : 1;
		//! @brief Noise enable register (NON)
		bool non : 1;
		//! @brief Echo enable register (EON)
		bool eon : 1;

		//! @brief Check if voice is in setup phase
		uint8_t konDelay;
		//! @brief Check if the output will be echoed
		bool echo;
		//! @brief Check if this voice will be looped
		bool loop;
		//! @brief Current BRR associated with this voice
		uint16_t brrAddress;
		//! @brief Current Offset in the BRR block
		uint8_t brrOffset = 1;
		//! @brief Previous modulation
		bool prevPmon : 1;
		//! @brief temporary NON register value
		bool tempNon : 1;
		//! @brief temporary Key On register value
		bool tempKon : 1;
		//! @brief temporary Key Off register value
		bool tempKof : 1;
		//! @brief all samples Decoded from BRR
		std::array<uint16_t, 12> samples;
		//! @brief Offset of current sample in samples buffer
		uint8_t sampleOffset;
		//! @brief Current envelope level
		uint16_t envelope;
		//! @brief Second envelope level used to make "special" waveforms
		uint16_t hiddenEnvelope;
		//! @brief current envelope Mode
		Envelope envelopeMode;
		//! @brief Relative fractional position in sample
		uint16_t gaussOffset;
	};

	//! @brief Current state of the DSP
	struct State
	{
		State(std::array<int16_t, 0x10000> &array, uint32_t size)
			: buffer(array), bufferSize(size)
		{};

		//! @brief Current voice modification to do
		uint8_t voice = 0;
		//! @brief Current buffer of samples
		std::array<int16_t, 0x10000> &buffer;
		//! @brief Size of buffer
		uint32_t bufferSize;
		//! @brief Current position in the buffer of samples
		uint32_t bufferOffset = 0;
	};

	struct Timer {
		//! @brief Ticks remaining in the timer
		uint16_t counter;
		//! @brief output every samples
		bool sample = true;
	};

	class DSP {
	private:
		//! @brief Number of samples per counter event
		std::array<uint16_t, 32> _rateModulus = {
				0, 2048, 1536, 1280, 1024, 768,
				640, 512, 384, 320, 256, 192,
				160, 128, 96, 80, 64, 48,
				40, 32, 24, 20, 16, 12,
				10, 8, 6, 5, 4, 3,
				2, 1
		};

		//! @brief Counter offset
		std::array<uint16_t, 32> _counterOffset = {
				0, 0, 1040, 536, 0, 1040,
				536, 0, 1040, 536, 0, 1040,
				536, 0, 1040, 536, 0, 1040,
				536, 0, 1040, 536, 0, 1040,
				536, 0, 1040, 536, 0, 1040,
				0,0
		};

		//! @brief Gaussian table used for making waves
		std::array<int16_t, 512> _gauss = {
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
			2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 5,
			6, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 10,
			11, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 15, 16, 16, 17, 17,
			18, 19, 19, 20, 20, 21, 21, 22, 23, 23, 24, 24, 25, 26, 27, 27,
			28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 36, 36, 37, 38, 39, 40,
			41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56,
			58, 59, 60, 61, 62, 64, 65, 66, 67, 69, 70, 71, 73, 74, 76, 77,
			78, 80, 81, 83, 84, 86, 87, 89, 90, 92, 94, 95, 97, 99, 100, 102,
			104, 106, 107, 109, 111, 113, 115, 117, 118, 120, 122, 124, 126, 128, 130, 132,
			134, 137, 139, 141, 143, 145, 147, 150, 152, 154, 156, 159, 161, 163, 166, 168,
			171, 173, 175, 178, 180, 183, 186, 188, 191, 193, 196, 199, 201, 204, 207, 210,
			212, 215, 218, 221, 224, 227, 230, 233, 236, 239, 242, 245, 248, 251, 254, 257,
			260, 263, 267, 270, 273, 276, 280, 283, 286, 290, 293, 297, 300, 304, 307, 311,
			314, 318, 321, 325, 328, 332, 336, 339, 343, 347, 351, 354, 358, 362, 366, 370,
			374, 378, 381, 385, 389, 393, 397, 401, 405, 410, 414, 418, 422, 426, 430, 434,
			439, 443, 447, 451, 456, 460, 464, 469, 473, 477, 482, 486, 491, 495, 499, 504,
			508, 513, 517, 522, 527, 531, 536, 540, 545, 550, 554, 559, 563, 568, 573, 577,
			582, 587, 592, 596, 601, 606, 611, 615, 620, 625, 630, 635, 640, 644, 649, 654,
			659, 664, 669, 674, 678, 683, 688, 693, 698, 703, 708, 713, 718, 723, 728, 732,
			737, 742, 747, 752, 757, 762, 767, 772, 777, 782, 787, 792, 797, 802, 806, 811,
			816, 821, 826, 831, 836, 841, 846, 851, 855, 860, 865, 870, 875, 880, 884, 889,
			894, 899, 904, 908, 913, 918, 923, 927, 932, 937, 941, 946, 951, 955, 960, 965,
			969, 974, 978, 983, 988, 992, 997, 1001, 1005, 1010, 1014, 1019, 1023, 1027, 1032, 1036,
			1040, 1045, 1049, 1053, 1057, 1061, 1066, 1070, 1074, 1078, 1082, 1086, 1090, 1094, 1098, 1102,
			1106, 1109, 1113, 1117, 1121, 1125, 1128, 1132, 1136, 1139, 1143, 1146, 1150, 1153, 1157, 1160,
			1164, 1167, 1170, 1174, 1177, 1180, 1183, 1186, 1190, 1193, 1196, 1199, 1202, 1205, 1207, 1210,
			1213, 1216, 1219, 1221, 1224, 1227, 1229, 1232, 1234, 1237, 1239, 1241, 1244, 1246, 1248, 1251,
			1253, 1255, 1257, 1259, 1261, 1263, 1265, 1267, 1269, 1270, 1272, 1274, 1275, 1277, 1279, 1280,
			1282, 1283, 1284, 1286, 1287, 1288, 1290, 1291, 1292, 1293, 1294, 1295, 1296, 1297, 1297, 1298,
			1299, 1300, 1300, 1301, 1302, 1302, 1303, 1303, 1303, 1304, 1304, 1304, 1304, 1304, 1305, 1305
		};

		//! @brief Buffer containing samples to be played
		std::array<int16_t, 0x10000> _soundBuffer = {};
		//! @brief 8x voices of sample used to make sound
		std::array<Voice, 8> _voices {};
		Master _master {};
		Echo _echo {};
		Noise _noise {};
		BRR _brr {};
		Latch _latch {};
		State _state;
		Timer _timer {};

		void voiceOutput(Voice &voice, bool channel);
		void voice1(Voice &voice);
		void voice2(Voice &voice);
		void voice3(Voice &voice);
		void voice3a(Voice &voice);
		void voice3b(Voice &voice);
		void voice3c(Voice &voice);
		void voice4(Voice &voice);
		void voice5(Voice &voice);
		void voice6(Voice &voice);
		void voice7(Voice &voice);
		void voice8(Voice &voice);
		void voice9(Voice &voice);
		void echo22();
		void echo23();
		void echo24();
		void echo25();
		void echo26();
		void echo27();
		void echo28();
		void echo29();
		void echo30();
		void misc27();
		void misc28();
		void misc29();
		void misc30();

		//! @brief Interpolate voice samples with gauss table
		int32_t interpolate(const Voice &voice);
		//! @brief Modify voice samples with its envelope
		void runEnvelope(Voice &voice);

		int32_t loadFIR(bool channel, int fir);
		void loadEcho(bool channel);
		int16_t outputEcho(bool channel);
		void writeEcho(bool channel);

		//! @brief Remove one tick from timer
		void timerTick();
		//! @brief Check if timer value is equal to rate value
		bool timerPoll(uint32_t rate);

		//! @brief Transform BRR value to samples
		void decodeBRR(Voice &voice);

		//! @brief Whole APU RAM map
		MemoryMap &_map;

		//! @brief Renderer used to play sounds
		Renderer::IRenderer &_renderer;

		//! @brief Read inside APU RAM
		uint8_t _readRAM(uint24_t addr);
		//! @brief Write into APU RAM
		void _writeRAM(uint24_t addr, uint8_t data);
	public:
		DSP(Renderer::IRenderer &renderer, MemoryMap &map);
		DSP(const DSP &) = default;
		DSP &operator=(const DSP &) = delete;
		~DSP() = default;

		//! @brief Return all 8 voices from DSP
		[[nodiscard]] const std::array<Voice, 8> &getVoices() const;
		[[nodiscard]] const Master &getMaster() const;
		[[nodiscard]] const Echo &getEcho() const;
		[[nodiscard]] const Noise &getNoise() const;
		[[nodiscard]] const BRR &getBrr() const;
		[[nodiscard]] const Latch &getLatch() const;

		//! @brief Read from the internal DSP register.
		//! @param addr The address to read from. The address 0x0 should refer to the first byte of the register.
		//! @throw InvalidAddress will be thrown if the address is more than $7F (the number of register).
		//! @return Return the value of the register.
		[[nodiscard]] uint8_t read(uint24_t addr) const;
		//! @brief Write data to the internal DSP register.
		//! @param addr The address to write to. The address 0x0 should refer to the first byte of register.
		//! @param data The new value of the register.
		//! @throw InvalidAddress will be thrown if the address is more than $7F (the number of register).
		void write(uint24_t addr, uint8_t data);

		//! @brief Get the name of this accessor (used for debug purpose)
		[[nodiscard]] std::string getName() const;
		//! @brief Execute current voice transformation
		void update();

		//! @brief Get the component of this accessor (used for debug purpose)
		[[nodiscard]] Component getComponent() const;

		//! @brief Get the size of the data. This size can be lower than the mapped data.
		//! @return The number of bytes inside this memory.
		[[nodiscard]] uint24_t getSize() const;
		//! @brief Return the number of samples written
		[[nodiscard]] int32_t getSamplesCount() const;
	};
}