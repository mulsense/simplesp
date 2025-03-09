set(target "simplesp")
message(STATUS "${target}")

project(${target})

file(GLOB_RECURSE INCLUDE ${CMAKE_SOURCE_DIR}/include/*.h)
file(GLOB_RECURSE SOURCE ${CMAKE_SOURCE_DIR}/source/*.cpp)

source_group("include" FILES ${INCLUDE})
source_group("source" FILES ${SOURCE})

add_library(${target} STATIC ${SOURCE} ${INCLUDE})
