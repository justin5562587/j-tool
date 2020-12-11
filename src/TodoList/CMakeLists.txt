aux_source_directory(. TODO_LIST_SRC)

set(CMAKE_AUTORCC ON)

#qt5_add_resources(QRC_FILES resources/source.qrc})

add_library(j_todo_list STATIC ${TODO_LIST_SRC} resources/source.qrc)
#add_library(j_todo_list STATIC ${TODO_LIST_SRC} ${QRC_FILES})