cmake_minimum_required( VERSION 2.8 )

# setup noiseutils project
project( noiseutils )

set( SOURCES
noiseutils.cpp
noiseutils.h
)

add_library(${PROJECT_NAME} ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "utils")

target_include_directories(${PROJECT_NAME} PRIVATE
${CMAKE_CURRENT_SOURCE_DIR}/../include
)
