cmake_minimum_required(VERSION 3.14...3.22)

project(collide_standalone LANGUAGES CXX)

if(MSVC)
	# I haven't figured out how to resolve this warning when importing std.core.
    add_compile_options("/wd5050")
endif()

# --- Import tools ----

include(../cmake/tools.cmake)

# ---- Dependencies ----

include(../cmake/CPM.cmake)

CPMAddPackage(
  GITHUB_REPOSITORY jarro2783/cxxopts
  VERSION 3.0.0
  OPTIONS "CXXOPTS_BUILD_EXAMPLES NO" "CXXOPTS_BUILD_TESTS NO" "CXXOPTS_ENABLE_INSTALL YES"
)

CPMAddPackage(
	NAME sfml
	GIT_TAG 2.5.1
	GITHUB_REPOSITORY SFML/SFML
	OPTIONS
		"BUILD_SHARED_LIBS NO" # This tells CMake to build static libraries
		"SFML_USE_STATIC_STD_LIBS YES" # This links SFML against the static runtime
		"SFML_BUILD_AUDIO NO"
		"SFML_BUILD_NETWORK NO"
		"SFML_BUILD_GRAPHICS YES"
		"SFML_BUILD_WINDOW YES"
)

# Add package: shoejunk/sprite
CPMAddPackage(
	NAME sprite
	GITHUB_REPOSITORY shoejunk/sprite
	VERSION 0.2.0
)

# Add package: shoejunk/log
CPMAddPackage(
	NAME log
	GITHUB_REPOSITORY shoejunk/log
	VERSION 1.3.0
)

# Add package: shoejunk/math
CPMAddPackage(
	NAME math
	GITHUB_REPOSITORY shoejunk/math
	VERSION 4.9.0
)

CPMAddPackage(NAME collide SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)

# ---- Create standalone executable ----

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)

add_executable(${PROJECT_NAME} ${sources})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 23 OUTPUT_NAME "collide")

target_link_libraries(${PROJECT_NAME} collide cxxopts sfml-graphics sfml-window sprite log math)
file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
