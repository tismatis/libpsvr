CC=gcc
libusb=$(shell pkg-config --libs --cflags libusb-1.0)
currDir=$(shell pwd)

all: shared

shared:
	$(CC) -shared -DPSVR_DLL -DPSVR_EXPORT_DLL -I"./include" -o libpsvr.so -fPIC src/psvr.c $(libusb)

lib:
	$(CC) -c -I"./include" src/psvr.c $(libusb)
	ar -cvq libpsvr.a psvr.o

example:
	$(CC) LibPSVR/LibPSVR_Example/example.c -DPSVR_DLL -I"./include" -L"$(currDir)" -lpsvr $(libusb) -o example

example-lib:
	$(CC) LibPSVR/LibPSVR_Example/example.c -I"./include" -L"$(currDir)" -lpsvr $(libusb) -o example

clean: clean-shared

clean-shared:
	rm libpsvr.so

clean-lib:
	rm psvr.o
	rm libpsvr.a

clean-example:
	rm example
