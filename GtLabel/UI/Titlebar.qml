import QtQuick
import "./common"

Item {
    id:titleBar
    height:58
    Image {
        id:logo
        anchors.left : parent.left
        anchors.leftMargin:32-6
        y:(58-height)/2-3
        width:sourceSize.width
        height:sourceSize.height
        smooth: true
        source: "qrc:/images/logo.png"
    }

    MouseArea {
        property bool selected: false;
        property point startPoint: Qt.point(0,0);
        anchors.fill:parent
        hoverEnabled:true
        acceptedButtons: Qt.LeftButton
        onPressed:(mouse) => {
            if(mouse.button === Qt.LeftButton) {
                selected=true;
                let tmp=view.moveStart();
                let globalMosue=mapToGlobal(mouseX,mouseY);
                startPoint.x = globalMosue.x - tmp.x;
                startPoint.y = globalMosue.y - tmp.y;
            }
        }
        onReleased:(mouse) => {
            selected=false;
        }
        Timer {
            id:timer
            interval:1000
            repeat:false
            onTriggered: {
                console.log("onTriggered");
                titleBar.focus=true;
            }
        }
        onPositionChanged:(mouse) => {
            if(selected) {
                let endpoint=mapToGlobal(mouseX,mouseY);
                view.moveing(startPoint,endpoint);
            }
        }
    }
    Row {
        anchors.right:parent.right
        anchors.rightMargin:24
        height:parent.height
        width:implicitWidth
        spacing:32
        ButtonImg {
            id:contstate
            width:40
            height:40
            anchors.verticalCenter:parent.verticalCenter
            url:"qrc:/images/connect.png"
            hovercolor:"#55FFFFFF"
            radius:8
        }
        ButtonImg {
            id:quit
            width:40
            height:40
            anchors.verticalCenter:parent.verticalCenter
            url:"qrc:/images/quit.png"
            hovercolor:"#55FFFFFF"
            radius:8
            onClicked:{
                view.close();
            }
        }
        ButtonImgText {
            id:user
            width:80
            height:40
            anchors.verticalCenter:parent.verticalCenter
            urlNormal:"qrc:/images/user.png"
            hovercolor:"#55FFFFFF"
            imagecolor:"#1C76E0"
            radius:8
            text:"admin"
            font.pixelSize:14
            txtNormalColor:"#333333"
        }
    }

    Row {
        height:parent.height
        width:childrenRect.width
        anchors.centerIn:parent
        Column {
            anchors.verticalCenter:parent.verticalCenter
            height:parent.height
            width:120
            ButtonImgText {
                width:parent.width
                height:parent.height-2
                urlNormal:"qrc:/images/dataBz.png"
                urlSelected:"qrc:/images/dataSelectBz.png"
                hovercolor:"#55FFFFFF"
                imagecolor:"#1C76E0"
                text:"数据标注"
                radius:8
                txtNormalColor:"#333333"
                txtSelectedColor:"#1C76E0"
                selected:true
                font.pixelSize:18
            }
            Rectangle{
                width:parent.width-2
                height:2
                color:"#1C76E0"
                radius:1
            }
        }
    }
}
