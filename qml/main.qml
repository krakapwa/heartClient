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
            Layout.fillWidth: true
            width: parent.width*0.95
            height: parent.height*0.95

            RowLayout{
                id:rowLayoutECG
                width: myColumnLayout.width
                //anchors.top: myColumnLayout.top
                    Text {
                        text: "ECG"
                        anchors.right: plotECG.left
                        font.family: "Helvetica"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pointSize: 12
                        rotation: -90
                        //verticalAlignment: Text.AlignVCenter
                    }
                    PlotArea {
                    id: plotECG
                    Layout.fillWidth: true
                    Layout.fillHeight: true
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
            }
            RowLayout{
                id:rowLayoutBCGx
                Layout.fillWidth: true
                width: myColumnLayout.width
                anchors.top: rowLayoutECG.bottom
                    Text {
                        text: "BCGx"
                        anchors.right: plotBCGx.left
                        font.family: "Helvetica"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pointSize: 12
                        rotation: -90
                        //verticalAlignment: Text.AlignVCenter
                    }
                    PlotArea {
                    id: plotBCGx
                    Layout.fillWidth: true
                    Layout.fillHeight: true
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
            }
            RowLayout{
                id:rowLayoutBCGy
                Layout.fillWidth: true
                width: myColumnLayout.width
                anchors.top: rowLayoutBCGx.bottom
                    Text {
                        text: "BCGy"
                        anchors.right: plotBCGy.left
                        font.family: "Helvetica"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pointSize: 12
                        rotation: -90
                        //verticalAlignment: Text.AlignVCenter
                    }
                    PlotArea {
                    id: plotBCGy
                    Layout.fillWidth: true
                    Layout.fillHeight: true
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
            }
            RowLayout{
                id:rowLayoutBCGz
                Layout.fillWidth: true
                width: myColumnLayout.width
                anchors.top: rowLayoutBCGz.bottom
                    Text {
                        text: "BCGy"
                        anchors.right: plotBCGz.left
                        font.family: "Helvetica"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pointSize: 12
                        rotation: -90
                        //verticalAlignment: Text.AlignVCenter
                    }
                    PlotArea {
                    id: plotBCGz
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
