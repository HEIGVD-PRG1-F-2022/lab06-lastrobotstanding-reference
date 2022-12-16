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
            GIT_TAG v0.2.7
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

#set(CMAKE_OSX_SYSROOT /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk)

add_library(Reference STATIC Game.cpp robot/RobotSmart.cpp robot/RobotsSimple.cpp students/robots/all.cpp)

#add_library(students STATIC
#        students/robots/robot_1/MyRobot1.cpp
#        students/robots/robot_2/BonusState.cpp
#        students/robots/robot_2/MapInfo.cpp
#        students/robots/robot_2/MeinRobot.cpp
#        students/robots/robot_2/Point.cpp
#        )
#add_library(students2 STATIC
#        students/robots/robot_3/action.cpp
#        students/robots/robot_3/point.cpp
#        students/robots/robot_3/sonny_robot.cpp
#        students/robots/robot_4/MonRobot.cpp
#        students/robots/robot_5/RobotFB.cpp
#        students/robots/robot_5/RobotFB_diag.cpp
#        students/robots/robot_6/BorisGuillaumator.cpp
#        students/robots/robot_7/MonRobotGPKD.cpp
#        students/robots/robot_8/RobotTeamR.cpp
#        students/robots/robot_9/RandomRoboto.cpp
#        students/robots/robot_9/Roboto.cpp
#        students/robots/robot_10/SoftIntersRobot.cpp
#        )
#target_link_libraries(students PRIVATE librobots libdio)
#target_link_libraries(students2 PRIVATE librobots libdio)

add_executable(ReferenceGame main.cpp)

target_link_libraries(Reference PRIVATE librobots libdio)
target_link_libraries(ReferenceGame PRIVATE librobots libdio Reference)
