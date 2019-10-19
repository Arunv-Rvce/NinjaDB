cmake_minimum_required(VERSION 3.15)
project(NinjaDB C)

set(CMAKE_C_STANDARD 99)

add_executable(NinjaDB main.c constants.c insert.c fileOperations.c db.c)