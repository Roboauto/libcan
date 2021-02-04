CMAKE_MINIMUM_REQUIRED(VERSION 3.19)
PROJECT(libcan CXX)

SET(INCLUDE_FILES
	include/CANFrameParser.h
	include/SocketCAN.h
	include/SLCAN.h
	include/CANFrame.h
	include/CANAdapter.h
	include/Vector/CANalyzer/LoggingFile.h
	include/Vector/XLDriverLibrary/VectorCANAdapter.h
)

SET(SOURCE_FILES
	src/CANFrameParser.cpp
	src/SocketCAN.cpp
	src/SLCAN.cpp
	src/CANAdapter.cpp
	src/Vector/XLDriverLibrary/VectorCANAdapter.cpp
	src/Vector/CANalyzer/LoggingFile.cpp
)

ADD_LIBRARY(libcan_static STATIC ${INCLUDE_FILES} ${SOURCE_FILES})
TARGET_INCLUDE_DIRECTORIES(libcan_static PUBLIC include/)
TARGET_LINK_LIBRARIES(libcan_static INTERFACE pthread)

ADD_LIBRARY(libcan ALIAS libcan_static)

IF(ENABLE_SAMPLES)
	ADD_SUBDIRECTORY(example)
ENDIF()
