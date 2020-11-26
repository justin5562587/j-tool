cmake_minimum_required(VERSION 3.17)
project(j_tool)

################################################################################################
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# add switch
#option(USE_LIBRARY "Compile j-utils into project?" ON)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/)

set(MAIN_SRC
        ${SRC_DIR}/MainWindow.cpp
        ${SRC_DIR}/MainWindow.h
        ${SRC_DIR}/main.cpp
        )

set(Qt5_DIR "/Users/justin/Qt/5.15.1/clang_64/lib/cmake/QT5")
set(POPPLER_QT5 "/Users/justin/cpp/THIRD_PART_LIBRARY/poppler-20.11.0/cmake/modules")

find_package(Qt5 COMPONENTS Gui Widgets REQUIRED)

# add custom utils
#add_subdirectory(utils)

# add custom core components
add_subdirectory(${SRC_DIR}/ImageBrowser)
add_subdirectory(${SRC_DIR}/TodoList)
#add_subdirectory(${SRC_DIR}/PDFProcessor)
#add_subdirectory(${SRC_DIR}/MultimediaPlayer)

set(QT_LIBS Qt5::Gui Qt5::Widgets)

target_link_libraries(j-image-browser ${QT_LIBS})
target_link_libraries(j-todo-list ${QT_LIBS})

add_executable(j_tool ${MAIN_SRC})
target_link_libraries(j_tool j-image-browser j-todo-list ${QT_LIBS})
