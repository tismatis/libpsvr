cmake_minimum_required(VERSION 3.0)

# initialise variables
set(PROJECT_DIR ${CMAKE_CURRENT_LIST_DIR})
set(PROJECT_SRC_DIR ${PROJECT_DIR}/src)
set(PROJECT_INCL_DIR ${PROJECT_DIR}/include)
set(TARGET_BUILD_DIR ${PROJECT_DIR}/build)
set(THIRD_PARTY ${PROJECT_DIR}/3rdParty)

# Set OpenPSVR's project source files
add_library(OpenPSVR SHARED ${PROJECT_INCL_DIR}/Control_Frame.h
                            ${PROJECT_INCL_DIR}/Device_Info.h
                            ${PROJECT_INCL_DIR}/Device_Status.h
                            ${PROJECT_INCL_DIR}/morpheus.h
                            ${PROJECT_INCL_DIR}/psvr.h
                            ${PROJECT_INCL_DIR}/PSVR_Common.h
                            ${PROJECT_INCL_DIR}/PSVR_Lib.h
                            ${PROJECT_INCL_DIR}/ReportID.h
                            ${PROJECT_INCL_DIR}/Sensor_Frame.h
                            ${PROJECT_INCL_DIR}/Unsolicited_Report.h
                            ${PROJECT_SRC_DIR}/psvr.c
)

# Set OpenPSVR's required 3rdParty include directories
set(LIBUSB_INCL_DIRS ${THIRD_PARTY}/libusb/libusb)
target_include_directories(OpenPSVR PUBLIC ${LIBUSB_INCL_DIRS})

# TODO This needs to be sorted out using Find
# Set OpenPSVR's required 3rdParty libraries
set(OPENVR_REQ_LIB ${THIRD_PARTY}/openvr/lib/win64/openvr_api.lib)
set(LIBPSVR_REQ_LIB ${THIRD_PARTY}/libpsvr/LibPSVR/x64/Release/lib/LibPSVR.lib)
set(LIBUSB_REQ_LIB ${THIRD_PARTY}/libusb/x64/Release/dll/libusb-1.0.lib)
target_link_libraries(OpenPSVR ${OPENVR_REQ_LIB}
                               ${LIBPSVR_REQ_LIB}
							   ${LIBUSB_REQ_LIB}
							   
# Shared architecture label used for install folder locations
if (${CMAKE_C_SIZEOF_DATA_PTR} EQUAL 8)
        if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
                set(ARCH_LABEL "Win64")
        elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
                set(ARCH_LABEL "OSX64")
        else()
                set(ARCH_LABEL "Linux64")
        endif()
else()
        if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
                set(ARCH_LABEL "Win32")
        elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
                set(ARCH_LABEL "OSX32")
        else()
                set(ARCH_LABEL "Linux32")
        endif()
endif()

# Define OpenPSVR install target
install(TARGETS OpenPSVR
        RUNTIME DESTINATION ${TARGET_BUILD_DIR}/${ARCH_LABEL}/libpsvr
        LIBRARY DESTINATION ${TARGET_BUILD_DIR}/${ARCH_LABEL}/libpsvr/lib
        ARCHIVE DESTINATION ${TARGET_BUILD_DIR}/${ARCH_LABEL}/libpsvr/lib
)