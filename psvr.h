/*
 * Copyright (C) 2016 Adawarp Inc.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef __cplusplus  
extern "C" {
#endif

#include "ReportID.h"
#include "Control_Frame.h"
#include "Sensor_Frame.h"
#include "Device_Info.h"
#include "Device_Status.h"
#include "Unsolicited_Report.h"

#include "PSVR_Lib.h"

typedef struct psvr_context psvr_context;

//Logger function prototype.
typedef void(*psvr_log)(const char* msg, va_list args);

//Set the psvr logger.
//Defaults to printf if there is no logger.
PSVR_API void psvr_set_log(psvr_log* logger);

//Open a connection to the PSVR
//Sensor and Control interfaces are opened.
PSVR_API int psvr_open(psvr_context **ctx);

//Open a connection to the PSVR
//Can specify which interfaces to open based on mask.
PSVR_API int psvr_open_ex(
	psvr_context **ctx,
	int interfaces_to_claim
);

//Close a connection to the PSVR
PSVR_API void psvr_close(psvr_context *ctx);

//Send a command to a specific interface
PSVR_API int psvr_send_sync(
	enum morpheus_usb_interfaces interface,
	psvr_context *ctx,
	uint8_t id,
	uint8_t *payload,
	uint32_t length
);

//Send a raw command to a specific interface
PSVR_API int psvr_send_raw_sync(
	enum morpheus_usb_interfaces interface,
	psvr_context *ctx,
	struct morpheus_control_command *command
);

//Read data from a specific interface
PSVR_API int psvr_read_sync(
	enum morpheus_usb_interfaces interface,
	psvr_context *ctx,
	uint8_t *payload,
	uint32_t length
);

//Send a command to the Control interface
PSVR_API int psvr_send_command_sync(
	psvr_context *ctx,
	uint8_t id,
	uint8_t *payload,
	uint32_t length
);

//Read data from the Sensor interface
PSVR_API int psvr_read_sensor_sync(
	psvr_context *ctx,
	uint8_t *payload,
	uint32_t length
);

//Read data from the Control interface
//NOTE! This is blocking.
PSVR_API int psvr_read_control_sync(
	psvr_context *ctx,
	uint8_t *payload,
	uint32_t length
);

#ifdef __cplusplus  
}
#endif  