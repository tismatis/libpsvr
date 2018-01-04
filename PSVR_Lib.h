#pragma once

#if (defined(_WIN32) || defined(_WIN64))
	#define PSVRWIN

	#ifdef PSVR_DLL
		#ifdef PSVR_EXPORT_DLL
			#define PSVR_LIB __declspec(dllexport)
		#else 
			#define PSVR_LIB __declspec(dllimport)
		#endif
	#else
		#define PSVR_LIB
	#endif
#else
	#define PSVRNIX

	#ifdef PSVR_DLL
		#ifdef PSVR_EXPORT_DLL
			#define PSVR_LIB extern
		#else 
			#define PSVR_LIB
		#endif
	#else
		#define PSVR_LIB
	#endif
#endif