import QtQuick
import QtQuick.Controls

Button {
    id:btn
    antialiasing:true

    required property string txtNormalColor
    property string txtSelectedColor: "balck"
    property string selectedcolor: "transparent"
    property string hovercolor: "transparent"
    property string normalcolor: "transparent"

    property bool selected: false
    property int radius:0
    property int space: 8
    property var pBorder: {"width":0,"color":"transpant"}

    hoverEnabled : true
    background: Rectangle {
        antialiasing:true
        color:{
            if(btn.selected) return btn.selectedcolor;
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
            Text {
                anchors.verticalCenter:parent.verticalCenter
                width:implicitWidth
                text:btn.text
                font:btn.font
                color:btn.selected ? btn.txtSelectedColor : btn.txtNormalColor
            }
        }
    }
    onPressed:{
        selected=true;
    }
    onReleased:{
        selected=false;
    }
}
