cmake_minimum_required(VERSION 3.8)

project(tests)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

include_directories(
    ../include
    ${GTEST_INCLUDE_DIRS}
)

add_executable(
  TestSerialization ./main.cpp
  ../bib/functions.cpp)

target_link_libraries(
  TestSerialization ${GTEST_LIBRARIES} Threads::Threads)

enable_testing()
add_test(FT "./TestSerialization")