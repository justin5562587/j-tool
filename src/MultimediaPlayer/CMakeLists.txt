aux_source_directory(. RESOURCES_SRC)

add_library(j_multimedia_player STATIC ${RESOURCES_SRC})

find_package(SDL2 REQUIRED)
find_package(SFML REQUIRED system window graphics audio)

# configuration for self mac
set(CMAKE_PREFIX_PATH /usr/local/Cellar/ffmpeg/4.3.1_4)

find_path(AVCODEC_INCLUDE_DIR libavcodec/avcodec.h)
find_library(AVCODEC_LIBRARY avcodec)

find_path(AVFORMAT_INCLUDE_DIR libavformat/avformat.h)
find_library(AVFORMAT_LIBRARY avformat)

find_path(AVUTIL_INCLUDE_DIR libavutil/avutil.h)
find_library(AVUTIL_LIBRARY avutil)

find_path(AVDEVICE_INCLUDE_DIR libavdevice/avdevice.h)
find_library(AVDEVICE_LIBRARY avdevice)

find_path(SWSCALE_INCLUDE_DIR libswscale/swscale.h)
find_library(SWSCALE_LIBRARY swscale)

find_path(AVFILTER_INCLUDE_DIR libavfilter/avfilter.h)
find_library(AVFILTER_LIBRARY avfilter)

target_include_directories(
        j_multimedia_player
        PUBLIC ${AVCODEC_INCLUDE_DIR} ${AVFORMAT_INCLUDE_DIR} ${AVUTIL_INCLUDE_DIR} ${AVDEVICE_INCLUDE_DIR} ${SWSCALE_INCLUDE_DIR} ${AVFILTER_INCLUDE_DIR}
        ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(
        j_multimedia_player
        PUBLIC ${AVCODEC_LIBRARY} ${AVFORMAT_LIBRARY} ${AVUTIL_LIBRARY} ${AVDEVICE_LIBRARY} ${SWSCALE_LIBRARY} ${AVFILTER_LIBRARY} ${SDL2_LIBRARIES}
        sfml-system sfml-audio sfml-window sfml-graphics
)
