cmake_minimum_required (VERSION 3.1)

project (cdn CXX C)

include_directories (
  ./lib
  ./deploy.h
)

set (CMAKE_CXX_STANDARD 11)

if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif ()

add_library (engine STATIC
  ./deploy.cpp
  ./io.cpp  
)

add_executable(cdn ./cdn.cpp)
target_link_libraries(cdn engine)