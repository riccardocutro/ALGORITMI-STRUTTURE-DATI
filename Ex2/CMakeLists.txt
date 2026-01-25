cmake_minimum_required(VERSION 3.10)
project(Ex2_HashTable C)

set(CMAKE_C_STANDARD 11)

include_directories(include)
include_directories(../Unity/src)

set(LIBRARY_SOURCES src/hash_table.c)

add_executable(main_ex2
        main/main_ex2.c
        ${LIBRARY_SOURCES}
)

add_executable(test_ex2
        test/test_ex2.c
        ../Unity/src/unity.c
        ${LIBRARY_SOURCES}
)