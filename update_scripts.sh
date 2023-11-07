#!/bin/bash

function update_scripts()
{
	local THIS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
	cd "$THIS_DIR"
	
	source automation_config.sh
	source $automation_dir/templates/templates_config.sh

	[ $? -ne 0 ] && echo "Get automation_config.sh manually for the first time. It contains paths to dependent directories" && return 3
	
	cp "$automation_dir/automation_config.sh" .
	cp "$scripts_dir/include/log.sh" .
	cp "$scripts_dir/include/os.sh" .
	cp "$scripts_dir/include/input.sh" .
	cp "$scripts_dir/include/file_utils.sh" .
	cp "$templates_dir/Scripts/update_cmake_modules.sh" .

	source log.sh
	local log_prefix="[update_scripts]: "

	[ $? -ne 0 ] && log_error "Error while delivering files" && return 1

	# deliver build scripts
	$automation_dir/run.sh \
		"$automation_dir/build_sh/deliver_build_scripts_job.sh" \
			"$THIS_DIR"

	[ $? -ne 0 ] && log_error "Error while delivering files" && return 2

	./update_cmake_modules.sh .
	[ $? -ne 0 ] && log_error "Error while updating cmake modules in root directory" && return 3

	./update_cmake_modules.sh qt
	[ $? -ne 0 ] && log_error "Error while updating cmake modules in qt directory" && return 4

	./update_cmake_modules.sh sdl_imgui
	[ $? -ne 0 ] && log_error "Error while updating cmake modules in sdl_imgui directory" && return 5


	log_success "Done"
}

update_scripts $@