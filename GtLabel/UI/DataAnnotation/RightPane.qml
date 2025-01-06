import QtQuick
import ui_main 1.0
import QtQuick.Controls
import QtQuick.Layouts
pragma ComponentBehavior: Bound

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

    ScrollView{
        y:topLeft.childrenRect.height+12
        anchors.left:parent.left
        width:root.width
        height:parent.height-y
        background:Rectangle{
            color:"#FFFFFF"
        }
        contentItem:Flickable {
            Column {
                id:topData
                anchors.left:parent.left
                anchors.top:parent.top
                width:parent.width
                Repeater {
                    model:dataTypeModel.allDatas
                    width:parent.width
                    Flow{
                        required property var modelData
                        width:parent.width
                        spacing:8
                        property int forntSelect: -1
                        property ButtonComplex fornt
                        Repeater{
                            model:modelData.treeNodes
                            Loader{
                                required property int index
                                required property string tagName
                                required property int deep
                                sourceComponent:{
                                    index===0 ? foldBtn : tagBtn
                                }
                            }
                        }
                    }
                }
            }
            Column {
                id:bottomData // 非二级结构
                anchors.left:parent.left
                anchors.top:topData.bottom
                anchors.topMargin:12
            }
        }
    }

    Component {
        id:foldBtn
        RowLayout {
            property string tagName:parent.tagName
            width:topData.width
            height:20
            Text{
                text:parent.tagName
                font.pixelSize:16
                color:"#333333"
                Layout.fillWidth:true
                Layout.fillHeight:true
            }

            Button {
                Layout.preferredWidth:12
                Layout.fillHeight:true
                background:Image {
                    anchors.centerIn:parent
                    width:sourceSize.width
                    height:sourceSize.height
                    source: "qrc:/images/fold.png"
                }
            }
        }
    }

    Component {
        id:tagBtn
        // js动态创建了
        ButtonComplex {
            id:btncpx
            width:88
            height:32
            text:parent.tagName
            font.pixelSize:14
            deep:parent.deep
            onClicked:{
                // if(parent.parent.forntSelect!=-1) {
                //     parent.parent.children[parent.parent.forntSelect].item.selected=false
                // }
                // parent.parent.forntSelect=parent.index;
                if(parent.parent.fornt) parent.parent.fornt.selected=false
                parent.parent.fornt=btncpx;
            }
        }
    }


    AnnotationIndexModel {
        id:dataTypeModel
    }
}
