#include <mutex>
#include <atomic>
#include <condition_variable>
#include <limits>
#include <cassert>
#include <utils/Log.h>
#include <abstract_ui/app.h>

LOG_POSTFIX("\n");
LOG_PREFIX("[utils::ui::app]: ");

namespace utils
{
	namespace ui
	{
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
				std::atomic<int> result = INT_MAX;
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
	}
}
