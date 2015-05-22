SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

FIND_PACKAGE(Boost REQUIRED COMPONENTS system thread unit_test_framework)
FIND_PACKAGE(MsgPack REQUIRED)
FIND_PACKAGE(Threads REQUIRED)

include_directories(
	${CMAKE_SOURCE_DIR}
	${Boost_INCLUDE_DIRS}
	${MsgPack_INCLUDE_DIR})

link_directories(${CMAKE_BINARY_DIR}/amagawa)

add_executable(
	test
	dispatcher.cc
	server.cc
	main.cc)

target_link_libraries(
	test amagawa ${Boost_LIBRARIES} ${MsgPack_LIBRARY})
