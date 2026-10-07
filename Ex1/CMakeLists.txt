cmake_minimum_required(VERSION 3.10)
project(LaboratorioAlgoritmi_Ex1 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -pedantic -g -O2")

include_directories(
        include
        ../Unity/src
)

set(SOURCE_FILES
        src/utils.c
        src/sort.c
)

add_executable(main_ex1
        main/main_ex1.c
        ${SOURCE_FILES}
)

add_executable(test_ex1
        test/test_ex1.c
        ${SOURCE_FILES}
        ../Unity/src/unity.c
)