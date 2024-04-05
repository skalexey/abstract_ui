
function job()
{
	local THIS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
	source $THIS_DIR/os.sh

	if is_windows; then
		export abstract_ui_deps="${HOME}/Projects"
	else
		export abstract_ui_deps="${HOME}/Projects"
	fi

	[ ! -z abstract_ui_deps ] && build_deps=$abstract_ui_deps
}

job $@