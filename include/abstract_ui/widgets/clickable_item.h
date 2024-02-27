#pragma once

#include <string>
#include <memory>
#include <functional>
#include <abstract_ui/widget.h>
#include <utils/common.h>

namespace utils
{
	namespace ui
	{
		class clickable_item : public virtual widget
		{
		public:
			using on_click_t = utils::void_bool_cb;
			using enable_if_pred_t = std::function<bool()>;
			using base = widget;

			void construct(const std::string& label, const on_click_t& on_click = nullptr)
			{
				m_label = label;
				m_on_click = on_click;
			}

			void set_on_click(const on_click_t& on_click) {
				m_on_click = on_click;
				on_set_on_click(on_click);
			}

			void on_click(bool up) const {
				if (m_on_click)
					m_on_click(up);
			}

			void set_text(const std::string& text) {
				m_label = text;
				on_set_text();
			}

			const std::string& get_text() const {
				return m_label;
			}

			void set_enable_if(const enable_if_pred_t& pred) {
				m_enable_if = pred;
			}

			bool on_update(float dt) override {
				if (m_enable_if)
					set_enabled(m_enable_if());
				return base::on_update(dt);
			}

			bool update(float dt) override {
				return base::update(dt);
			}

			virtual void set_enabled(bool enabled) = 0;
			virtual bool is_enabled() const  = 0;

		protected:
			virtual void on_set_on_click(const on_click_t& on_click) {}
			virtual void on_set_text() {}

		private:
			on_click_t m_on_click;
			std::string m_label;
			enable_if_pred_t m_enable_if;
		};
		using clickable_item_ptr = std::shared_ptr<clickable_item>;
	}
}