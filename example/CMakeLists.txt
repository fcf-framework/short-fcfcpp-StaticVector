cmake_minimum_required(VERSION 3.0)

set(PROJECT_NAME fcfBasisTest)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/libraries)

add_executable("static-vector-speed" static-vector-speed/static-vector-speed.cpp
                                     )

add_executable("static-vector-tests" 
                                     static-vector-tests/static-vector-tests.cpp
                                     static-vector-tests/StaticVector.cpp
                                     static-vector-tests/StaticVectorCopy.cpp
                                     static-vector-tests/StaticVectorException.cpp
                                     static-vector-tests/StaticVectorIterator.cpp
                                     static-vector-tests/StaticVectorPush.cpp
                                     static-vector-tests/StaticVectorNotReduce.cpp
                                     )

