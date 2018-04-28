cmake_minimum_required(VERSION 3.2)

# initialise variables
set(ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/..)
set(PROJECT_SRC_DIR ${ROOT_DIR}/src)
set(PROJECT_INCL_DIR ${ROOT_DIR}/include)
set(TARGET_BUILD_DIR ${ROOT_DIR}/build)
set(CMAKE_BUILD_TYPE Release)

# Set libpsvr's project source files
add_library(libpsvr SHARED ${PROJECT_INCL_DIR}/Control_Frame.h
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

# Set libpsvr's required 3rdParty include directories
target_include_directories(libpsvr PUBLIC ${LIBUSB_INCL_DIRS})

# Set libpsvr's required 3rd party libusb library
target_link_libraries(libpsvr ${LIBUSB_REQ_LIB})

# Set preprocessor directive so it builds a lib api instead of a dll
add_definitions(-DPSVR_DLL)
add_definitions(-DPSVR_EXPORT_DLL)

set_target_properties(libpsvr PROPERTIES VERSION ${PROJECT_VERSION})
							   
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

# Define libpsvr install target
install(TARGETS libpsvr 
        RUNTIME DESTINATION ${TARGET_BUILD_DIR}/${ARCH_LABEL}/libpsvr
        LIBRARY DESTINATION ${TARGET_BUILD_DIR}/${ARCH_LABEL}/libpsvr
		ARCHIVE DESTINATION ${TARGET_BUILD_DIR}/${ARCH_LABEL}/libpsvr
)