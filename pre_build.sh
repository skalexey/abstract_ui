#!/bin/bash

function pre_build()
{
	local sdl_imgui_folder="sdl_imgui"
	local qt_folder="qt"
	if [ -z $1 ]; then
		pre_build $sdl_imgui_folder
		pre_build $qt_folder
	else
		./update_cmake_modules.sh .
		[ -z $1 ] && local dir_to_build="." || local dir_to_build=$1
		echo "dir_to_build: '$dir_to_build'"
		local passedFolder=$(basename $dir_to_build)
		[ "$passedFolder" == "$sdl_imgui_folder" ] && ./update_cmake_modules.sh ./$sdl_imgui_folder || echo 0 > /dev/null
		[ "$passedFolder" == "$qt_folder" ] && ./update_cmake_modules.sh ./$qt_folder || echo 0 > /dev/null
	fi
}

pre_build $@