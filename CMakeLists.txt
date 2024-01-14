cmake_minimum_required (VERSION 3.15)

project(abstract_ui C CXX)

macro(setup_module)
	module_message("Setting up module: ${MODULE_NAME}")
	
	# Utils dependency
	set(UTILS_DIR "${DEPS}/Utils")
	module_message("UTILS_DIR: ${UTILS_DIR}")
	module_add_includes(${UTILS_DIR}/include)
	# For IDE
	file(GLOB_RECURSE UTILS_HEADERS "${UTILS_DIR}/include/*")
	GROUP_SRC("${UTILS_HEADERS}" "${UTILS_DIR}/include")
	
	# VL dependency
	set(VL_DIR "${DEPS}/VL")
	add_dependency_module(${VL_DIR} VL)

	# For IDE
	add_headers_to_sources()
	
	# add library
	module_add_library()
endmacro()

include(cmake_modules/modules.cmake)
modules_include_all(cmake_modules)
setup_module_project()
