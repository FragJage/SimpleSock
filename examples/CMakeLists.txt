include_directories("../src")
include_directories("../dependency")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11") # enable C++11
 
add_executable(example1 example1.cpp ../src/SimpleSock.cpp ../dependency/SafeFunctions/SafeFunctions.cpp)
add_executable(example2 example2.cpp ../src/SimpleSock.cpp ../dependency/SafeFunctions/SafeFunctions.cpp)
