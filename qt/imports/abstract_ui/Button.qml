import QtQuick
import QtQuick.Controls

Button {
    property var model
    onClicked: function(mouse) {
        model.itemClicked();
    }
}
