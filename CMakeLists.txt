cmake_minimum_required(VERSION 3.23)
project(ReferenceGame)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent) # once in the project to include the module

# Allows to have local librobots for development and tests. Uses the
# librobots from git if the local directory is not present.
if (EXISTS librobots/)
    add_subdirectory(librobots)
else ()
    FetchContent_Declare(librobots
            GIT_REPOSITORY https://github.com/HEIGVD-PRG1-F-2022/librobots.git
            GIT_TAG v0.2.6
            )
    FetchContent_MakeAvailable(librobots)
endif ()

# Allows to have local libdio for development and tests. Uses the
# libdio from git if the local directory is not present.
if (EXISTS libdio/)
    add_subdirectory(libdio)
else ()
    FetchContent_Declare(libdio
            GIT_REPOSITORY https://github.com/HEIGVD-PRG1-F-2022/libdio.git
            GIT_TAG v0.2.3
            )
    FetchContent_MakeAvailable(libdio)
endif ()

add_executable(ReferenceGame main.cpp Game.cpp robot/RobotSmart.cpp robot/RobotsSimple.cpp)

target_link_libraries(ReferenceGame PRIVATE librobots libdio)
