SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

FIND_PACKAGE(Boost REQUIRED COMPONENTS system thread)
FIND_PACKAGE(MsgPack REQUIRED)

include_directories(${Boost_INCLUDE_DIRS} ${MsgPack_INCLUDE_DIR})

set(AMAGAWA_SOURCES
	server.cc
	session.cc
	dispatcher.cc)

add_library(amagawa ${AMAGAWA_SOURCES})
