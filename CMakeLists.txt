cmake_minimum_required(VERSION 3.5)
project(cylangTest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory(lib/cyfw)
include_directories(${CYFW_INCLUDE_DIR})

add_subdirectory(lib/cyril-lang/src)
include_directories(${CYRIL_LANG_INCLUDES})

embed_resources(Fonts resources/a-scp-r.ttf)

set(SOURCE_FILES main.cpp)
add_executable(cylangTest ${SOURCE_FILES} ${Fonts})
target_link_libraries(cylangTest cyfw cyril_lang)
