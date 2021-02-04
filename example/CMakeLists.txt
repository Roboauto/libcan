CMAKE_MINIMUM_REQUIRED(VERSION 3.19)
PROJECT(libcanExample CXX)

IF(NOT TARGET libcan)
	MESSAGE(FATAL_ERROR "libcan target not found. Cannot compile libcanExample")
ENDIF()

ADD_EXECUTABLE(libcanExample test.cpp)
TARGET_LINK_LIBRARIES(libcanExample libcan)