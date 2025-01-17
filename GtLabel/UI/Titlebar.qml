import QtQuick
import "./common"

Item {
    id: titleBar

    height: 58

    Image {
        id: logo

        anchors.left: parent.left
        anchors.leftMargin: 32 - 6
        height: sourceSize.height
        smooth: true
        source: "qrc:/images/logo.png"
        width: sourceSize.width
        y: (58 - height) / 2 - 3
    }
    MouseArea {
        property bool selected: false
        property point startPoint: Qt.point(0, 0)

        acceptedButtons: Qt.LeftButton
        anchors.fill: parent
        hoverEnabled: true

        onPositionChanged: mouse => {
            if (selected) {
                let endpoint = mapToGlobal(mouseX, mouseY);
                view.moveing(startPoint, endpoint);
            }
        }
        onPressed: mouse => {
            if (mouse.button === Qt.LeftButton) {
                selected = true;
                let tmp = view.moveStart();
                let globalMosue = mapToGlobal(mouseX, mouseY);
                startPoint.x = globalMosue.x - tmp.x;
                startPoint.y = globalMosue.y - tmp.y;
            }
        }
        onReleased: mouse => {
            selected = false;
        }

        Timer {
            id: timer

            interval: 1000
            repeat: false

            onTriggered: {
                console.log("onTriggered");
                titleBar.focus = true;
            }
        }
    }
    Row {
        anchors.right: parent.right
        anchors.rightMargin: 20
        height: parent.height
        spacing: 24
        width: implicitWidth

        ButtonImg {
            id: min

            anchors.verticalCenter: parent.verticalCenter
            height: 40
            hovercolor: "#55FFFFFF"
            radius: 8
            url: "qrc:/images/min.png"
            width: 40
            onClicked:{
                view.showMinimized();
            }
        }

        ButtonImg {
            id: max
            anchors.verticalCenter: parent.verticalCenter
            height: 40
            hovercolor: "#55FFFFFF"
            radius: 8
            url: !view.isShowMax ? "qrc:/images/max.png":"qrc:/images/normal.png"
            width: 40
            onClicked:{
                view.isShowMax ? view.showNor():view.showMax();
            }
        }

        ButtonImg {
            id: contstate
            visible:false
            anchors.verticalCenter: parent.verticalCenter
            height: 40
            hovercolor: "#55FFFFFF"
            radius: 8
            url: "qrc:/images/connect.png"
            width: 40
        }
        ButtonImg {
            id: quit
            anchors.verticalCenter: parent.verticalCenter
            height: 40
            hovercolor: "#55FFFFFF"
            radius: 8
            url: "qrc:/images/quit.png"
            width: 40

            onClicked: {
                view.close();
            }
        }
        ButtonImgText {
            id: user

            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 14
            height: 40
            hovercolor: "#55FFFFFF"
            imagecolor: "#1C76E0"
            radius: 8
            text: "admin"
            txtNormalColor: "#333333"
            urlNormal: "qrc:/images/user.png"
            width: 80
        }
    }
    Row {
        anchors.centerIn: parent
        height: parent.height
        width: childrenRect.width

        Column {
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height
            width: 120

            ButtonImgText {
                font.pixelSize: 18
                height: parent.height - 2
                hovercolor: "#55FFFFFF"
                imagecolor: "#1C76E0"
                radius: 8
                selected: true
                text: "数据标注"
                txtNormalColor: "#333333"
                txtSelectedColor: "#1C76E0"
                urlNormal: "qrc:/images/dataBz.png"
                urlSelected: "qrc:/images/dataSelectBz.png"
                width: parent.width
            }
            Rectangle {
                color: "#1C76E0"
                height: 2
                radius: 1
                width: parent.width - 2
            }
        }
    }
}
