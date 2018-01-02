#pragma once

#include "PSVR_Common.h"

#pragma pack(1)
union psvr_device_info {
	struct psvr_device_info_struct {
		unsigned short int reserved0;
		struct {
			uint8_t minor;
			uint8_t major;
		} version;
		unsigned short int reserved1;
		uint32_t reserved2;
 		unsigned short int reserved3;
		uint8_t serialNumber[16];
		uint32_t reserved4[5];
	};
	uint8_t raw[48];
};
#pragma pack()
