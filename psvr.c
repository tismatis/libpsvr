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

#include "PSVR_Common.h"

#include "psvr.h"
#include "morpheus.h"

struct psvr_context {
	libusb_context *usb;
	libusb_device_handle *usb_handle;
	struct libusb_config_descriptor *usb_descriptor;
	uint32_t claimed_interfaces;
};

#define INTERFACES_MASK_TO_CLAIM (\
	(1 << PSVR_INTERFACE_HID_SENSOR) |\
	(1 << PSVR_INTERFACE_HID_CONTROL)\
)

static psvr_log* psvr_logger = NULL;

void psvr_set_log(psvr_log* logger) { psvr_logger = logger; }

void psvr_printf(const char* msg, ...) {
	va_list valist;
	va_start(valist, msg);

	if (psvr_logger)
		(*psvr_logger)(msg, valist);
	else {
		vprintf(msg, valist);
		printf("\n");
	}

	va_end(valist);
}

int psvr_open(psvr_context **ctx) {
	int i;
	int err;
	libusb_device *device;
	psvr_context *_ctx = NULL;

	_ctx = (psvr_context *)malloc(sizeof(psvr_context));

	memset(_ctx, 0, sizeof(psvr_context));

	if ((err = libusb_init(&_ctx->usb)) != LIBUSB_SUCCESS) {
		psvr_printf("Initialization Failed");
		goto error;
	}

	libusb_set_debug(_ctx->usb, 3);

	_ctx->usb_handle = libusb_open_device_with_vid_pid(
		_ctx->usb,
		MORPHEUS_VID,
		MORPHEUS_PID
	);

	if (_ctx->usb_handle == NULL) {
		psvr_printf("Morpheous device not found");
		goto error;
	}

	device = libusb_get_device(_ctx->usb_handle);

	if (
		(
			err = libusb_get_config_descriptor_by_value(
				device,
				MORPHEUS_CONFIGURATION_PSVR,
				&_ctx->usb_descriptor
			)
		)
		!= LIBUSB_SUCCESS
	) {
		psvr_printf("Get config descriptor failed");
		goto error;
	}

	for (i = 0; i < _ctx->usb_descriptor->bNumInterfaces; i++) {
		int mask = 1 << i;
		if (INTERFACES_MASK_TO_CLAIM & mask) {
		#ifndef PSVRWIN
			err = libusb_kernel_driver_active(_ctx->usb_handle, i);
			if (err < 0) {
				psvr_printf("Interface #%d driver status failed - %i", i, err);
				goto error;
			}
			if (err == 1) {
				psvr_printf("Detach kernel driver on interface #%d", i);
				err = libusb_detach_kernel_driver(_ctx->usb_handle, i);
				if (err != LIBUSB_SUCCESS) {
					psvr_printf("Interface #%d detach failed", i);
					goto error;
				}
			}
		#endif
			err = libusb_claim_interface(_ctx->usb_handle, i);
			if (err != LIBUSB_SUCCESS) {
				psvr_printf("Interface #%d claim failed", i);
				goto error;
			}
			_ctx->claimed_interfaces |= mask;
		}
	}

	*ctx = _ctx;

	return 0;

error:
	psvr_close(_ctx);
	return -1;
}

int psvr_open_ex(psvr_context **ctx, int interfaces_to_claim) {
	int i;
	int err;
	libusb_device *device;
	psvr_context *_ctx = NULL;

	_ctx = (psvr_context *)malloc(sizeof(psvr_context));

	memset(_ctx, 0, sizeof(psvr_context));

	if ((err = libusb_init(&_ctx->usb)) != LIBUSB_SUCCESS) {
		psvr_printf("Initialization Failed");
		goto error;
	}

	libusb_set_debug(_ctx->usb, 3);

	_ctx->usb_handle = libusb_open_device_with_vid_pid(
		_ctx->usb,
		MORPHEUS_VID,
		MORPHEUS_PID
	);

	if (_ctx->usb_handle == NULL) {
		psvr_printf("Morpheous device not found");
		goto error;
	}

	device = libusb_get_device(_ctx->usb_handle);

	if (
		(
			err = libusb_get_config_descriptor_by_value(
				device,
				MORPHEUS_CONFIGURATION_PSVR,
				&_ctx->usb_descriptor
			)
		)
		!= LIBUSB_SUCCESS
	) {
		psvr_printf("Get config descriptor failed");
		goto error;
	}

	for (i = 0; i < _ctx->usb_descriptor->bNumInterfaces; i++) {
		int mask = 1 << i;
		if (interfaces_to_claim & mask) {
		#ifndef PSVRWIN
			err = libusb_kernel_driver_active(_ctx->usb_handle, i);
			if (err < 0) {
				psvr_printf("Interface #%d driver status failed - %i", i, err);
				goto error;
			}
			if (err == 1) {
				psvr_printf("Detach kernel driver on interface #%d", i);
				err = libusb_detach_kernel_driver(_ctx->usb_handle, i);
				if (err != LIBUSB_SUCCESS) {
					printf("Interface #%d detach failed", i);
					goto error;
				}
			}
		#endif
			err = libusb_claim_interface(_ctx->usb_handle, i);
			if (err != LIBUSB_SUCCESS) {
				psvr_printf("Interface #%d claim failed", i);
				goto error;
			}
			_ctx->claimed_interfaces |= mask;
		}
	}

	*ctx = _ctx;

	return 0;

error:
	psvr_close(_ctx);
	return -1;
}

int psvr_send_sync(enum morpheus_usb_interfaces interface, psvr_context *ctx, uint8_t id, uint8_t *payload, uint32_t length) {
	if (!(ctx->claimed_interfaces & (1 << interface))) return LIBUSB_ERROR_OTHER; //only execute if we have claimed the interface

	struct morpheus_control_command command;
	command.header.id = id;
	command.header.magic = 0xAA;
	command.header.length = length;
	memcpy(command.payload, payload, length);

	return psvr_send_raw_sync(interface, ctx, &command);
}

int psvr_send_raw_sync(enum morpheus_usb_interfaces interface, psvr_context *ctx, struct morpheus_control_command *command) {
	if (!(ctx->claimed_interfaces & (1 << interface))) return LIBUSB_ERROR_OTHER; //only execute if we have claimed the interface
	int ep;
	int xferred;
	int err;

	ep = ctx->usb_descriptor->interface[interface]
		.altsetting[0]
		.endpoint[0]
		.bEndpointAddress;
	ep &= ~ENDPOINT_IN;

	err = libusb_bulk_transfer(
		ctx->usb_handle,
		ep,
		(uint8_t *)command,
		command->header.length + MORPHEUS_COMMAND_HEADER_SIZE,
		&xferred,
		0
	);

	return err;
}

int psvr_read_sync(enum morpheus_usb_interfaces interface, psvr_context *ctx, uint8_t *payload, uint32_t length) {
	if (!(ctx->claimed_interfaces & (1 << interface))) return LIBUSB_ERROR_OTHER; //only execute if we have claimed the interface

	int ep;
	int xferred = 0;
	int err;

	ep = ctx->usb_descriptor->interface[interface]
		.altsetting[0]
		.endpoint[0]
		.bEndpointAddress;
	ep |= ENDPOINT_IN;

	//err = libusb_interrupt_transfer(
	err = libusb_bulk_transfer(
		ctx->usb_handle,
		ep,
		payload,
		length,
		&xferred,
		0
	);


	if (err != LIBUSB_SUCCESS) {
		return err;
	}

	return xferred;
}

int psvr_send_command_sync(psvr_context *ctx, uint8_t id, uint8_t *payload, uint32_t length) {
	return psvr_send_sync(PSVR_INTERFACE_HID_CONTROL, ctx, id, payload, length);
}

int psvr_read_sensor_sync(psvr_context *ctx, uint8_t *payload, uint32_t length) {
	return psvr_read_sync(PSVR_INTERFACE_HID_SENSOR, ctx, payload, length);
}

int psvr_read_control_sync(psvr_context *ctx, uint8_t *payload, uint32_t length) {
	return psvr_read_sync(PSVR_INTERFACE_HID_CONTROL, ctx, payload, length);
}

void psvr_close(psvr_context *ctx)
{
	if (!ctx) return;

	int i = 0;

	while (ctx->claimed_interfaces) {
		int mask = 1 << i;
		if (ctx->claimed_interfaces & mask) {
			libusb_release_interface(ctx->usb_handle, i);
			psvr_printf("Interface #%d released", i);
			ctx->claimed_interfaces &= ~mask;
		}
		i++;
	}
	if (ctx->usb_descriptor != NULL) {
		libusb_free_config_descriptor(ctx->usb_descriptor);
		psvr_printf("Descriptor is freed");
	}
	if (ctx->usb_handle != NULL) {
		libusb_close(ctx->usb_handle);
		psvr_printf("Device is freed");
	}
	if (ctx->usb != NULL) {
		libusb_exit(ctx->usb);
		psvr_printf("LibUsb is freed");
	}
	free(ctx);
}