import QtQuick
import ui_main 1.0

Item {
    id:root
    Flow {
        id:topLeft
        spacing:12
        clip:false
        width:parent.width*0.53
        Text {
            id:graintype
            y:(parent.height-implicitHeight)/2
            width:implicitWidth
            height:40
            verticalAlignment:Text.AlignVCenter
            text:"谷物种类"
        }
        GCombox {
            id:cbxgraintype
            textRole: "text"
            valueRole: "value"
            leftPadding:12
            width:144
            height:40
            model:[{text:"小麦",value:"wheat"},{text:"玉米",value:"corn"}]
        }
        Rectangle{
            width:root.width
            height:1
            color:"#E5E5E5"
        }
    }
    Row {
        height:40
        spacing:16
        anchors.right:parent.right
        clip:true
        ButtonText{
            txtNormalColor:"#FFFFFF"
            txtSelectedColor:"#FFFFFF"
            normalcolor:"#1C76E0"
            hovercolor:"#DD1C76E0"
            text:"标注"
            font.pixelSize:16
            width:88
            height:40
            radius:4
        }
        ButtonText{
            txtNormalColor:"#1C76E0"
            txtSelectedColor:"#1C76E0"
            normalcolor:"#FFFFFF"
            hovercolor:"#F0F0F0"
            text:"重置"
            font.pixelSize:16
            width:88
            height:40
            radius:4
            pBorder:{
                "width":1,"color":"#1C76E0"
            }
        }
    }

    AllDatatypeModel {
        id:dataTypeModel
    }

    Rectangle{
        y:topLeft.childrenRect.height+12
        anchors.left:parent.left
        width:root.width
        height:parent.height-y
        color:"#E5E5E5"
        Column {
            anchors.fill:parent
            Repeater {
                model:dataTypeModel.allDatas
                Row{
                    required property var modelData
                    Repeater{
                        model:modelData.treeNodes
                        Text{
                            required property string tagName
                            text:tagName
                            width:100
                            height:implicitHeight
                        }
                    }
                }
            }
        }


    }

}
