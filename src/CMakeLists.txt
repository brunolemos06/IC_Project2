cmake_minimum_required(VERSION 3.16)
project(opencv_example)

find_package(OpenCV REQUIRED)

SET(CMAKE_BUILD_TYPE "Release")
#SET(CMAKE_BUILD_TYPE "Debug")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++17 -pthread")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3")
SET(CMAKE_CXX_FLAGS_DEBUG "-g3 -fsanitize=address")

SET(BASE_DIR ${CMAKE_SOURCE_DIR})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BASE_DIR}/../bin)


#EXERCISE 1
add_executable(ex2 ex2.cpp)
target_link_libraries(ex2 ${OpenCV_LIBS})

#EXERCISE 1
add_executable(copy copy.cpp)
target_link_libraries(copy ${OpenCV_LIBS})

#EXERCISE 2a
add_executable(ex2_a ex2_a.cpp)
target_link_libraries(ex2_a ${OpenCV_LIBS})

#EXERCISE 2ba
add_executable(ex2_ba ex2_ba.cpp)
target_link_libraries(ex2_ba ${OpenCV_LIBS})

#EXERCISE 2bb
add_executable(ex2_bb ex2_bb.cpp)
target_link_libraries(ex2_bb ${OpenCV_LIBS})

#EXERCISE 2c
add_executable(ex2_c ex2_c.cpp)
target_link_libraries(ex2_c ${OpenCV_LIBS})

#EXERCISE 2d
add_executable(ex2_d ex2_d.cpp)
target_link_libraries(ex2_d ${OpenCV_LIBS})


