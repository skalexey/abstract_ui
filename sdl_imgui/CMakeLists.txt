# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.15)

project(abstract_ui_sdl_imgui C CXX)

macro(setup_module)
	module_message("Setting up module: ${MODULE_NAME}")

	# ImGui dependency
	set(IMGUI_DIR "${DEPS}/imgui")
	module_message("IMGUI_DIR: ${IMGUI_DIR}")
	# set(GLFW_DIR "${IMGUI_DIR}/libs/glfw-3.3.8.bin.WIN64")
	# set(GLFW_LIB_DIR "${IMGUI_DIR}/libs/glfw-3.3.8.bin.WIN64/lib-vc2022")

	add_library(imgui STATIC
		${IMGUI_DIR}/imgui.cpp
		${IMGUI_DIR}/imgui_draw.cpp
		${IMGUI_DIR}/imgui_widgets.cpp
		${IMGUI_DIR}/imgui_tables.cpp
		${IMGUI_DIR}/imgui_demo.cpp
		${IMGUI_DIR}/backends/imgui_impl_SDL3.cpp
		${IMGUI_DIR}/backends/imgui_impl_sdlrenderer3.cpp
	)

	# SDL dependency
	set(SDL_DIR ${DEPS}/SDL)
	set(SDL_BUILD_DIR ${SDL_DIR}/${BUILD_DIR_NAME})
	add_subdirectory(${SDL_DIR} ${SDL_BUILD_DIR} EXCLUDE_FROM_ALL)
	set(SDL_INCLUDES ${SDL_DIR}/include)

	set(IMGUI_INCLUDES
		${IMGUI_DIR}
		${IMGUI_DIR}/backends
		${GLFW_DIR}/include
		${SDL_INCLUDES}
	)
	# target_include_directories(imgui PUBLIC ${IMGUI_INCLUDES})
	add_dependency_library(imgui)
	module_add_includes(${IMGUI_INCLUDES})

	# Base dependency	
	add_dependency_module(${DEPS}/abstract_ui abstract_ui)

	# For IDE
	add_headers_to_sources()
	
	# add library
	module_add_library()

	message("TARGETS: ${BUILDSYSTEM_TARGETS}")
	# if(TARGET SDL3::SDL3)
	# 	# It has an implicit dependency on SDL3 functions, so it MUST be added before SDL3::SDL3 (or SDL3::SDL3-static)
	# 	target_link_libraries(${MODULE_NAME} PRIVATE SDL3::SDL3)
	# elseif(TARGET SDL3)
	# 	target_link_libraries(${MODULE_NAME} PRIVATE SDL3)
	# else()
	# 	message( SEND_ERROR "No SDL3 target found!")
	# endif()
	# Link to the actual SDL3 library. SDL3::SDL3 is the shared SDL library, SDL3::SDL3-static is the static SDL libarary.
	target_link_libraries(${MODULE_NAME} PRIVATE SDL3::SDL3)
endmacro()

include(cmake_modules/modules.cmake)
modules_include_all(cmake_modules)
setup_module_project()
