#pragma once

#include <memory>
#include <QObject>
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
				widget() {
					add_on_update([self = this](float dt) {
						self->collect_property_updates();
						return true;
					});
				}
				~widget() override;
				
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
				void set_position(const vec2i& pos ) override;
				void set_size(const vec2i& size) override;
				
			protected:
				void on_qobject_created() override;

			private:
				void collect_property_updates();

			private:
				qt::widget_model* m_model = nullptr;
			};
			using widget_ptr = std::shared_ptr<qt::widget>;
		}
	}
}