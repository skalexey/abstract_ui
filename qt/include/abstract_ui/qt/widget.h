#pragma once

#include <memory>
#include <abstract_ui/qt/node.h>
#include <abstract_ui/widget.h>

class QObject;

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			class widget : public virtual qt::node, public virtual ui::widget
			{
				friend class qt::widget_model;
				
			public:
				using base = ui::widget;

				widget() {
					add_on_update([self = this](float dt) {
						self->collect_property_updates();
						return true;
					});
				}

				// *** Position and size type consideration: ***
				// All the positions and sizes are in pixels, therefore integer type is used for such properties.				
				// *** *** *** *** *** *** *** *** *** *** *****
				vec2i get_screen_size() const override;

				widget* as_widget() override {
					return this;
				}

				// Object of the actual widget for getting main properties like size and color. It may be nested into qobject() due to layouting.
				// Position is always taken from qobject() since it is the root object of the widget.
				virtual QObject* widget_qobject() const {
					return qobject();
				}

				const vec2i& get_size() const override;
				const vec2i& get_position() const override;
				void set_position(const vec2i& pos ) override;
				void set_size(const vec2i& size) override;
				void set_max_width(int value) override;
				int get_max_width() const override;
				const qt::widget_model* get_model() const {
					return m_model;
				}
				qt::widget_model* model() {
					return m_model;
				}

			protected:
				int init() override;
				int init(const QUrl& componentUrl, const QVariantMap& initial_properties = {}) override;
				virtual qt::widget_model* create_model() const;
				void on_horizontal_alignment_set() override;
				void on_vertical_alignment_set() override;
				static int convert_horizontal_alignment(alignment align);
				static int convert_vertical_alignment(alignment align);

			private:
				void collect_property_updates();

			private:
				qt::widget_model* m_model = nullptr;
			};
			using widget_ptr = std::shared_ptr<qt::widget>;
		}
	}
}