#pragma once

#include "PSVR_Common.h"

#pragma pack(1)
struct psvr_sensor_frame {
	struct {
		uint8_t reserved : 1;
		uint8_t plus : 1;
		uint8_t minus : 1;
		uint8_t mute : 1;
	} button;
	uint8_t reserved0;
	uint8_t volume;
	uint8_t reserved1[5];
	union {
		uint8_t as_byte;
		struct {
			uint8_t worn : 1;
			uint8_t display_active : 1;
			uint8_t hdmi_disconnected : 1;	// XXX
			uint8_t microphone_muted : 1;
			uint8_t headphone_connected : 1;
			uint8_t reserved : 2;
			uint8_t tick : 1;
		};
	} status;
	uint8_t reserved2[11];
	struct {
		struct {
			int16_t yaw;
			int16_t pitch;
			int16_t roll;
		} gyro;
		struct {
			int16_t x;
			int16_t y;
			int16_t z;
		} accel;
		uint8_t reserved[4];
	} data[2];
	uint8_t reserved3[12];
};
#pragma pack()