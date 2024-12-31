import QtQuick
import ui_main 1.0
import QtQuick.Layouts

Item {
    id:rootDataAnnotation
    Item {
        height:56
        width:parent.width
        ButtonImgText{
            anchors.verticalCenter:parent.verticalCenter
            anchors.right:parent.right
            anchors.rightMargin:15
            width:110
            height:40
            urlNormal:"qrc:/images/upload.png"
            hovercolor:"#90FFFFFF"
            text:"数据上传"
            radius:8
            txtNormalColor:"#29364A"
            font.pixelSize:16
        }
    }

    Rectangle {
        anchors.top:parent.top
        anchors.topMargin:56
        anchors.left:parent.left
        anchors.leftMargin:24
        anchors.right:parent.right
        anchors.rightMargin:24
        anchors.bottom:parent.bottom
        anchors.bottomMargin:24
        radius:8
        color:"#FFFFFF"
        border.color:"#ECECEC"
        border.width:1
        Rectangle {
            anchors.left:parent.left
            anchors.leftMargin:24
            anchors.top:parent.top
            anchors.topMargin:24
            anchors.bottom:parent.bottom
            anchors.bottomMargin:24
            width:parent.width*0.6
            radius:4
            border{
                width:1
                color:"#E5E5E5"
            }
            ColumnLayout {
                anchors.fill:parent
                anchors.margins:1
                spacing:0
                Rectangle {
                    Layout.fillWidth:true
                    Layout.fillHeight:true
                    Layout.verticalStretchFactor:1000
                    color:"#E5E5E5"
                    Layout.minimumHeight: 50
                }
                Rectangle {
                    Layout.fillWidth:true
                    Layout.fillHeight:true
                    Layout.verticalStretchFactor:168
                    color:"#FFFFFF"
                    Layout.minimumHeight: 50
                }
            }
        }
    }
}
