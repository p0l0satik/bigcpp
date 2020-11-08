cmake_minimum_required(VERSION 3.8)
project(IO)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O5 -fopenmp")

add_subdirectory(tinyxml2)
set(LIBS tinyxml2)



add_executable(IO main.cpp SimulatedAnnealing.cpp solution.cpp temperature.cpp)

target_link_libraries(IO ${LIBS})