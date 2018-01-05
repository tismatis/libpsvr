#pragma once

#include "PSVR_Common.h"

//1 payload is equal to a length of 4 bytes;
//1 payload is equal to 1 uint32_t or 4 uint8_t;

enum psvr_report_id {
	//length: 8 - 2 payloads
	//enable - payload0: 0xFFFFFF00, payload1: bool
	//enable2??? - payload0: 0x80FFFF00, payload1: bool
	//enable3??? - payload0: 0x00FFFFFF, payload1: bool
	//tracking test - payload0: (uint8_t[]){ value, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF }, payload1: 0
	//with tracking
	eRID_VRTracking = 0x11,

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
	eRID_Recenter = 0x1b,

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
	eRID_CinematicMode = 0x21,
	
	//length: 4 - 1 payload
	//enable - payload: (uint32_t)bool
	//without tracking
	eRID_VRMode = 0x23,

	eRID_Unknown = 0x25,
	
	//length: 8 - 2 payloads
	/*
	uint8_t cmd[] = { 0x80, 0, 0, 0, 0, 0, 0, 0 };
	*/
	eRID_DeviceInfo = 0x81
};