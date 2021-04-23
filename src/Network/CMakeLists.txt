aux_source_directory(. RESOURCES_SRC)

add_library(j_network STATIC ${RESOURCES_SRC} test_a.cpp)

find_path(libevent_INCLUDE_DIRS event.h)
find_library(libevent_LIBRARIES event)

target_include_directories(
        j_network
        PUBLIC ${libevent_INCLUDE_DIRS}
)

target_link_libraries(
        j_network
        PUBLIC ${libevent_LIBRARIES}
)
