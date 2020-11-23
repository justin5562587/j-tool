cmake_minimum_required(VERSION 3.17)
project(j_tool LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# add switch
option(USE_LIBRARY "Compile j-utils into project?" ON)

set(INCROOT ${CMAKE_CURRENT_SOURCE_DIR}/include/)
set(SRCROOT ${CMAKE_CURRENT_SOURCE_DIR}/src/)

# add the  header path
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(CORE_COMPONENTS_HEADER
        ${INCROOT}/TodoList.h
        ${INCROOT}/TodoItem.h
        ${INCROOT}/AddTodoItemDialog.h
        ${INCROOT}/ImageBrowser.h
        ${INCROOT}/PDFProcessor.h
        ${INCROOT}/MainWindow.h
        )

set(CORE_COMPONENTS_SRC
        ${SRCROOT}/TodoList.cpp
        ${SRCROOT}/TodoItem.cpp
        ${SRCROOT}/AddTodoItemDialog.cpp
        ${SRCROOT}/ImageBrowser.cpp
        ${SRCROOT}/PDFProcessor.cpp
        ${SRCROOT}/MainWindow.cpp
        )

add_library(j_core_components STATIC ${CORE_COMPONENTS_HEADER} ${CORE_COMPONENTS_SRC})

set(MAIN_SRC
        ${SRCROOT}/main.cpp
        )

set(Qt5_DIR "/Users/justin/Qt/5.15.1/clang_64/lib/cmake/QT5")
set(POPPLER_QT5 "/Users/justin/cpp/THIRD_PART_LIBRARY/poppler-20.11.0/cmake/modules")

#find_package(PkgConfig REQUIRED)
#pkg_search_module(POPPLER_QT5 REQUIRED poppler-qt5)

find_package(Qt5 COMPONENTS Gui Widgets REQUIRED)
#find_package(poppler_qt5)

if (USE_LIBRARY)
    # add self custom utils
    add_subdirectory(utils)
    add_executable(j_tool )
    target_link_libraries(j_tool ${MAIN_SRC} j_core_components Qt5::Gui Qt5::Widgets j-utils)
else ()
    add_executable(j_tool )
    target_link_libraries(j_tool ${MAIN_SRC} j_core_components Qt5::Gui Qt5::Widgets)
endif ()
