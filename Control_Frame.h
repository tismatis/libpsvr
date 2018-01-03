#pragma once

#include "PSVR_Common.h"

#include "Device_Info.h"
#include "Device_Status.h"
#include "Unsolicited_Report.h"

#pragma pack(1)
union psvr_control_frame {
	struct psvr_control_frame_struct {
		uint8_t id;
		uint8_t status;
		uint8_t start;
		uint8_t length;
		union {
			uint8_t data[60];
			union psvr_device_info2 dinfo; //id == eRT_Info
			union psvr_device_status dstatus; //id == eRT_Status
			union psvr_unsolicited_report ureport; //id == eRT_Unsolicited
		};
	};
	uint8_t raw[64];
};
#pragma pack()