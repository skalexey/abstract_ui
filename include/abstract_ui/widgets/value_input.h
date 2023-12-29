#pragma once

#include <string>
#include <functional>
#include <utils/string_utils.h>
#include <abstract_ui/widget.h>

namespace utils
{
	namespace ui
	{
		template <typename T>
		class value_input : public virtual widget
		{
			using base = widget;

		public:
			using on_update_t = std::function<void(const T&)>;
			using on_new_value_t = std::function<void(const T&)>;

			value_input(
				const std::string& label
				, const T& default_value = {}
				, const on_update_t& on_update = nullptr
			)
				: m_default_value(default_value)
				, m_on_update(on_update)
			{
				if (!label.empty())
					set_label(label);
			}

			void set_on_update(const on_update_t& on_update) {
				m_on_update = on_update;
			}

			void on_update() const {
				if (m_on_update)
					m_on_update(get_value());
			}

			void on_update(const T& value) {
				if (m_on_update)
					set_value(value);
				on_update();
			}

			void on_update(const T& value) const {
				if (m_on_update)
					m_on_update(value);
			}

			const on_update_t& get_on_update() const {
				return m_on_update;
			}

			void on_show() override {
			}

			void set_label(const std::string& label) {
				m_label = label;
				on_set_label();
			}

			const std::string& get_default_value() const {
				return m_default_value;
			}

			virtual const T& get_value() const = 0;
			virtual void set_value(const T& value) = 0;

			const std::string& get_label() const {
				return m_label;
			}

			void set_on_new_value(const on_new_value_t& on_new_value) {
				m_on_new_value = on_new_value;
				on_set_on_new_value();
			}

			const on_new_value_t& get_on_new_value() const {
				return m_on_new_value;
			}

		protected:
			virtual bool update_input(float dt) {
				return true;
			}

			virtual bool update_text(float dt) {
				return true;
			}

			virtual void on_set_label() {};
			virtual void on_set_on_new_value() {}
			
			bool on_update(float dt) override final {
				if (!update_text(dt))
					return false;
				if (!update_input(dt))
					return false;
				return true;
			}

		private:
			on_update_t m_on_update;
			on_new_value_t m_on_new_value;
			T m_default_value;
			std::string m_label;
		};
	}
}