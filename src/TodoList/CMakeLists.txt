aux_source_directory(. TODO_LIST_SRC)

add_library(j-todo-list STATIC ${TODO_LIST_SRC})

find_package(Qt5 COMPONENTS Gui Widgets REQUIRED)

target_link_libraries(j-todo-list Qt::Gui Qt5::Widgets)
