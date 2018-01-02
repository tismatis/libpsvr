#pragma once

#include "PSVR_Common.h"

enum psvr_device_status_mask {
	eDSM_HeadsetOn =	(1 << 0),
	eDSM_Worn =			(1 << 1),
	eDSM_Cinematic =	(1 << 2),
	eDSM_reserved0 =	(1 << 3),
	eDSM_Headphones =	(1 << 4),
	eDSM_Mute =			(1 << 5),
	eDSM_CEC =			(1 << 6),
	eDSM_reserved1 =	(1 << 7)
};

#pragma pack(1)
union psvr_device_status {
	struct psvr_device_status_struct {
		uint8_t mask;
		uint32_t volume;
		unsigned short int reserved0;
		union {
			uint8_t bridgeOutputID;
			uint8_t reserved1;
		};
	};
	uint8_t raw[8];
};
#pragma pack()