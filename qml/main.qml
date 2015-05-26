import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.1
import QtQuick.Window 2.1
import QuickPlot 1.0

ApplicationWindow {
    id:appWindow
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
        id:menuBar
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
        anchors.top: menuBar.bottom
        anchors.fill: parent
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
            ToolbarSeparator{}
            ToolButton {
                id: syncTool
                action: syncAction
            }

            Item { Layout.fillWidth: true }
        }
    }

                function appendSamples(samples){
                    for (var prop in samples) {
                        if(prop === "ECG")
                            {
                                meterECG.appendDataPoint(samples[prop])
                            }
                        if(prop === "BCGx")
                            {
                                meterBCGx.appendDataPoint(samples[prop])
                            }
                        if(prop === "BCGy")
                            {
                                meterBCGy.appendDataPoint(samples[prop])
                            }
                        if(prop === "BCGz")
                            {
                                meterBCGz.appendDataPoint(samples[prop])
                            }
                    }
                }

        ColumnLayout{
            id: myColumnLayout
            //Layout.fillWidth: true
            //Layout.fillHeight: true
            anchors.fill: parent
            anchors.margins: 5

                    PlotArea {
                    id: plotECG
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    anchors.horizontalCenter: parent.horizontalCenter
                    hasXTicks: false
                    hasYTicks: false
                    yScaleEngine: TightScaleEngine {
                    }

                    items: [
                        ScrollingCurve {
                        id: meterECG
                        numPoints: 600
                        }
                    ]
                    }
                    Text {
                        text: "ECG"
                        id: ecgText
                        font.family: "Helvetica"
                        anchors.verticalCenter: plotECG.verticalCenter
                        verticalAlignment: Text.AlignVCenter
                        font.pointSize: 12
                    }
                    PlotArea {
                    id: plotBCGx
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: plotECG.bottom
                    hasXTicks: false
                    hasYTicks: false
                    yScaleEngine: TightScaleEngine {
                    }

                    items: [
                        ScrollingCurve {
                        id: meterBCGx
                        numPoints: 600
                        }
                    ]
                    }
                    Text {
                        text: "BCGx"
                        id: bcgxText
                        font.family: "Helvetica"
                        anchors.verticalCenter: plotBCGx.verticalCenter
                        verticalAlignment: Text.AlignVCenter
                        font.pointSize: 12
                    }
                    PlotArea {
                    id: plotBCGy
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: plotBCGx.bottom
                    hasXTicks: false
                    hasYTicks: false
                    yScaleEngine: TightScaleEngine {
                    }

                    items: [
                        ScrollingCurve {
                        id: meterBCGy
                        numPoints: 600
                        }
                    ]
                    }
                    Text {
                        text: "BCGy"
                        id: bcgyText
                        font.family: "Helvetica"
                        anchors.verticalCenter: plotBCGy.verticalCenter
                        verticalAlignment: Text.AlignVCenter
                        font.pointSize: 12
                    }
                    PlotArea {
                    id: plotBCGz
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: plotBCGy.bottom
                    hasXTicks: false
                    hasYTicks: false
                    yScaleEngine: TightScaleEngine {
                    }

                    items: [
                        ScrollingCurve {
                        id: meterBCGz
                        numPoints: 600
                        }
                    ]
                    }
                    Text {
                        text: "BCGz"
                        id: bcgzText
                        font.family: "Helvetica"
                        anchors.verticalCenter: plotBCGz.verticalCenter
                        verticalAlignment: Text.AlignVCenter
                        font.pointSize: 12
                    }
                    TextArea {
                        id: myConsole
                        frameVisible: true
                        anchors.top: plotBCGz.bottom
                        anchors.bottom: parent.bottom
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        //height: parent.height
                    }
        }



signal connectClicked()
signal startStopClicked()
signal syncClicked()

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
    Action {
        id: syncAction
        text: "sync data"
        //iconSource: "images/textjustify.png"
        //iconName: "edit-copy"
        onTriggered: syncClicked()
    }

}
