cmake_minimum_required(VERSION 3.17)
project(j_tool VERSION 0.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# add switch
#option(USE_LIBRARY "Compile j-utils into project?" ON)

# get git hash
set(GIT_HASH "unknown")
find_package(Git REQUIRED)
if (GIT_FOUND)
    execute_process(
            COMMAND ${GIT_EXECUTABLE} log -1 --pretty=format:%h
            OUTPUT_VARIABLE GIT_HASH
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
    )
endif()

configure_file(
        version.h.in
        generated/version.h
        @ONLY
)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/)

set(MAIN_SRC
        ${SRC_DIR}/MainWindow.cpp
        ${SRC_DIR}/MainWindow.h
        ${SRC_DIR}/main.cpp
        )

set(Qt5_DIR "/Users/justin/Qt/5.15.1/clang_64/lib/cmake/QT5")
find_package(Qt5 COMPONENTS Gui Widgets REQUIRED)

#set(Poppler_ROOT "/Users/justin/cpp/THIRD_PART_LIBRARY/poppler-20.11.0")
#find_package(PkgConfig REQUIRED)
#pkg_check_modules(
#        Poppler
#        REQUIRED poppler
#        IMPORTED_TARGET
#)

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
target_include_directories(j_tool PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/generated)
target_link_libraries(j_tool j-image-browser j-todo-list ${QT_LIBS})
