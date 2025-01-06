import QtQuick
import QtQuick.Controls

Button {
    id:btn
    hoverEnabled:true
    required property int deep
    property bool selected: false
    background : Rectangle {
        id:bck
        property string forntState: "normal"
        radius:4
        border.width:1
        state: "normal"
        states: [
            State {
                name: "normal"
                PropertyChanges {
                    target:bck
                    color: {
                        if(btn.deep===1) {
                            return "#E4E7EA"
                        }else if(btn.deep===2) {
                            return "#FFFFFF"
                        }
                    }
                }
                PropertyChanges {
                    target:bck
                    border.color:{
                        if(btn.deep===1) {
                            return "transparent"
                        }else if(btn.deep===2) {
                            return "#CBCBCB"
                        }
                    }
                }
            },
            State {
                name: "hovered"
                PropertyChanges {
                    target:bck
                    border.color:"#1C76E0"
                }
                PropertyChanges {
                    target:bck
                    color: {
                        if(btn.deep===1) {
                            return "#E4E7EA"
                        }else if(btn.deep===2) {
                            return "#FFFFFF"
                        }
                    }
                }
            },
            State {
                name: "selected"
                PropertyChanges {
                    target:bck
                    border.color:"#1C76E0"
                }
                PropertyChanges {
                    target:bck
                    color: "#221C76E0"
                }
            }
            /*,
            State {
                name: "disable"
                PropertyChanges {
                    target:bck
                    border.color:"#77CBCBCB"
                }
            }*/
        ]
    }
    contentItem:Item{
        Text{
            anchors.centerIn:parent
            text:btn.text
            font:btn.font
            color: {
                if(btn.selected) return "#1C76E0"
                return "#333333"
            }
        }
        Image {
            visible: btn.selected
            anchors.bottom:parent.bottom
            anchors.right:parent.right
            anchors.bottomMargin:-5
            anchors.rightMargin:-7
            width:sourceSize.width
            height:sourceSize.height
            source: "qrc:/images/selected.png"
        }
    }
    onHoveredChanged:{
        if(selected) return;
        btn.hovered? bck.state="hovered" : bck.state=bck.forntState
    }
    onClicked:{
        selected=true;

    }
    onSelectedChanged:{
        if(btn.selected){
            bck.state="selected"
            bck.forntState="selected"
        }else {
            bck.state="normal"
            bck.forntState="normal"
        }
    }
}
