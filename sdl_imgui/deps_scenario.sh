#!/bin/bash

function deps_scenario()
{
    local THIS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
    local log_prefix="-- [$(basename $THIS_DIR) $(basename "${BASH_SOURCE[0]}")]: "
    log_info "Start deps_scenario" " -"
	
    source $THIS_DIR/dependencies.sh
    source $THIS_DIR/deps_config.sh

    download_dependency "SDL" "$depsLocation" "https://github.com/libsdl-org/SDL.git"
    # SDL2 point used until 05.03.2024
    # commit 10e1ef00e29479f3ac0ba4d0bf3e0083fca8b2a0
    # Author: Cameron Cawley <ccawley2011@gmail.com>
    # Date:   Tue Sep 6 11:21:57 2022 +0100

    download_dependency "stb" "$depsLocation" "https://github.com/nothings/stb.git"
    # download_dependency "SDL_image" "$depsLocation" "https://github.com/libsdl-org/SDL_image.git"
}

deps_scenario $@
