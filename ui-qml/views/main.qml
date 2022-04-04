import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

/*
import QtQuick.Window
import QtQuick.Layouts
import Qt.labs.settings
*/

Window {

    readonly property string applicationName: "Leiopa"

    id: applicationWindow
    title: applicationName
    visible:true
    width: 300
    height: 600

    Material.theme: Material.Dark

    Pane {
        id: toolBar
        height: 32
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0

        Label {
            id: label
            text: applicationName
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 8
        }

        ToolButton {
            id: toolButton
            height: 20
            text: "⋮" // "\u2630" is not correctly displayed
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 0
        }
    }

    Pane {
        id: pane
        opacity: 0.495
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: toolBar.bottom
        anchors.bottom: statusBar.top
        padding: 0
        rightPadding: 0
        leftPadding: 0
        bottomPadding: 0
        topPadding: 0
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0

        Button {
            id: buttonSwitchTheme
            text: qsTr("Light <-> Dark")
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked:
                if (parent.parent.parent.Material.theme === Material.Light)
                    parent.parent.parent.Material.theme = Material.Dark
                else
                    parent.parent.parent.Material.theme = Material.Light
        }
    }

    Pane {
        id: statusBar
        height: 32
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        padding: 0
        topPadding: 0
        bottomPadding: 0
        rightPadding: 0
        leftPadding: 0
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        anchors.bottomMargin: 0
    }

}
