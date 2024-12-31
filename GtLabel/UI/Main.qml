import QtQuick
import QtQuick.Effects
import QtQuick.Controls
import ui_dataAnnotation 1.0

Rectangle {
    id: root
    color:"#F6F6F6"
    focusPolicy:Qt.StrongFocus
    Rectangle {
        id:topbar
        height:64
        width:parent.width
        gradient: Gradient {
                 orientation: Gradient.Horizontal
                 GradientStop { position: 0.0; color: "#D4D7DB" }
                 GradientStop { position: 1.0; color: "#E4E7EA" }
             }
        Titlebar {
            id:titleBar
            objectName:"titleBar"
            anchors.top:parent.top
            anchors.topMargin:6
            anchors.left:parent.left
            anchors.leftMargin:6
            anchors.right:parent.right
            anchors.rightMargin:6
        }
        layer.enabled:true
        layer.effect:MultiEffect{
            shadowEnabled:true
            shadowBlur:1
            shadowColor:"#777777"
            shadowVerticalOffset:2
        }

        radius:2
    }
    SwipeView {
        clip:true
        anchors.left:parent.left
        anchors.right:parent.right
        anchors.bottom:parent.bottom
        anchors.top:topbar.bottom
        DataAnnotation{

        }
    }
    radius:2
}
