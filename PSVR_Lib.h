#pragma once

#if (defined(_WIN32) || defined(_WIN64))
	#define PSVRWIN

	/*#ifdef PSVR_EXPORT_LIB
		#define PSVR_LIB __declspec( dllexport )
	#else 
		#define PSVR_LIB __declspec(dllimport)
	#endif*/
	#define PSVR_LIB
#else
	#define PSVRNIX
	//#define PSVR_LIB extern
	#define PSVR_LIB
#endif