cmake_minimum_required( VERSION 3.5 )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

FIND_PACKAGE( Boost 1.40 COMPONENTS system REQUIRED )

INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

set( INCLUDES
     hasher.h 
     isession.h 
     session.h 
     session.cpp 
     tcpserver.h 
     tcpserver.cpp)

add_library(server ${INCLUDES})
add_executable( tcp_server main.cpp )

TARGET_LINK_LIBRARIES( tcp_server LINK_PUBLIC ${Boost_LIBRARIES} server -lpthread )
