#pragma once

#include "PSVR_Common.h"

enum psvr_unsolicited_report_code {
	eURC_Ok,
	eURC_UnknownReport,
	eURC_BadGpID,
	eURC_BadReportLength
};

#pragma pack(1)
union psvr_unsolicited_report_union {
	struct psvr_unsolicited_report_struct {
		uint8_t id;
		enum psvr_unsolicited_report_code code;
		uint8_t message[58];

	#ifdef __cplusplus  
	} s;
	#else
	};
	#endif
	uint8_t raw[60];
};
#pragma pack()

typedef union psvr_unsolicited_report_union psvr_unsolicited_report;