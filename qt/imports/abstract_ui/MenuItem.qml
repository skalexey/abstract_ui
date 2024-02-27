import QtQuick.Controls

MenuItem {
	property var model
	text: qsTr("&Item")
	onClicked: function(mouse) {
        model.itemClicked();
    }
}
