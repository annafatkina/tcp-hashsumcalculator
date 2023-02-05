cmake_minimum_required( VERSION 3.5 )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

FIND_PACKAGE( Boost 1.40 COMPONENTS system REQUIRED )

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} ../src)

set ( TESTS 
      hasher.cc 
      session.cc
      tcpserver.cc 
      )

add_executable( tests ${TESTS})

TARGET_LINK_LIBRARIES( tests LINK_PUBLIC ${Boost_LIBRARIES} server -lpthread GTest::gtest_main gmock)

#include(GoogleTest)
#gtest_discover_tests(tests)
