#include <utility>
#include <iostream>
#include <utils/string_utils.h>
#include <utils/command_line.h>
#include <utils/log.h>
#include "cpp_generator.h"

LOG_TITLE("ui_cpp_generator")

int main(int argc, char* argv[])
{
	std::cout << std::boolalpha;
	if (argc < 2)
	{
		LOG_ERROR("Too few arguments");
		return 1;
	}
	// 1 - input file
	// 2 - output directory path
	LOG("abstract_ui_cpp_generator project");
	utils::ui::cpp_generator gen;
	utils::ui::cpp_generator_params params;
	
	for (int i = 0; i < argc; i++)
	{
		auto arg = argv[i];
		if (i == 1)
			params.input_file_path = arg;
		else if (i == 2)
			params.out_dir_path = arg;
		else
		{
			if (auto opt_val = utils::get_option_value(arg, "-branch"))
				params.json_branch = opt_val.val;
			else if (auto opt_val = utils::get_option_value(arg, "-ignore"))
				params.ignore = opt_val.val;
			else if (auto opt_val = utils::get_option_value(arg, "-setters"))
				params.generate_setters = utils::parse_bool(opt_val.val);
		}
	}
	return gen.generate(params);
}

