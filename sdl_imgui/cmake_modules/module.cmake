macro(module_message msg)
	message("${MODULE_NAME}: ${msg}")
endmacro(module_message)

macro(set_module_name module_name)
	module_message("Set MODULE_NAME to '${module_name}'")
	set(MODULE_NAME ${module_name})
endmacro(set_module_name)

macro(setup_module_includes)
	set(${MODULE_NAME}_INCLUDES
		${CUSTOM_INCLUDES}
		${INCLUDES}
		"${CMAKE_CURRENT_SOURCE_DIR}/include"
	)
	set(${MODULE_NAME}_INCLUDES ${${MODULE_NAME}_INCLUDES} PARENT_SCOPE)
	message("Set ${MODULE_NAME}_INCLUDES: ${${MODULE_NAME}_INCLUDES}")
	if(NOT "${${MODULE_NAME}_INCLUDES}" STREQUAL "")
		include_directories(${${MODULE_NAME}_INCLUDES})
	endif()
endmacro(setup_module_includes)

macro(copy_runtime_deps)
	if(WIN32)
		add_custom_command(TARGET ${MODULE_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${MODULE_NAME}> $<TARGET_FILE_DIR:${MODULE_NAME}>
			COMMAND_EXPAND_LISTS
		)
	endif()
endmacro(copy_runtime_deps)

macro(module_add_library_or_executable)
	if (NOT DEFINED HEADERS)
		set_headers()
	endif()
	setup_module_includes()
	setup_src()
	if(BUILD_LIBRARY)
		module_message("BUILD_LIBRARY")
		# TODO: parameterize.
		# If don't pass STATIC then it uses STATIC by default, but SHARED if the parent is SHARED.
		add_library(${MODULE_NAME} STATIC ${SRC})
	elseif(BUILD_SHARED_LIBRARY)
		module_message("BUILD_SHARED_LIBRARY")
		add_library(${MODULE_NAME} SHARED ${SRC})
	else()
		module_message("BUILD_EXECUTABLE")
		add_executable(${MODULE_NAME} ${SRC})
	endif()
	target_link_libraries(${MODULE_NAME} PUBLIC ${DEPENDENCY_LIBRARIES})
	module_message("target_link_libraries(${MODULE_NAME} PUBLIC ${DEPENDENCY_LIBRARIES}")
	if(BUILD_SHARED_LIBRARY OR NOT BUILD_LIBRARY)
		copy_runtime_deps()
	endif()
endmacro(module_add_library_or_executable)

macro(module_add_library)
	set(BUILD_SHARED_LIBRARY OFF)
	set(BUILD_LIBRARY OFF)
	if (ARGC GREATER 0)
		# module_message("argv0: ${ARGV0}")
		if ("${ARGV0}" STREQUAL "SHARED")
			set(BUILD_SHARED_LIBRARY ON)
		else()
			module_message("Build static library because of the argument passed: ${ARGV0}")
			set(BUILD_LIBRARY ON)
		endif()
	else()
		module_message("Build static library")
		set(BUILD_LIBRARY ON)
	endif()
	module_add_library_or_executable()
endmacro(module_add_library)

macro(module_add_executable)
	module_add_library_or_executable()
endmacro(module_add_executable)
