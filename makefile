CC=gcc
libusb=$(shell pkg-config --libs --cflags libusb-1.0)

all:
	$(CC) -shared -DPSVR_DLL -DPSVR_EXPORT_DLL -o libpsvr.so -fPIC psvr.c $(libusb)

lib:
	$(CC) -c psvr.c $(libusb)
	ar -cvq libpsvr.a psvr.o

clean:
	rm libpsvr.so

clean-lib:
	rm psvr.o
	rm libpsvr.a
