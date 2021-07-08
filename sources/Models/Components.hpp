//
// Created by anonymus-raccoon on 3/24/20.
//

#pragma once

namespace ComSquare
{
	enum Component {
		Cpu = 1u << 0u,
		Ppu = 1u << 1u,
		Apu = 1u << 2u,
		Rom = 1u << 3u,
		WRam = 1u << 4u,
		VRam = 1u << 5u,
		OAMRam = 1u << 6u,
		CGRam = 1u << 7u,
		SRam = 1u << 8u,
	};
}