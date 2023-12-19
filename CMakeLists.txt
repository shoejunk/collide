# collide

cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# ---- Project ----

project(
	collide
	VERSION 1.0.0
	LANGUAGES CXX)

# ---- Fetch CPM ----

set(CPM_DOWNLOAD_VERSION 0.32.0) 
set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
if(NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
  message(STATUS "Downloading CPM.cmake v${CPM_DOWNLOAD_VERSION}")
  file(DOWNLOAD https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake ${CPM_DOWNLOAD_LOCATION})
endif()
include(${CPM_DOWNLOAD_LOCATION})

# ---- Add dependencies via CPM ----
# See https://github.com/TheLartians/CPM.cmake for details and examples

# PackageProject.cmake will be used to make our target installable
CPMAddPackage("gh:TheLartians/PackageProject.cmake@1.8.0")

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

# Add package: shoejunk/math
CPMAddPackage(
	NAME math
	GITHUB_REPOSITORY shoejunk/math
	VERSION 4.9.0
)

# ---- Add source files ----
file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/source/*.ixx")

# ---- Create library ----

# Note: for header-only libraries change all PUBLIC flags to INTERFACE and create an interface
# target: add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME} ${headers} ${sources})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 23)

# being a cross-platform target, we enforce standards conformance on MSVC
target_compile_options(${PROJECT_NAME} PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive-;/experimental:module>")

target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                         $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>
)

target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window math)

# the location where the project's version header will be placed should match the project's regular
# header paths
string(TOLOWER ${PROJECT_NAME}/version.h VERSION_HEADER_LOCATION)

packageProject(
  NAME ${PROJECT_NAME}
  VERSION ${PROJECT_VERSION}
  NAMESPACE ${PROJECT_NAME}
  BINARY_DIR ${PROJECT_BINARY_DIR}
  INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include
  INCLUDE_DESTINATION include/${PROJECT_NAME}-${PROJECT_VERSION}
  VERSION_HEADER "${VERSION_HEADER_LOCATION}"
  COMPATIBILITY SameMajorVersion
)
