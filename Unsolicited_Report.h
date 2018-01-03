#pragma once

#include "PSVR_Common.h"

enum psvr_unsolicited_report_code {
	eURC_Ok,
	eURC_UnknownReport,
	eURC_reserved,
	eURC_BadReportLength
};

#pragma pack(1)
union psvr_unsolicited_report {
	struct psvr_unsolicited_report_struct {
		uint8_t id;
		enum psvr_unsolicited_report_code code;
		uint8_t message[58];
	};
	uint8_t raw[60];
};
#pragma pack()