import QtQuick.Controls
import QtQuick.Layouts


ToolBar {
	property var model
	RowLayout {
		objectName: "content"
		anchors.fill: parent
	}
}

// QML MenuBar does not support buttons, so ToolBar is used instead.
/*MenuBar {
	property var model
}*/