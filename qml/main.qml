import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.1
import QtQuick.Window 2.1
import QuickPlot 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    minimumWidth: 640
    minimumHeight: 600
    title: "heartClient"

    MessageDialog {
        id: aboutBox
        title: "About HeartClient"
        text: "This is a basic client"
        icon: StandardIcon.Information
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

                function appendSamples(samples){
                    meter.appendDataPoint(samples)
                }
        Rectangle {
        id: rectPlot
        visible: true
        width: parent.width
        height: 480

            MouseArea {
                anchors.fill: parent
                onClicked: {
                Qt.quit();
                }
            }

            PlotArea {
                id: plotArea
                anchors.fill: parent

                yScaleEngine: TightScaleEngine {
                max: 1.5
                min: -1.5
                }

                items: [
                ScrollingCurve {
                id: meter;
                numPoints: 300
                }
                ]
            }
            /*

            Timer {
                id: timer;
                interval: 20;
                repeat: true;
                running: true;

                property real pos: 0

                onTriggered: {
                meter.appendDataPoint( Math.sin(pos) );
                pos += 0.05;
                }
            }
            */
        } //end Rectangle

        TextArea {
            id: myConsole
            frameVisible: false
            anchors.top: rectPlot.bottom
            width: parent.width
            //height: parent.height
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

}
