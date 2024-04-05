#include <iostream>
#include <DMBCore.h>
#include <utils/log.h>
#include "cpp_writer.h"
#include "cpp_generator.h"

LOG_TITLE("cpp_generator")

namespace utils
{
	namespace ui
	{
		int cpp_generator::generate(const cpp_generator_params& __params)
		{
			dmb::Model m;
			auto& out_dir_path = __params.out_dir_path;
			auto& input_file_path = __params.input_file_path;
			auto& json_branch = __params.json_branch;
			if (m.Load(input_file_path))
			{
				cppw_params params;
				params.cppgen_params = __params;
				params.ignore = __params.ignore;
				const vl::Object* data_ptr = nullptr;
				if (!json_branch.empty())
				{
					if (auto node = m.GetVarNodeRegistry().GetNode(json_branch))
					{
						auto& data = node->GetData()->as<vl::Object>();
						auto last_dot_pos = json_branch.find_last_of(".");
						if (last_dot_pos != std::string::npos)
							params.root_name = json_branch.substr(last_dot_pos + 1);
						else
							params.root_name = json_branch;
						cpp_writer wr(m.GetTypeResolver(), params);
						data.Accept(wr);
					}
					else
					{
						LOG_ERROR("	Can't find json branch by the given path '" << json_branch << "'");
						return 2;
					}
				}
				else
				{
					cpp_writer wr(m.GetTypeResolver(), params);
					m.GetData().Accept(wr);
				}
			}
			else
			{
				LOG_ERROR("	Can't load JSON '" << input_file_path << "'");
				return 1;
			}
			
			return 0;
		}
	}
}