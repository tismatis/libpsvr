#pragma once

#if (defined(_WIN32) || defined(_WIN64))
	#define PSVRWIN

	#ifdef PSVR_DLL
		#ifdef PSVR_EXPORT_DLL
			#define PSVR_API __declspec(dllexport)
		#else 
			#define PSVR_API __declspec(dllimport)
		#endif
	#else
		#define PSVR_API
	#endif
#else
	#define PSVRNIX

	#ifdef PSVR_DLL
		//#ifdef PSVR_EXPORT_DLL
			#define PSVR_API __attribute__ ((visibility ("default")))
		//#else 
		//	#define PSVR_API __attribute__ ((visibility ("default")))
		//#endif
	#else
		#define PSVR_API
	#endif
#endif