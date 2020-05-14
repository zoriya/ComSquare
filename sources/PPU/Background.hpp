//
// Created by cbihan on 5/14/20.
//

#ifndef COMSQUARE_BACKGROUND_HPP
#define COMSQUARE_BACKGROUND_HPP

#include <stdint-gcc.h>
#include <array>
#include "../Models/Vector2.hpp"

class Background {
	int width;
	int height;
	int bpp;
	bool directColor;
	bool highRes;
	std::array<std::array<uint16_t, 1024>, 1024> buffer;
};


#endif //COMSQUARE_BACKGROUND_HPP
