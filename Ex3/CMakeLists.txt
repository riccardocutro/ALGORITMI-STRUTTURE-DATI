cmake_minimum_required(VERSION 3.10)
project(Ex3_PriorityQueue C)

set(CMAKE_C_STANDARD 11)

include_directories(include)
include_directories(../Unity/src)

set(SOURCE_FILES
        src/priority_queue.c
        src/hash_table.c
)

add_executable(main_ex3
        main/main_ex3.c
        ${SOURCE_FILES}
)

add_executable(test_ex3
        test/test_ex3.c
        ../Unity/src/unity.c
        ${SOURCE_FILES}
)