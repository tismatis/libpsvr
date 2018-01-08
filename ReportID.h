#pragma once

#include "PSVR_Common.h"

//1 payload is equal to a length of 4 bytes;
//1 payload is equal to 1 uint32_t or 4 uint8_t;

enum psvr_report_id {
	//length: 8 - 2 payloads
	/*
	payload0:
	0xFFFFFFFF - tracking lights on
	0x??FFFFFF - loading screen
	*/

	//tracking test? - payload0: (uint8_t[]){ value, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF }, payload1: 0
	//with tracking
	//I have no idea what this command does...
	//formally VRMode.
	eRID_Unknown0 = 0x11,

	//length: 4 - 1 payload
	//off - (uint8_t[]){0x01, 0x00, 0x00, 0x00}
	eRID_ProcessorPower = 0x13,

	//length: 16 - 4 payloads
	/*
	uint8_t cmd[] = {
		0, //(uint8_t)(mMask & 0xFF),
		0, //(uint8_t)((mMask >> 8) & 0xFF),
		0, //A,
		0, //B,

		0, //C
		0, //D
		0, //E
		0, //F

		0, //G
		0, //H
		0, //I
		0,

		0,
		0,
		0,
		0
	};
	*/
	eRID_LED = 0x15,

	//length: 4 - 1 payload
	//power - payload: (uint32_t)bool
	eRID_HeadsetPower = 0x17,

	//length: 4 - 1 payload
	//(uint8_t[]){ 0x00, 0x00, 0x00, 0x00 }
	//accepts anything?
	//dont know if this does anything
	//formally known as recenter.
	eRID_Unknown1 = 0x1b,

	//length: 16 - 4 payloads
	/*
	uint8_t cmd[] = {
		0, //Mask, //lock? 0x80 or 0xc0
		0, //ScreenSize, //26 to 80
		0, //ScreenDistance, //20 to 50
		0, //IPD, //1 to 255

		0,
		0,
		0,
		0,

		0,
		0,
		0, //Brightness, //1 to 32
		0, //MicVolume, //0 to 255?

		0, //hdmi //0 to 4
		0,
		0, //(uint8_t)(UnknownVRSetting ? 0 : 1),
		0
	};
	*/
	eRID_SetSettings = 0x21,
	
	//length: 4 - 1 payload
	//Cinematic mode - { 0x00, 0x00, 0x00, 0x00 }
	//VR Mode - { 0x01 0x00 0x00 0x00 }
	eRID_SetMode = 0x23,

	eRID_Unknown2 = 0x25,
	
	//length: 8 - 2 payloads
	//(uint8_t[]){ 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	eRID_DeviceInfo = 0x81
};

enum LedMask {
	eLM_None = 0,
	eLM_LedA = (1 << 0),
	eLM_LedB = (1 << 1),
	eLM_LedC = (1 << 2),
	eLM_LedD = (1 << 3),
	eLM_LedE = (1 << 4),
	eLM_LedF = (1 << 5),
	eLM_LedG = (1 << 6),
	eLM_LedH = (1 << 7),
	eLM_LedI = (1 << 8),
	eLM_All = eLM_LedA | eLM_LedB | eLM_LedC | eLM_LedD | eLM_LedE | eLM_LedF | eLM_LedG | eLM_LedH | eLM_LedI
};