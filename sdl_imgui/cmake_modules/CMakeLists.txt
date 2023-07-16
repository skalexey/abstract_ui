# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.15)

project({PROJECT_NAME} C CXX)

macro(setup_module)
	module_message("Setting up module: ${MODULE_NAME}")
	
	# Utils dependency
	set(UTILS_DIR "${DEPS}/Utils")
	file(GLOB_RECURSE UTILS_HEADERS "${UTILS_DIR}/include/*")
	GROUP_SRC("${UTILS_HEADERS}" "${UTILS_DIR}/include")
	module_message("UTILS_DIR: ${UTILS_DIR}")

	# Random dependency
	add_dependency_module(${DEPS}/random_dependency random_dependency)

	# add library
	module_add_library()
endmacro()

include(cmake_modules/modules.cmake)
modules_include_all(cmake_modules)
setup_module_project()
