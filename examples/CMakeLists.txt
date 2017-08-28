include_directories("../src")
include_directories("../dependency")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11") # enable C++11
 
add_executable(example1 example1.cpp ../src/SimpleSock.cpp ../src/SimpleSockTCP.cpp ../src/SimpleSockUDP.cpp ../dependency/SafeFunctions/SafeFunctions.cpp)
add_executable(example2 example2.cpp ../src/SimpleSock.cpp ../src/SimpleSockTCP.cpp ../src/SimpleSockUDP.cpp ../dependency/SafeFunctions/SafeFunctions.cpp)

if(WIN32)
  target_link_libraries(example1 ws2_32)
  target_link_libraries(example2 ws2_32)
endif()
