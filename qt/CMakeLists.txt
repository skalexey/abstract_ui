# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.15)

project(abstract_ui_qt C CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 COMPONENTS Gui Qml Quick Widgets QuickControls2)
find_package(Qt6 COMPONENTS Core Gui Qml Quick Widgets QuickControls2 REQUIRED)

# To build this library you need Qt 6.2.0 or higher
message("Qt6_VERSION: ${Qt6_VERSION}")

if (Qt6_VERSION VERSION_LESS 6.2.0)
    message(FATAL_ERROR "You need Qt 6.2.0 or newer to build the library.")
endif()

macro(setup_module)
	module_message("Setting up module: ${MODULE_NAME}")
	# Base dependency
	add_dependency_module(${DEPS}/abstract_ui abstract_ui)

	# setup_module_includes()
    # set_headers()
	qt_wrap_cpp(MOC_FILES ${DEPS}/abstract_ui/qt/include/abstract_ui/qt/widget_model.h)
	qt_wrap_cpp(MOC_FILES ${DEPS}/abstract_ui/qt/include/abstract_ui/qt/widgets/button_model.h)
	qt_wrap_cpp(MOC_FILES ${DEPS}/abstract_ui/qt/include/abstract_ui/qt/widgets/text_input_model.h)
	module_message("MOC_FILES: ${MOC_FILES}")
    add_src(
        ${HEADERS} # for IDE
		${MOC_FILES}
    )

	# For IDE
	add_headers_to_sources()

	# Configure as a library
	module_add_library()

	target_link_libraries(${MODULE_NAME} PRIVATE
		Qt${QT_VERSION_MAJOR}::Core
		Qt${QT_VERSION_MAJOR}::Gui
		Qt${QT_VERSION_MAJOR}::Quick
		Qt${QT_VERSION_MAJOR}::Qml
		Qt${QT_VERSION_MAJOR}::Widgets
		Qt${QT_VERSION_MAJOR}::QuickControls2 
	)

endmacro()

include(cmake_modules/modules.cmake)
modules_include_all(cmake_modules)
setup_module_project()
