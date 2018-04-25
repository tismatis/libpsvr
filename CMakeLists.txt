cmake_minimum_required(VERSION 3.2)
project(libpsvr VERSION 0.1.0)
include(${CMAKE_CURRENT_SOURCE_DIR}/CMakeUtils.txt)
include(ExternalProject)

set(ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})
set(CMAKE_CXX_STANDARD_17)
set(CMAKE_CXX_STANDARD_REQUIRED_ON)
IF (NOT MSVC)
    set(CMAKE_CXX_FLAGS "-std=c++11")
    IF (NOT CMAKE_COMPILER_IS_GNUCXX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    ENDIF()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-switch")
ENDIF()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Configure and build external libusb projects based on os architecture
if (${CMAKE_C_SIZEOF_DATA_PTR} EQUAL 8)
        if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
                set(ARCH_LABEL "Win64")
				build_external_project(libusb ${ROOT_DIR}/deps https://github.com/libusb/libusb/releases/download/v1.0.22/libusb-1.0.22.7z)
				set(LIBUSB_REQ_LIB ${ROOT_DIR}/deps/libusb/src/libusb/MS64/dll/libusb-1.0.lib)
				set(LIBUSB_INCL_DIRS ${ROOT_DIR}/deps/libusb/src/libusb/include/libusb-1.0)
				
        elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
                set(ARCH_LABEL "OSX64")
				# TODO
				build_external_project(libusb ${ROOT_DIR}/deps https://github.com/libusb/libusb/releases/download/v1.0.22/libusb-1.0.22.tar.bz2)
        else()
				set(ARCH_LABEL "Linux64")
				# TODO
				build_external_project(libusb ${ROOT_DIR}/deps https://github.com/libusb/libusb/releases/download/v1.0.22/libusb-1.0.22.tar.bz2)

        endif()
endif()

add_subdirectory(src)