import QtQuick.Controls

ToolButton {
	property var model
	text: qsTr("&Item")
	property var menu
	onClicked: function(mouse) {
		if (menu)
			menu.popup();
        model.itemClicked();
    }
}
