import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Label {
    // function debug() {
    //     console.log("Text.qml: width: " + width);
    //     console.log("Text.qml: Layout.maximumWidth: " + Layout.maximumWidth);
    //     console.log("Text.qml: parent.Layout.maximumWidth: " + parent.Layout.maximumWidth);
    //     console.log("Text.qml: parent.objectName: " + parent.objectName);
    //     console.log("Text.qml: parent.width: " + parent.width);
    // }
    // onTextChanged: {
    //     debug();
    // }
    // Component.onCompleted: {
    //     debug();
    // }
    wrapMode: Text.WordWrap
    // This works due to parent.Layout.maximumWidth set to ~Infinity by default
    Layout.maximumWidth: Math.min(parent.Layout.maximumWidth, width)
}
