// This class just adds the ability to work with nodes
// as a qt::node* instead of a ui::node*

#pragma once

#include <QVariantMap>
#include <abstract_ui/qt/fwd.h>
#include <abstract_ui/node.h>

class QObject;
class QUrl;

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			// Hide ui::node* interface using protected inheritance
			class node : public virtual ui::node
			{
				friend class ui::widget_factory;
				using base = ui::node;

			public:
				// TODO: support constructor with qt::node* 
				node();
				~node() override;
				
				bool is_initialized() const override {
					if (!base::is_initialized())
						return false;
					return m_object != nullptr;
				}
				// Contains shared code to be called by derived classes upon the construction
				virtual int init(const QUrl& componentUrl, const QVariantMap& initial_properties = {});

				const qt::node* get_parent() const {
					return get_typed_parent<qt::node>();
				}
				
				qt::node* parent() {
					return const_cast<qt::node*>(get_parent());
				}

				qt::node* root() {
					return dynamic_cast<qt::node*>(base::root());
				}

				virtual qt::widget* as_widget() {
					return nullptr;
				}
				
				// All the dynamic casts require a fully defined type.
				// So we define them in the cpp file.
				qt::app& app();
				const qt::app& get_app() const;

				// Root node of the widget. Some properties like position are taken from it.
				virtual QObject* qobject() const {
					return m_object;
				} // = 0; // TODO: set it to 0 then

				// Object for adding child nodes. It may be nested into qobject() due to layouting.
				virtual QObject* content_qobject() const {
					return m_content;
				} // = 0; // TODO: set it to 0 then

				QObject* parent_qobject() const;

			protected:
				void on_destroy() override;
				void on_set_parent(ui::node* parent) override;

				int init(QObject* object, QObject* content_qobject = nullptr);

			private:
				int post_construct_1() override final;

			private:
				QObject* m_object = nullptr;
				QObject* m_content = nullptr;
				
			};
			using node_ptr = std::shared_ptr<qt::node>;
		}
	}
}