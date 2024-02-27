import QtQuick.Controls

ToolButton {
	property var model
	text: qsTr("&Item")
	onClicked: function(mouse) {
        model.itemClicked();
    }
}
