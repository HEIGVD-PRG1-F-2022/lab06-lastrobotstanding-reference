cmake_minimum_required(VERSION 3.23)
project(ReferenceGame)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent) # once in the project to include the module

FetchContent_Declare(librobots
        GIT_REPOSITORY https://github.com/HEIGVD-PRG1-F-2022/librobots.git
        GIT_TAG v0.2.5
        )
FetchContent_MakeAvailable(librobots)
#add_subdirectory(librobots)

#FetchContent_Declare(libdio
#        GIT_REPOSITORY https://github.com/HEIGVD-PRG1-F-2022/libdio.git
#        GIT_TAG v0.2.2
#        )
#FetchContent_MakeAvailable(libdio)
add_subdirectory(libdio)

#add_library(Game OBJECT Game.cpp)
#target_link_libraries(Game PRIVATE librobots libdio)
add_executable(ReferenceGame main.cpp Game.cpp robot/RobotSmart.cpp robot/RobotsSimple.cpp)

target_link_libraries(ReferenceGame PRIVATE librobots libdio)
