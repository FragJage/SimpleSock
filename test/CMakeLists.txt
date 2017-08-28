include_directories("../src")
include_directories("../dependency")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall") # various warning flags
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wshadow -W -Wunused-variable -Wunused-parameter -Wunused-function -Wunused -Wno-system-headers -Wno-deprecated -Woverloaded-virtual") # various warning flags
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0") # debug, no optimisation
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage") # enabling coverage
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11") # enable C++11
 
add_executable(unittest main.cpp ../src/SimpleSock.cpp ../dependency/SafeFunctions/SafeFunctions.cpp)

if(WIN32)
  target_link_libraries(unittest ws2_32)
endif()
