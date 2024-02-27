import QtQuick.Controls
import QtQuick.Layouts


ToolBar {
	property var model
	RowLayout {
		objectName: "content"
		anchors.fill: parent

		ToolButton {
			text: qsTr("TolBaton")
		}

		Menu {
			title: qsTr("Test menu")
		}
	}
}

/*MenuBar {
	property var model
}*/