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

#include "ReportID.h"
#include "Sensor_Frame.h"
#include "Device_Info.h"
#include "Device_Status.h"
#include "Unsolicited_Report.h"

typedef struct psvr_context psvr_context;

int psvr_open(psvr_context **ctx);

int psvr_open_ex(
	psvr_context **ctx,
	int interfaces_to_claim
);

void psvr_close(psvr_context *ctx);

int psvr_send_sync(
	enum morpheus_usb_interfaces interface,
	psvr_context *ctx,
	enum psvr_report_id id,
	uint8_t *payload,
	uint32_t length
);
int psvr_read_sync(
	enum morpheus_usb_interfaces interface,
	psvr_context *ctx,
	uint8_t *payload,
	uint32_t length
);

int psvr_send_command_sync(
	psvr_context *ctx,
	enum psvr_report_id id,
	uint8_t *payload,
	uint32_t length
);

int psvr_read_sensor_sync(
	psvr_context *ctx,
	uint8_t *payload,
	uint32_t length
);
int psvr_read_control_sync(
	psvr_context *ctx,
	uint8_t *payload,
	uint32_t length
);