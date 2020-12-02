# get the latest git commit hash
set(GIT_HASH "unknown")

find_package(Git REQUIRED)

if (GIT_FOUND)
    execute_process(
            COMMAND ${GIT_EXECUTABLE} log -1 --pretty=format:%h
            OUTPUT_VARIABLE GIT_HASH
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
    )
endif ()

configure_file(
        ${CMAKE_CURRENT_LIST_DIR}/version.h.in
        ${TARGET_DIR}/generated/version.h
        @ONLY
)
