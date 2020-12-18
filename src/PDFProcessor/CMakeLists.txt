aux_source_directory(. PDF_PROCESSOR_SRC)

#find_package(PkgConfig REQUIRED)
#pkg_check_modules(
#        Poppler
#        REQUIRED poppler
#        IMPORTED_TARGET
#)

find_package(Poppler REQUIRED)
#find_path(AVCODEC_INCLUDE_DIR libavcodec/avcodec.h)
#find_library(AVCODEC_LIBRARY avcodec)

add_library(j_pdf_processor STATIC ${PDF_PROCESSOR_SRC})

#target_link_libraries(j_pdf_processor PRIVATE PkgConfig::Poppler)
target_link_libraries(j_pdf_processor PRIVATE ${Poppler_LIBRARIES})
target_link_directories(j_pdf_processor PRIVATE ${Poppler_INCLUDE_DIR})
#target_link_libraries(j_pdf_processor PRIVATE ${Poppler_LIBRARIES})
#
#target_link_directories(j_pdf_processor PRIVATE ${Poppler_INCLUDE_DIR})
