aux_source_directory(. IMAGE_BROWSER_SRC)

add_library(j-image-browser STATIC ${IMAGE_BROWSER_SRC})

find_package(Qt5 COMPONENTS Gui Widgets REQUIRED)

target_link_libraries(j-image-browser Qt::Gui Qt5::Widgets)