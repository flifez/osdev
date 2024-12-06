cmake_minimum_required(VERSION 3.0)

project(osdev NONE)

add_custom_target(
        osdev ALL
        COMMAND make
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)