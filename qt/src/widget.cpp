#include <QVariant>
#include <QObject>
#include <QJSValue>
#include <abstract_ui/widget.h>
#include <abstract_ui/qt/widget.h>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			widget::~widget()
			{
				if (auto object = qobject())
				{
					QVariant result = object->property("onDestroy");
					if (result.canConvert<QJSValue>()) {
						QJSValue jsFunction = result.value<QJSValue>();
						if (jsFunction.isCallable()) {
							QJSValueList args;
							QJSValue returnValue = jsFunction.call(args);
						}
					}
					object->setParent(nullptr);
					object->deleteLater();
				}
			}
			vec2i qt::widget::get_screen_size() const
			{
				// TODO: implement
				return vec2i(1920, 1080);
			}
		}
	}
}