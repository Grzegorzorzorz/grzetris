cmake_minimum_required(VERSION 3.20)

project("grzetris"
	VERSION 0.0.0
)


if(UNIX AND APPLE)
	set(platform "apple")
	message(WARNING
"!!!ATTENTION!!!
You're using a Mac.
grzetris should work, but I cannot properly test on this platform.
Running this software is at your own risk, I provide no guarantees of \
functionality or safety (see section 15 & 16 of the licence).
The risk of data loss is near zero, but not impossible. It didn't break \
anything on my (linux) machine ;)
Let me know if it works! <contact@ciolek.co.uk>"
)
endif()

if(UNIX AND NOT APPLE)
	set(platform "unix")
endif()

message("Platform: ${platform}")

INCLUDE(FindPkgConfig)
set(PROJECT_LIB_PATH "${PROJECT_SOURCE_DIR}/lib")

find_package(Curses REQUIRED)

include_directories("${PROJECT_LIB_PATH}/json-3.11.3/include")

include_directories("include/")
file(GLOB_RECURSE ${PROJECT_NAME}_SRC "src/*.cpp")

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRC})
target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_compile_definitions(${PROJECT_NAME}
    PUBLIC
	$<$<STREQUAL:$<UPPER_CASE:${platform}>,UNIX>:PLAT_UNIX>
	$<$<STREQUAL:$<UPPER_CASE:${platform}>,APPLE>:PLAT_APPLE>
	$<$<CONFIG:Debug>:BUILD_DEBUG>
	$<$<CONFIG:RelWithDebInfo>:BUILD_DEBUG>
	$<$<CONFIG:Release>:BUILD_RELEASE>
	$<$<CONFIG:MinSizeRel>:BUILD_RELEASE>
)
