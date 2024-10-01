cmake_minimum_required(VERSION 3.20)

project("grzetris"
	VERSION 0.0.0
)

set(PROJECT_LIB_PATH "${PROJECT_SOURCE_DIR}/lib/")

INCLUDE(FindPkgConfig)

find_package(Curses REQUIRED)

include_directories("include/")

file(GLOB_RECURSE ${PROJECT_NAME}_SRC "src/*.cpp")


add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRC})
target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

target_compile_definitions(${PROJECT_NAME} PRIVATE
	$<$<CONFIG:Debug>:
	DEBUG=1
	>
	)
