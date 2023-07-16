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
			class node : protected virtual ui::node
			{
				using base = ui::node;

			public:
				// TODO: support constructor with qt::node* 
				node();
				
				const qt::node* get_parent() const {
					return dynamic_cast<const qt::node*>(base::get_parent());
				}
				
				qt::node* parent() {
					auto base_parent = base::parent();
					auto casted_parent = dynamic_cast<qt::node*>(base_parent);
					assert(base_parent == casted_parent && "The parent is not a qt::node*");
					return casted_parent;
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

				
				virtual QObject* qobject() {
					return m_object;
				} // = 0; // TODO: set it to 0 then

				virtual QObject* get_qobject() const {
					return m_object;
				} // = 0; // TODO: set it to 0 then

				virtual QObject* content_qobject() {
					return m_object;
				} // = 0; // TODO: set it to 0 then

			protected:
				// Contains shared code to be called by derived classes upon the construction
				int init(const QUrl& componentUrl, const QVariantMap& initial_properties);
				
				int init(QObject* object) {
					m_object = object;
					return 0;
				}

				virtual void on_add_node(qt::node* node) {}
				virtual int on_qt_node_post_construct() {
					return 0;
				}

			private:
				int on_post_construct() override final;

				void on_add_node(ui::node* node) override {
					on_add_node(dynamic_cast<qt::node*>(node));
				}

			private:
				QObject* m_object = nullptr;
				
			};
			using node_ptr = std::shared_ptr<qt::node>;
		}
	}
}