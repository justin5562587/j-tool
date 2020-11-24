aux_source_directory(. MULTIMEDIA_PLAYER_SRC)

add_library(j-multimedia-player STATIC ${MULTIMEDIA_PLAYER_SRC})

find_package(Qt5 COMPONENTS Gui Widgets REQUIRED)

target_link_libraries(j-multimedia-player Qt::Gui Qt5::Widgets)
