#!/bin/bash

function deps_scenario()
{
    local THIS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
    local log_prefix="-- [$(basename $THIS_DIR) $(basename "${BASH_SOURCE[0]}")]: "
    log_info "Start deps_scenario" " -"
	
    source $THIS_DIR/dependencies.sh
    source $THIS_DIR/deps_config.sh

    download_dependency "VL" "$depsLocation" "git@github.com:skalexey/VL.git"
    download_dependency "rapidjson" "$depsLocation" "https://github.com/Tencent/rapidjson.git"
}

deps_scenario $@
