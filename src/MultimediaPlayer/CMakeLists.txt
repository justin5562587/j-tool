aux_source_directory(. MULTIMEDIA_PLAYER_SRC)

add_library(j_multimedia_player STATIC ${MULTIMEDIA_PLAYER_SRC})

#find_path(AVCODEC_INCLUDE_DIR libavcodec/avcodec.h)
#find_library(AVCODEC_LIBRARY avcodec)
#
#find_path(AVFORMAT_INCLUDE_DIR libavformat/avformat.h)
#find_library(AVFORMAT_LIBRARY avformat)
#
#find_path(AVUTIL_INCLUDE_DIR libavutil/avutil.h)
#find_library(AVUTIL_LIBRARY avutil)
#
#find_path(AVDEVICE_INCLUDE_DIR libavdevice/avdevice.h)
#find_library(AVDEVICE_LIBRARY avdevice)

#set(SDL2_DIR "/Users/justin/cpp/THIRD_PART_LIBRARY/SDL2-2.0.12")
#find_package(SDL2 REQUIRED)

#target_include_directories(
#        j_multimedia_player
#        PRIVATE ${AVCODEC_INCLUDE_DIR} ${AVFORMAT_INCLUDE_DIR} ${AVUTIL_INCLUDE_DIR} ${AVDEVICE_INCLUDE_DIR}
#)
#
#target_link_libraries(
#        j_multimedia_player
#        PRIVATE ${AVCODEC_LIBRARY} ${AVFORMAT_LIBRARY} ${AVUTIL_LIBRARY} ${AVDEVICE_LIBRARY}
#)
