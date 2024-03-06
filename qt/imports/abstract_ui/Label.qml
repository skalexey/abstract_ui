import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Label {
    property var model
    Layout.maximumWidth: parent.Layout.maximumWidth
    wrapMode: Text.WordWrap
}
