import QtQuick
import QtQuick.Controls

Popup{
    id:gmsbPop
    width:400
    height:280
    anchors.centerIn: Overlay.overlay
    closePolicy:Popup.NoAutoClose
    topMargin:0
    topPadding:0
    modal: true
    required property string title
    property string content
    property bool sure: false
    property bool cancle: false
    signal accept(bool val)
    background:Rectangle {
        color:"#FFFFFF"
        radius:8
    }
    contentItem:Item{
        Text {
            anchors.top:parent.top
            anchors.topMargin:16
            anchors.horizontalCenter:parent.horizontalCenter
            width:implicitWidth
            height:implicitHeight
            text:gmsbPop.title
            font.pixelSize:16
            color:"#333333"
        }

        Text {
            anchors.verticalCenter:parent.verticalCenter
            width:parent.width-20
            height:implicitHeight
            anchors.leftMargin:10
            wrapMode:Text.WordWrap
            horizontalAlignment : Text.AlignHCenter
            text:gmsbPop.content
            color:"#333333"
            font.pixelSize:16
        }

        Row {
            spacing:32
            anchors.bottom:parent.bottom
            anchors.bottomMargin:24
            anchors.horizontalCenter:parent.horizontalCenter
            width: childrenRect.width
            height:40
            ButtonText {
                text:"确认"
                font.pixelSize: 16
                visible:gmsbPop.sure
                height: 40
                hovercolor: "#DD1C76E0"
                normalcolor: "#1C76E0"
                selectedcolor:"#1C76E0"
                radius: 4
                txtNormalColor: "#FFFFFF"
                txtSelectedColor: "#FFFFFF"
                width: 88
                onClicked:{
                    gmsbPop.close();
                    gmsbPop.accept(true);
                }
            }

            ButtonText {
                visible:gmsbPop.cancle
                font.pixelSize: 16
                height: 40
                hovercolor: "#E8F1FC"
                normalcolor: "#FFFFFF"
                selectedcolor:"#CCCCCC"
                pBorder: {
                    "width": 1,
                    "color": "#1C76E0"
                }
                radius: 4
                text: "取消"
                txtNormalColor: "#1C76E0"
                txtSelectedColor: "#1C76E0"
                width: 88
                onClicked:{
                    gmsbPop.close();
                    gmsbPop.accept(false);
                }
            }
        }
    }
}
