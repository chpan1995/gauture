import QtQuick
import QtQuick.Effects
import QtQuick.Controls
import ui_dataAnnotation 1.0

Rectangle {
    id: root

    color: "#F6F6F6"
    // focusPolicy: Qt.StrongFocus
    radius: 2

    Rectangle {
        id: topbar

        height: 64
        layer.enabled: true
        radius: 2
        width: parent.width

        gradient: Gradient {
            orientation: Gradient.Horizontal

            GradientStop {
                color: "#D4D7DB"
                position: 0.0
            }
            GradientStop {
                color: "#E4E7EA"
                position: 1.0
            }
        }
        layer.effect: MultiEffect {
            shadowBlur: 1
            shadowColor: "#777777"
            shadowEnabled: true
            shadowVerticalOffset: 2
        }

        Titlebar {
            id: titleBar

            anchors.left: parent.left
            anchors.leftMargin: 6
            anchors.right: parent.right
            anchors.rightMargin: 6
            anchors.top: parent.top
            anchors.topMargin: 6
            objectName: "titleBar"
        }
    }
    SwipeView {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: topbar.bottom
        clip: true

        DataAnnotation {
            objectName: "dataAnnotation"
        }
    }
}
