cmake_minimum_required(VERSION 2.8)

project(WatershedImageFilter)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})
if (ITKVtkGlue_LOADED)
    find_package(VTK REQUIRED)
    include(${VTK_USE_FILE})
endif()

file(GLOB_RECURSE SOURCES_CPP
        WatershedSegment.cpp
        )

file(GLOB_RECURSE HEADER
        WatershedSegment.h
        header.h)

add_executable(WatershedImageFilter MACOSX_BUNDLE ${SOURCES_CPP} WatershedImageFilter.cxx)

if( "${ITK_VERSION_MAJOR}" LESS 4 )
    target_link_libraries(WatershedImageFilter ITKReview ${ITK_LIBRARIES})
else( "${ITK_VERSION_MAJOR}" LESS 4 )
    target_link_libraries(WatershedImageFilter ${ITK_LIBRARIES})
endif( "${ITK_VERSION_MAJOR}" LESS 4 )