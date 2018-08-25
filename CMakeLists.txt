cmake_minimum_required(VERSION 2.8.7)
project(Input C CXX)

# ---
# add include files
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/include)

# ---
# find required packages
include(FindPkgConfig)

pkg_check_modules(SDL2 REQUIRED sdl2)
include_directories(${SDL2_INCLUDE_DIRS})

pkg_check_modules(SDL2-TTF REQUIRED SDL2_ttf)
include_directories(${SDL2-TTF_INCLUDE_DIRS})

MESSAGE( "CMAKE_SOURCE_DIR: " ${CMAKE_SOURCE_DIR})

# ---
# create executable file
set(BIN_NAME Input)

file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/*.c)
set(SRC_LIST
    ${SOURCES}
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/pkg/")
add_executable(${BIN_NAME} ${SRC_LIST})
set_target_properties(${BIN_NAME} PROPERTIES LINKER_LANGUAGE C)

target_link_libraries (${BIN_NAME}
        ${SDL2_LDFLAGS}
        ${SDL2-TTF_LDFLAGS}
)

# copy resource files to output folder
if(EXISTS "${CMAKE_SOURCE_DIR}/res")
    file(COPY "${CMAKE_SOURCE_DIR}/res" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    MESSAGE( "files in 'res' have been copied to output " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()

