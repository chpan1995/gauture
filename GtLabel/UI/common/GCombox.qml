import QtQuick
import QtQuick.Controls.Basic

ComboBox {
    id:control
    property bool selected: false
    delegate: ItemDelegate {
        id: delegate
        required property var model
        required property int index
        width: control.width
        contentItem: Text {
            text: delegate.model[control.textRole]
            color: "#333333"
            font: control.font
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
        }
        background:Button {
            id:btn
            hoverEnabled:true
            background:Rectangle{
                color:btn.hovered?"#DDDDDD":"transparent"
            }
        }

        highlighted: control.highlightedIndex === index
    }
    indicator: Image {
        id: img
        x: control.width - width - control.rightPadding
        y: control.topPadding + (control.availableHeight - height) / 2
        width: sourceSize.width
        height: sourceSize.height
        source:"qrc:/images/down.png"
        Connections {
            target: control
            function onPressedChanged() {
                if(control.pressed){
                    img.rotation===0? img.rotation=180:img.rotation=0
                }}
            function onSelectedChanged() {

                if(!control.selected) {
                    img.rotation=0
                }
            }
        }
        Behavior on rotation {
            NumberAnimation { duration:200 }
        }
    }

    contentItem: Text {
        leftPadding: 0
        rightPadding: control.indicator.width + control.spacing

        text: control.displayText
        font: control.font
        color: control.pressed ? "#333333" : "#333333"
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: 120
        implicitHeight: 40
        border.color: control.pressed ? "#333333" : "#CBCBCB"
        border.width: control.visualFocus ? 2 : 1
        radius: 4
    }
    popup: Popup {
        y: control.height - 1
        width: control.width
        implicitHeight: contentItem.implicitHeight
        padding: 1

        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: control.popup.visible ? control.delegateModel : null
            currentIndex: control.highlightedIndex

            ScrollIndicator.vertical: ScrollIndicator { }
        }

        background: Rectangle {
            border.color: "#CBCBCB"
            radius: 4
        }

        onVisibleChanged :{
            control.selected=visible;
        }
    }
}
