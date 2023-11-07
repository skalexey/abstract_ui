# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.15)

project(abstract_ui_qt C CXX)

macro(setup_module)
	module_message("Setting up module: ${MODULE_NAME}")
	# Base dependency
	add_dependency_module(${DEPS}/abstract_ui abstract_ui)

	# add_src(
	# 	include/abstract_ui/qt/widgets/button_model.h
	# 	include/abstract_ui/qt/widgets/text_input_model.h
	# )
	# add library
	module_add_library()
endmacro()

include(cmake_modules/modules.cmake)
modules_include_all(cmake_modules)
setup_module_project()
