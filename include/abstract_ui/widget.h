#pragma once

// Turning off inheritance view dominance warning
#pragma warning( disable : 4250 )

#include <memory>
#include <functional>
#include <string>
#include <utils/vec2.h>
#include <abstract_ui/fwd.h>
#include <abstract_ui/node.h>

namespace utils
{
	namespace ui
	{
		class widget : public virtual node
		{
			public:
				using on_show_cb = std::function<void()>;
				using on_hide_cb = std::function<void()>;

				enum alignment : int
				{
					none = 0
					, center
					, left
					, right
					, top
					, bottom
				};

				

				struct size_policy
				{
					enum type : int
					{
						unchanged = 0
						, fixed
						, automatic
					};
					size_policy::type horizontal = size_policy::type::unchanged;
					size_policy::type vertical = size_policy::type::unchanged;
				};

				bool show() {
					on_before_show();
					m_is_visible = true;
					on_show();
					return m_is_visible;
				}

				void hide() {
					m_is_visible = false;
					on_hide();
				}

				void set_visible(bool visible) {
					if (!visible && m_is_visible)
						hide();
					else if (!m_is_visible && visible)
						show();
				}

				bool is_visible() const {
					return m_is_visible;
				}

				virtual void set_on_show(const on_show_cb& on_show) {
					m_on_show = on_show;
				}
				
				void set_on_hide(const on_hide_cb& on_hide) {
					m_on_hide = on_hide;
				}

				void set_on_before_show(const on_show_cb& on_before_show) {
					m_on_before_show = on_before_show;
				}

				virtual const std::string& get_title() const {
					static std::string default_title = "widget";
					return default_title;
				};
				virtual vec2i get_screen_size() const = 0;
				virtual const vec2i& get_position() const {
					return m_position;
				}
				virtual const vec2i& get_size() const {
					return m_size;
				}
                const vec2f get_relative_position() const {
					return vec2f(get_position()) / vec2f(get_screen_size());
				}
                const vec2f get_relative_size() const {
					return vec2f(get_size()) / vec2f(get_screen_size());
				}

				virtual void set_position(const vec2i& pos) {
					bool position_changed = pos != m_position;
					m_position = pos;
  					on_set_position();
					if (position_changed)
						on_position_changed();
				}
				void set_position(const vec2i& pos, const vec2f& anchor ) {
					set_position(pos - vec2f(get_size()) * anchor);
				}
				void set_position_relative(const vec2f& pos) {
					set_position(pos / vec2f(get_screen_size()));
				}
				void set_position_relative(const vec2f& pos, const vec2f& anchor) {
					set_position(vec2i(pos * vec2f(get_screen_size())), anchor);
				}
				void set_y(int y) {
					set_position({ get_position().x, y });
				}
				void set_x(int x) {
					set_position({ x, get_position().y });
				}
				virtual void set_size(const vec2i& size) {
					if (m_size_policy.horizontal == size_policy::type::automatic && m_size_policy.vertical == size_policy::type::automatic)
						return;
					bool size_changed = size != m_size;
					m_size = size;
					on_set_size();
					if (size_changed)
						on_size_changed();
				}
				void set_size_policy(size_policy::type horizontal, size_policy::type vertical = size_policy::type::unchanged) {
					if (vertical != size_policy::type::unchanged)
						m_size_policy.vertical = vertical;
					if (horizontal != size_policy::type::unchanged)
						m_size_policy.horizontal = horizontal;
					on_set_size_policy();
				}
				void set_size_policy(const size_policy& policy) {
					m_size_policy = policy;
					on_set_size_policy();
				}
				const size_policy& get_size_policy() const {
					return m_size_policy;
				}
				virtual void set_size_relative(const vec2f& size) {
					set_size(size / vec2f(get_screen_size()));
				}
				void set_vertical_alignment(const alignment& align) {
					m_vertical_alignment = align;
					on_vertical_alignment_set();
				}
				void set_horizontal_alignment(const alignment& align) {
					m_horizontal_alignment = align;
					on_horizontal_alignment_set();
				}
				const alignment& get_vertical_alignment() { return m_vertical_alignment; }
				const alignment& get_horizontal_alignment() { return m_horizontal_alignment; }

				const on_show_cb& get_on_show() const {
					return m_on_show;
				}
				
				const on_hide_cb& get_on_hide() const {
					return m_on_hide;
				}
				
				const on_show_cb& get_on_before_show() const {
					return m_on_before_show;
				}

				virtual void set_max_width(int width) = 0;
				virtual int get_max_width() const = 0;
				
			protected:
				virtual void on_horizontal_alignment_set() {};
				virtual void on_vertical_alignment_set() {};
				virtual void on_position_changed() {};
				virtual void on_set_size_policy() {};
				virtual void on_set_position() {};
				virtual void on_size_changed() {};
				virtual void on_set_size() {};
				virtual void on_hide() {
					if (m_on_hide)
						m_on_hide();
				};
				virtual void on_before_show() {
					if (m_on_before_show)
						m_on_before_show();
				};
				virtual void on_show() {
					if (m_on_show)
						m_on_show();
				};

			protected:
				// Actual size widget size can be changed automatically by the implementation,
				// so get_size() const uses it to store the retrieved value.
				// Therefore, the actual value stored in this value may be not the actual size of the widget.
				mutable vec2i m_size;
				// Implementation can change the position
				vec2i m_position;

			private:
				size_policy m_size_policy;
				alignment m_vertical_alignment = alignment::none;
				alignment m_horizontal_alignment = alignment::none;
				on_hide_cb m_on_hide;
				on_show_cb m_on_show;
				on_show_cb m_on_before_show;
				bool m_is_visible = true;
		};

		using widget_ptr = std::shared_ptr<widget>;

		#define REGISTER_WIDGET(factory_t, widget_t) factory_t::instance().template register_creator<typename widget_t::base, widget_t>();
		
		template <typename Factory_t, typename Widget_t>
		struct [[maybe_unused]] registrator
		{
			using widget_t = Widget_t;
			using factory_t = Factory_t;
            registrator() {
                REGISTER_WIDGET(factory_t, widget_t);
			}
		};
		
        #define REGISTER_WIDGET_DECLARATIVELY(factory_t, widget_t) utils::ui::registrator<factory_t, widget_t> g_registrator;
	}
}
