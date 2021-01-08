cmake_minimum_required(VERSION 3.17)
project(j_tool VERSION 0.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/)

set(MAIN_SRC
        ${SRC_DIR}/MainWindow.cpp
        ${SRC_DIR}/MainWindow.h
        ${SRC_DIR}/main.cpp
        )

set(Qt5_DIR "/Users/justin/Qt/5.15.1/clang_64/lib/cmake/QT5")
find_package(Qt5 COMPONENTS Gui Widgets MultimediaWidgets Multimedia REQUIRED)

find_package(SDL2 REQUIRED)

option(USE_J_UTILS "Add j-utils into project?" OFF)
if (USE_J_UTILS)
    add_subdirectory(utils)
endif()

add_subdirectory(${SRC_DIR}/MultimediaPlayer)
add_subdirectory(${SRC_DIR}/PDFProcessor)
add_subdirectory(${SRC_DIR}/Calendar)
add_subdirectory(${SRC_DIR}/ScreenShot)

set(SHARED_QT_LIBS Qt5::Gui Qt5::Widgets)

target_link_libraries(j_multimedia_player PUBLIC ${SHARED_QT_LIBS} Qt5::MultimediaWidgets Qt5::Multimedia)
target_link_libraries(j_pdf_processor PUBLIC ${SHARED_QT_LIBS})
target_link_libraries(j_calendar PUBLIC ${SHARED_QT_LIBS})
target_link_libraries(j_screen_shot PUBLIC ${SHARED_QT_LIBS})

qt5_add_resources(RESOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/resources/source.qrc)

add_executable(j_tool ${MAIN_SRC} ${RESOURCE_FILES})
target_link_libraries(j_tool j_multimedia_player j_pdf_processor j_calendar j_screen_shot)

add_custom_command(
        OUTPUT
            ${CMAKE_CURRENT_BINARY_DIR}/generated/version.h
            ALL
        COMMAND
            ${CMAKE_COMMAND} -D TARGET_DIR=${CMAKE_CURRENT_BINARY_DIR} -P ${CMAKE_CURRENT_SOURCE_DIR}/get-version-info.cmake
        WORKING_DIRECTORY
            ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
        get_version_info
        ALL
        DEPENDS
            ${CMAKE_CURRENT_BINARY_DIR}/generated/version.h
)

add_dependencies(j_tool get_version_info)

target_include_directories(j_tool PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/generated)
