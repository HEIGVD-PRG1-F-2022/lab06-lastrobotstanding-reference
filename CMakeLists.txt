cmake_minimum_required(VERSION 3.23)
project(ineiti)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent) # once in the project to include the module

FetchContent_Declare(librobots
        GIT_REPOSITORY https://github.com/HEIGVD-PRG1-F-2022/librobots.git
        GIT_TAG v0.1.2
        )
FetchContent_MakeAvailable(librobots)

FetchContent_Declare(libdio
        GIT_REPOSITORY https://github.com/HEIGVD-PRG1-F-2022/libdio.git
        GIT_TAG split_display
        )
FetchContent_MakeAvailable(libdio)

add_executable(ineiti main.cpp Message.cpp RobotState.cpp Position.cpp Bonus.cpp Direction.cpp Direction.h)
target_link_libraries(ineiti PRIVATE librobots libdio)
