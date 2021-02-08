CMAKE_MINIMUM_REQUIRED(VERSION 3.19)
PROJECT(libcan CXX)

SET(INCLUDE_FILES
	include/libcan/CANFrameParser.h
	include/libcan/SocketCAN.h
	include/libcan/CANFrame.h
	include/libcan/CANAdapter.h
)

SET(SOURCE_FILES
	src/CANFrameParser.cpp
	src/SocketCAN.cpp
	src/CANAdapter.cpp
)

ADD_LIBRARY(libcan_static STATIC ${INCLUDE_FILES} ${SOURCE_FILES})
TARGET_INCLUDE_DIRECTORIES(libcan_static PUBLIC include/)
TARGET_LINK_LIBRARIES(libcan_static INTERFACE pthread)

ADD_LIBRARY(libcan ALIAS libcan_static)

IF(BUILD_SAMPLES)
	ADD_SUBDIRECTORY(example)
ENDIF()
