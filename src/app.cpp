#include <climits>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <limits>
#include <cassert>
#include <utils/log.h>
#include <abstract_ui/app.h>
#include <abstract_ui/menu_manager.h>

LOG_TITLE("utils::ui::app");

namespace
{
	utils::ui::app* g_app = nullptr;
	int g_app_count = 0;
}

namespace utils
{
	namespace ui
	{
		app::app(int argc, char* argv[])
			: m_args{argc, argv}
			, m_thread_id(std::this_thread::get_id())
		{
			if (!g_app)
				g_app = this;
			++g_app_count;
		}

		app& app::get()
		{
			assert(g_app_count == 1 && "This method is used only for a single app");
			return *g_app;
		}
		
		int app::do_in_main_thread(const utils::int_cb& job)
		{
			LOG_VERBOSE("do_in_main_thread: main_thread_id: " << get_thread_id() << ", current_thread_id: " << std::this_thread::get_id() << "\n");
			if (std::this_thread::get_id() == get_thread_id())
				return job();
			else
			{
				std::mutex mtx;
				std::unique_lock ul(mtx);
				std::condition_variable cv;
				int result = INT_MAX;
				add_on_update([&](float dt) {
					LOG_VERBOSE("Calling a job from thread " << std::this_thread::get_id() << "\n");
					result = job();
					cv.notify_one();
					return false;
				});
				cv.wait(ul);
				assert(result != INT_MAX);
				return result;
			}
		}

		utils::ui::menu_manager app::menu_manager()
		{
			return utils::ui::menu_manager(*this);
		}
	}
}
