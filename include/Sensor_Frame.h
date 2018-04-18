#pragma once

#include "PSVR_Common.h"

#pragma pack(1)
union psvr_sensor_frame_union {
	struct psvr_sensor_frame_struct {
		union {
			uint8_t as_byte;
			struct {
				uint8_t reserved : 1;
				uint8_t plus : 1;
				uint8_t minus : 1;
				uint8_t mute : 1;
			}; //1 [0]
		} button;

		uint8_t reserved0; //1 [1]
		uint8_t volume; //1 [2]
		uint8_t reserved1[5]; //5 [3..7]
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
		} status; //[8]
		
		uint8_t reserved2[7]; //[9..15]

		/*uint32_t timestamp1; //[16] //4
		int16_t gyroYaw1; //[20] //2
		int16_t gyroPitch1; //[22] //2
		int16_t gyroRoll1; //[24] //2
		int16_t accelX1; //[26] //2
		int16_t accelY1; //[28] //2
		int16_t accelZ1; //[30] //2

		uint32_t timestamp2; //[32] //4
		int16_t gyroYaw2; //[36] //2
		int16_t gyroPitch2; //[38] //2
		int16_t gyroRoll2; //[40] //2
		int16_t accelX2; //[42] //2
		int16_t accelY2; //[44] //2
		int16_t accelZ2; //[46] //2*/

		struct {
			uint32_t timestamp; //[16/32]
			struct {
				int16_t yaw; //[20/36]
				int16_t pitch; //[22/38]
				int16_t roll; //[24/40]
			} gyro; //[20..24/36..40]
			struct {
				int16_t x; //[26/42]
				int16_t y; //[28/44]
				int16_t z; //[30/46]
			}accel;//[26..30/42..46]
		}data[2]; //[16..46]

		uint8_t calStatus; //[48] //1
		uint8_t ready; //[49] //1

		//uint8_t A; //[50] //1
		//uint8_t B; //[51] //1
		//uint8_t C; //[52] //1
		uint8_t reserved3[3]; //[50..52]

		uint8_t voltageVal; //[53] //1
		uint8_t voltageRef; //[54] //1

		int16_t irSensor; //[55] //2

		//uint8_t D; //[58] //1
		//uint8_t E; //[59] //1
		//uint8_t F; //[60] //1
		//uint8_t G; //[61] //1
		//uint8_t H; //[62] //1
		uint8_t reserved4[5]; //[58..62]

		uint8_t packetSeq; //[63]
	#ifdef __cplusplus  
	} s;
	#else
	};
	#endif
	uint8_t raw[64];
};
#pragma pack()

typedef union psvr_sensor_frame_union psvr_sensor_frame;