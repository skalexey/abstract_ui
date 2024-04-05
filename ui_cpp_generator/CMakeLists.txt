cmake_minimum_required (VERSION 3.15)

project(ui_cpp_generator C CXX)

macro(setup_module)
	module_message("Setting up module: ${MODULE_NAME}")
	module_message("Parent dir: ${PARENT_DIR}")

	# abstract_ui dependency
	set(ABSTRACT_UI_DIR "${DEPS}/abstract_ui")
	add_dependency_module(${ABSTRACT_UI_DIR} abstract_ui)
	
	# Utils dependency
	set(UTILS_DIR "${DEPS}/Utils")
	module_add_includes(${UTILS_DIR}/include)
	add_src(
		${UTILS_DIR}/src/string_utils.cpp
	)

	# DMBCore dependency + VL as a dependency of DMBCore
	set(DMBCORE_DIR "${DEPS}/DataModelBuilder/Core")
	add_dependency_module(${DMBCORE_DIR} DMBCore)

	# ui_cpp_generator sources
	add_src(
		main.cpp
	)

	# Group sources for IDE
	add_headers_to_sources()
	group_headers()

	# add executable
	module_add_executable()

	# add_custom_command(
	#     TARGET VL PRE_BUILD
	#     COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/get_dependencies.sh ${CMAKE_BUILD_TYPE} "${CMAKE_CURRENT_SOURCE_DIR}"
	#     VERBATIM
	# )

	configure_file(${DEPS}/vocabulary/core/assets/ui/library_window.json ${CMAKE_CURRENT_BINARY_DIR}/test.json COPYONLY)
endmacro()

include(cmake_modules/modules.cmake)
modules_include_all(cmake_modules)
setup_module_project()
