#LibPSVR
C library for Morpheus (Playstation VR). Tested with Ubuntu 15.10 + libusb-1.0.20.

## Build
```
$ ./build.sh
```
## Example
```
$ gcc example.c -I/path_to_psvr_h -L/path_to_libpsvr_so -lpsvr
```
## OS X (El Capitan)
For quick start, use brew before build.  

```
$ brew install pkg-config libusb
```

You will need to unload kernel modules manually.  

```
$ sudo kextunload -b com.apple.driver.usb.IOUSBHostHIDDevice
```

## Windows
Requires the latest libusb to be installed.  
Paths are set to be one folder outside of master folder with the folder name of "libusb-master".  
This can be changed if you wish.  
To compile, use the solution in the LibPSVR folder.  
Solution file is for Visual Studio 2017.  
  
To use PSVR with this, you must install WinUSB driver for the PS VR Sensors and PS VR Control, Interfaces 4 and 5 respectively.  
You can use zadiag, which is referenced with libusb, to install the drivers.