cmake_minimum_required(VERSION 3.17)
project(j_tool)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(INCROOT ${CMAKE_CURRENT_SOURCE_DIR}/include/)
set(SRCROOT ${CMAKE_CURRENT_SOURCE_DIR}/src/)

# add the  header path
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)


set(FILES_HEADER
        ${INCROOT}/FileConverter.h
        )

set(FILES_SRC
        ${SRCROOT}/FileConverter.cpp
        ${SRCROOT}/main.cpp
        )

#set(SFML_ROOT "/Users/justin/cpp/SFML_BUILD")
set(Qt5_DIR "/Users/justin/Qt/5.15.1/clang_64/lib/cmake/QT5")

#find_package(SFML REQUIRED COMPONENTS graphics window system)
find_package(Qt5 COMPONENTS Gui Widget REQUIRED)

add_executable(j_tool ${FILES_HEADER} ${FILES_SRC})

#target_link_libraries(j_tool sfml-graphics sfml-window sfml-system)
target_link_libraries(j_tool Qt5::Gui Qt5::Widget)
