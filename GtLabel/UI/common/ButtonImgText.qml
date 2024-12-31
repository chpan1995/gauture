import QtQuick
import QtQuick.Controls

Button {
    id:btn
    antialiasing:true
    property string urlSelected
    required property string urlNormal
    required property string txtNormalColor
    property string txtSelectedColor: "balck"

    property string hovercolor: "transparent"
    property string normalcolor: "transparent"

    property string imagecolor: "#666666"
    property string imagehovercolor: "#1C76E0"

    property bool selected: false
    property int radius:0
    property int space: 8
    property var pBorder: {"width":0,"color":"transpant"}

    hoverEnabled : true
    background: Rectangle {
        antialiasing:true
        color:{
            if(btn.hovered){
                return btn.hovercolor;
            }else {
                return btn.normalcolor;
            }
        }
        border{
            width:btn.pBorder.width
            color:btn.pBorder.color
        }

        radius:btn.radius
    }
    contentItem:Item {
        Row {
            anchors.centerIn:parent
            width:childrenRect.width
            height:parent.height
            spacing:btn.space
            Image {
                id:img
                anchors.verticalCenter:parent.verticalCenter
                smooth: true
                source: btn.selected ? btn.urlSelected : btn.urlNormal
                width:sourceSize.width
                height:sourceSize.height
            }
            Text {
                anchors.verticalCenter:parent.verticalCenter
                width:implicitWidth
                text:btn.text
                font:btn.font
                color:btn.selected ? btn.txtSelectedColor : btn.txtNormalColor
            }
        }
    }

}
