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
		union {
			uint8_t mask;
			struct {
				uint8_t headsetOn : 1;
				uint8_t worn : 1;
				uint8_t cinematic : 1;
				uint8_t maskreserved0 : 1;
				uint8_t headphones : 1;
				uint8_t mute : 1;
				uint8_t cec : 1;
				uint8_t maskreserved1 : 1;
			};
		};
		uint32_t volume;
		uint16_t reserved0;
		union {
			uint8_t bridgeOutputID;
			uint8_t reserved1;
		};
	};
	uint8_t raw[8];
};
#pragma pack()