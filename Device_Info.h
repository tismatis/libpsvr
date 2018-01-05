#pragma once

#include "PSVR_Common.h"

#pragma pack(1)
union psvr_device_info_union {
	struct psvr_device_info_struct {
		uint16_t reserved0; //0,1
		union {
			struct {
				uint8_t minor; //2
				uint8_t major; //3
			} version;
		};
		uint16_t reserved1; //4, 5
		uint32_t reserved2; //6, 7, 8, 9
		uint16_t reserved3; //10, 11
		union {
			uint8_t serialNumber[16]; //12 - 27
		};
		uint32_t reserved4[5]; //28 - 48

	#ifdef __cplusplus  
	} s;
	#else
	};
	#endif

	uint8_t raw[48];
};
#pragma pack()

typedef union psvr_device_info_union psvr_device_info;