#pragma once

#include "PSVR_Common.h"

#pragma pack(1)
union psvr_sensor_frame_union {
	struct psvr_sensor_frame_struct {
		struct {
			uint8_t reserved : 1;
			uint8_t plus : 1;
			uint8_t minus : 1;
			uint8_t mute : 1;
		} button; //1
		uint8_t reserved0; //1
		uint8_t volume; //1
		uint8_t reserved1[5]; //5
		union {
			uint8_t as_byte; //1
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
		uint8_t reserved2[11]; //11
		struct {
			//uint32_t timestamp; //4
			struct {
				int16_t yaw; //2
				int16_t pitch; //2
				int16_t roll; //2
			} gyro;
			struct {
				int16_t x; //2
				int16_t y; //2
				int16_t z; //2
			} accel;
			uint8_t reserved[4]; //4
		} data[2]; //32
		uint8_t reserved3[12]; //12

		/*uint8_t calStatus; //1
		uint8_t ready; //1
		uint8_t reserved3; //1
		uint8_t voltageVal; //1
		uint8_t voltageRef; //1
		int16_t irSensor; //2
		uint8_t reserved4[5]; //5
		uint8_t frame; //1
		uint8_t reserved5; //1*/

	#ifdef __cplusplus  
	} s;
	#else
	};
	#endif
	uint8_t raw[64];
};
#pragma pack()

typedef union psvr_sensor_frame_union psvr_sensor_frame;