macro(check_extra_includes)
	if(NOT "${INCLUDES}" STREQUAL "")
		message("Add extra includes: ${INCLUDES}")
	endif()
endmacro(check_extra_includes)

macro(module_add_includes)
	module_message("module_add_includes")
	foreach(loop_var ${ARGV})
		set(${MODULE_NAME}_CUSTOM_INCLUDES ${${MODULE_NAME}_CUSTOM_INCLUDES} ${loop_var})
	endforeach()
	module_message("${MODULE_NAME}_CUSTOM_INCLUDES: ${${MODULE_NAME}_CUSTOM_INCLUDES}")
endmacro(module_add_includes)