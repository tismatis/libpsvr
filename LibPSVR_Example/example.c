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

#include <stdio.h>
#include <stdint.h>

#if !(defined(_WIN32) || defined(_WIN64))
	#include <unistd.h>
#else
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#endif

#include "psvr.h"
#include "morpheus.h"

static psvr_context *ctx;

static int command_set_power(uint32_t on) {
	printf("Set Power %d\n", on);
	return psvr_send_command_sync(ctx, eRID_HeadsetPower, (uint8_t*)&on, 4);
}

static int command_enable_vr_mode() {
	uint32_t payload[2];
	payload[0] = 0xFFFFFF00;
	payload[1] = 0x00000000;
	printf("Enable VR Mode\n");
	return psvr_send_command_sync(ctx, eRID_VRMode, (uint8_t *)&payload, 8);
}

static int command_get_device_info() {
	printf("Getting Device Info.\n");

	uint8_t cmd[] = { 0x80, 0, 0, 0, 0, 0, 0, 0 };
	int r = psvr_send_command_sync(ctx, eRID_DeviceInfo, cmd, 8);

	return r;
}

static void print_control_data(psvr_control_frame *frame) {
	switch (frame->header.r_id) {
	case eRT_Info:
	{
		printf("Info:\n");
		printf("- major: %i\n", frame->dinfo.version.major);
		printf("- minor: %i\n", frame->dinfo.version.minor);
		printf("- serial: %s\n", frame->dinfo.serialNumber);
		printf("- serial raw: ");
		for (int i = 0; i < 16; i++) printf("%i ", frame->dinfo.serialNumber[i]);
		printf("\n");

		//debugging based on raw data.
		/*unsigned int major = frame->data[7] + 0x30;
		unsigned int minor = frame->data[8] + 0x30;

		uint8_t serial[16];
		for (int i = 0, i2 = 12; i < 16; i++, i2++) serial[i] = frame->data[i2];

		printf("Info2:\n");
		printf("- major: %i\n", major);
		printf("- minor: %i\n", minor);
		printf("- serial: %s\n", serial);
		printf("- serial raw: ");
		for (int i = 0; i < 16; i++) printf("%i ", serial[i]);
		printf("\n");*/
	}
		break;
	case eRT_Status:
	{
		printf("Status:\n");
		printf("- Mask: %i\n", frame->dstatus.mask);
		printf("-- HeadsetOn: %s\n", frame->dstatus.headsetOn ? "true" : "false");
		printf("-- Worn: %s\n", frame->dstatus.worn ? "true" : "false");
		printf("-- Cinematic: %s\n", frame->dstatus.cinematic ? "true" : "false");
		printf("-- reserved0: %s\n", frame->dstatus.maskreserved0 ? "true" : "false");
		printf("-- Headphones: %s\n", frame->dstatus.headphones ? "true" : "false");
		printf("-- Mute: %s\n", frame->dstatus.mute ? "true" : "false");
		printf("-- CEC: %s\n", frame->dstatus.cec ? "true" : "false");
		printf("-- reserved1: %s\n", frame->dstatus.maskreserved1 ? "true" : "false");
		printf("- Volume: %i\n", frame->dstatus.volume);
		printf("- reserved0: %i\n", frame->dstatus.reserved0);
		printf("- bridgeOutputID: %i\n", frame->dstatus.bridgeOutputID);
		printf("- raw: ");
		for (int i = 0; i < 8; i++) printf("%i ", frame->dstatus.raw[i]);
		printf("\n");
	}
		break;
	case eRT_Unsolicited:
	{
		printf("Report:\n");
		printf("- id: %i\n", frame->ureport.id);
		printf("- code: %i\n", frame->ureport.code);
		printf("- message: %s\n", frame->ureport.message);
		printf("- message raw: ");
		for (int i = 0; i < 58; i++) printf("%i ", frame->ureport.message[i]);
		printf("\n");
	}
		break;
	}
}

static void print_sensor_data(psvr_sensor_frame *frame) {
	int i;
	printf("Button: Plus=%d, Minus=%d, Mute=%d\n", frame->button.plus, frame->button.minus, frame->button.mute);
	printf("Volume: %d\n", frame->volume);
	printf("Status: %x\n", frame->status.as_byte);
	for (i = 0; i < 2; i++) {
		printf("Frame#%d: gyro=%8d, %8d, %8d accel=%8d, %8d, %8d\n", i,
			   frame->data[i].gyro.yaw, frame->data[i].gyro.pitch, frame->data[i].gyro.roll,
			   frame->data[i].accel.x, frame->data[i].accel.y, frame->data[i].accel.z);
	}
}

#if defined(_WIN32) || defined(_WIN64)
void usleep(DWORD waitTime) {
	LARGE_INTEGER perfCnt, start, now;

	QueryPerformanceFrequency(&perfCnt);
	QueryPerformanceCounter(&start);

	do {
		QueryPerformanceCounter((LARGE_INTEGER*)&now);
	} while ((now.QuadPart - start.QuadPart) / (float)(perfCnt.QuadPart) * 1000 * 1000 < waitTime);
}

void pause() {
	//_getch();
	system("pause");
}
#endif

int main(void) {
	int r;

	if ((r = psvr_open(&ctx)) < 0) {
		printf("Cannot open PSVR\n");
		pause();
		return -1;
	}

	command_set_power(1);

	//\x80\x00\x00\x00\x00\x00\x00\x00
	//uint8_t cmd[] = { 0x80, 0, 0, 0, 0, 0, 0, 0 };

	//0-4 calibration data
	//uint8_t cmd[] = { 0x86, 0, 0, 0, 0, 0, 0, 0 };
	//r = psvr_send_command_sync(ctx, eRID_DeviceInfo, cmd, 8);

	/*psvr_control_frame psvrFrame;
	for (int i = 0; i < 10; i++) {
		r = psvr_read_control_sync(ctx, (uint8_t *)&psvrFrame, sizeof(psvr_control_frame)); //BLOCKING!!!
		print_control_data(&psvrFrame);
		usleep(10 * 1000);
	}*/

	command_enable_vr_mode();

	psvr_sensor_frame frame;
	for (int i = 0; i < 1000; i++) {
		r = psvr_read_sensor_sync(ctx, (uint8_t *)&frame, sizeof(psvr_sensor_frame));
		print_sensor_data(&frame);
		usleep(10 * 1000);
	}

	command_set_power(0);

	psvr_close(ctx);

	pause();

	return 0;
}