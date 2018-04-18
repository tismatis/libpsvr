
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if (defined(_WIN32) || defined(_WIN64))
	#include <stdint.h>
	#include <libusb.h>
	#define PSVRWIN
#else
	#include "libusb.h"
#endif