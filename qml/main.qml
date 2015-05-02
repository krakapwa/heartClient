import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.1
import QtQuick.Window 2.1
import QuickPlot 1.0

ApplicationWindow {
    visible: true
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
                    myConsole.append("append test");
                    for (var prop in samples) {
                                //meter.appendDataPoint(samples)
                                myConsole.append("Label: " + prop+ " = " + samples[prop])
                            }
                }

        ColumnLayout{
            Layout.fillWidth: true
            Layout.fillHeight: true
            width: parent.width
            spacing: 2

            PlotArea {
                id: plotECG
                Layout.fillWidth: true
                yScaleEngine: TightScaleEngine {
                max: 1.5
                min: -1.5
                }

                items: [
                ScrollingCurve {
                id: meterECG;
                numPoints: 600
                }
                ]
            }
            PlotArea {
                id: plotBCGx
                anchors.top: plotECG.bottom
                Layout.fillWidth: true

                yScaleEngine: TightScaleEngine {
                max: 1.5
                min: -1.5
                }

                items: [
                ScrollingCurve {
                id: meterBCGx;
                numPoints: 600
                }
                ]
            }
            PlotArea {
                id: plotBCGy
                anchors.top: plotBCGx.bottom
                Layout.fillWidth: true

                yScaleEngine: TightScaleEngine {
                max: 1.5
                min: -1.5
                }

                items: [
                ScrollingCurve {
                id: meterBCGy;
                numPoints: 600
                }
                ]
            }
            PlotArea {
                id: plotBCGz
                anchors.top: plotBCGy.bottom
                Layout.fillWidth: true
                Layout.fillHeight: true

                yScaleEngine: TightScaleEngine {
                max: 1.5
                min: -1.5
                }

                items: [
                ScrollingCurve {
                id: meterBCGz;
                numPoints: 600
                }
                ]
            }
            TextArea {
                id: myConsole
                frameVisible: false
                anchors.top: plotBCGz.bottom
                Layout.fillWidth: true
                Layout.fillHeight: true
                //height: parent.height
            }
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
