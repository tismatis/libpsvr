#pragma once

#include "PSVR_Common.h"

//1 payload is equal to a length of 4 bytes;
//1 payload is equal to 1 uint32_t or 4 uint8_t;

enum psvr_report_id {
	//length: 4 - 1 payload
	//power - payload: (uint32_t)bool
	eRID_HeadsetPower = 0x17,

	//length: 8 - 2 payloads
	//enable - payload0: 0xFFFFFF00, payload1: bool
	//tracking test - payload0: (uint8_t[]){ value, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF }, payload1: 0
	eRID_VRTracking = 0x11,
	
	//length: 4 - 1 payload
	//enable - payload: (uint32_t)bool
	eRID_VRMode = 0x23,
	
	//length: 4 - 1 payload
	//power - payload: (uint32_t)bool
	eRID_ProcessorPower = 0x13,
	
	//length: 16 - 4 payloads
	/*
	uint8_t cmd[] = {
		0, //Mask,
		0, //ScreenSize,
		0, //ScreenDistance,
		0, //IPD,

		0,
		0,
		0,
		0,

		0,
		0,
		0, //Brightness,
		0, //MicVolume,

		0,
		0,
		0, //(uint8_t)(UnknownVRSetting ? 0 : 1),
		0
	};
	*/
	eRID_CinematicMode = 0x21,
	
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
	
	//length: 8 - 2 payloads
	/*
	uint8_t cmd[] = { 0x80, 0, 0, 0, 0, 0, 0, 0 };
	*/
	eRID_DeviceInfo = 0x81
};