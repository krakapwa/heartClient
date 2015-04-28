import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.1
import QtQuick.Window 2.1

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    minimumWidth: 400
    minimumHeight: 300
    title: "heartClient"

    MessageDialog {
        id: aboutBox
        title: "About HeartClient"
        text: "This is a basic client"
        icon: StandardIcon.Information
    }

signal connectClicked()
signal startStopClicked()

    // this function is our QML slot
    function appendText(text){
        myConsole.append(text)
    }
    function enableConnectButton(){
        startStopTool.opacity = 1
        connectTool.enabled = true
    }

    function disableConnectButton(){
        connectTool.opacity = 0.5
        connectTool.enabled = false
    }
    function enableStartStopButton(){
        startStopTool.opacity = 1
        startStopTool.enabled = true
    }
    function disableStartStopButton(){
        startStopTool.opacity = 0.5
        startStopTool.enabled = false
    }
    Action {
        id: connectAction
        text: "Connect"
        shortcut: "ctrl+c"
        //iconSource: "images/editredo.png"
        //iconName: "edit-cut"
        onTriggered: connectClicked()
    }

    Action {
        id: startStopAction
        text: "start/stop"
        shortcut: "Ctrl+s"
        //iconSource: "images/textjustify.png"
        //iconName: "edit-copy"
        onTriggered: startStopClicked()
    }
    menuBar: MenuBar {
        Menu {
            title: "&File"
            MenuItem { action: connectAction }
            MenuItem { action: startStopAction }
            MenuItem { text: "Quit"; onTriggered: Qt.quit() }
        }
        Menu {
            title: "&Help"
            MenuItem { text: "About..." ; onTriggered: aboutBox.open() }
        }
    }

    toolBar: ToolBar {
        id: mainToolBar
        width: parent.width
        RowLayout {
            anchors.fill: parent
            spacing: 0
            ToolButton {
                id: connectTool
                action: connectAction
            }
            ToolbarSeparator{}
            ToolButton {
                id: startStopTool
                action: startStopAction
            }


            Item { Layout.fillWidth: true }
        }
    }

    TextArea {
        id: myConsole
        frameVisible: false
        width: parent.width
        height: parent.height
    }
}
