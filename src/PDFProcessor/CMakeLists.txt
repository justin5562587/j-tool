aux_source_directory(. PDF_PROCESSOR_SRC)

add_library(j-pdf-processor STATIC ${PDF_PROCESSOR_SRC})

find_package(Qt5 COMPONENTS Gui Widgets REQUIRED)

target_link_libraries(j-pdf-processor Qt::Gui Qt5::Widgets)
