#pragma once

#include "PSVR_Common.h"

#include "Device_Info.h"
#include "Device_Status.h"
#include "Unsolicited_Report.h"

enum psvr_report_type {
	eRT_Info = 0x80,
	eRT_Status = 0xF0,
	eRT_Unsolicited = 0xA0
};

#pragma pack(1)
union psvr_control_frame_union {
	struct psvr_control_frame_struct {
		uint8_t r_id;
		uint8_t gp_id;
		uint8_t start;
		uint8_t length;
		union {
			uint8_t data[64];
			psvr_device_info dinfo; //id == eRT_Info
			psvr_device_status dstatus; //id == eRT_Status
			psvr_unsolicited_report ureport; //id == eRT_Unsolicited
		};

	#ifdef __cplusplus  
	} s;
	#else
	};
	#endif
	uint8_t raw[68]; //header is 4 bytes, data is 64 bytes, see morpheus.h.
};
#pragma pack()

typedef union psvr_control_frame_union psvr_control_frame;