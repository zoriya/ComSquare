//
// Created by anonymus-raccoon on 2/13/20.
//

#ifndef COMSQUARE_EXTENDEDRAM_HPP
#define COMSQUARE_EXTENDEDRAM_HPP

#include <cstddef>
#include <cstdint>
#include "../Models/Int24.hpp"

namespace ComSquare::Ram
{
	class ExtendedRam {
	private:
		uint16_t *_data;
		size_t _size;
	public:
		explicit ExtendedRam(size_t size);
		ExtendedRam(const ExtendedRam &) = delete;
		ExtendedRam &operator=(const ExtendedRam &) = delete;
		~ExtendedRam();

		uint16_t read(uint24_t addr);
		void write(uint24_t addr, uint16_t data);
	};
}

#endif //COMSQUARE_EXTENDEDRAM_HPP
