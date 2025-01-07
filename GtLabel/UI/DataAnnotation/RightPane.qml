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

    Flickable{
        id:cont
        y:topLeft.childrenRect.height+12
        anchors.left:parent.left
        width:root.width+10
        height:parent.height-y
        clip:true
        contentHeight:topData.childrenRect.height+bottoData.childrenRect.height+12
        rightMargin:12
        ScrollBar.vertical: ScrollBar {
            policy:cont.contentHeight>cont.height ? ScrollBar.AlwaysOn : ScrollBar.AsNeeded
        }

        Item{
            width:parent.width
            height:topData.childrenRect.height+bottoData.childrenRect.height+12
            Column {
                id:topData // 二级结构
                anchors.left:parent.left
                anchors.top:parent.top
                width:parent.width
                spacing:12
                Repeater {
                    model:dataTypeModel.allDatas
                    Column{
                        id: topNode
                        required property var modelData
                        property ButtonComplex forntBtn
                        width:parent.width
                        spacing:12
                        RowLayout {
                            width:topData.width
                            height:20
                            Text{
                                text:modelData.title
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
                        Repeater{
                            model:modelData.sortNodes
                            Column {
                                required property var modelData
                                required property int index
                                width:topData.width
                                spacing:12
                                Flow{
                                    width:topData.width
                                    spacing:12
                                    Repeater{
                                        model:modelData
                                        ButtonComplex {
                                            id:secBtn
                                            required property var modelData
                                            required property int index
                                            width:88
                                            height:32
                                            text:modelData
                                            font.pixelSize:14
                                            deep:index===0 ? 1 :2
                                            onClicked:{
                                                if(topNode.forntBtn && topNode.forntBtn!==secBtn){
                                                    topNode.forntBtn.selected=false
                                                }
                                                topNode.forntBtn=secBtn;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        Canvas {
                            width: topNode.width
                            height: 2
                            onPaint: {
                                var ctx = getContext("2d");
                                ctx.strokeStyle = "#D5D5D5"; // 线条颜色
                                ctx.lineWidth = 1; // 线条宽度
                                ctx.setLineDash([6, 4]); // [虚线长度, 间隔长度]
                                ctx.beginPath();
                                ctx.moveTo(0, height / 2);
                                ctx.lineTo(width, height / 2);
                                ctx.stroke();
                            }
                        }
                    }
                }
            }
            Column {
                id:bottoData // // 一级结构
                anchors.left:parent.left
                anchors.top:topData.bottom
                anchors.topMargin:12
                width:parent.width
                spacing:12
                Repeater {
                    model:dataSingleTypeModel.allDatas
                    width:parent.width
                    Flow{
                        required property var modelData
                        required property int index
                        width:parent.width
                        spacing:12
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
                        Canvas {
                            width: parent.width
                            height: 2
                            onPaint: {
                                if(parent.index===dataSingleTypeModel.allDatas.length-1) return
                                var ctx = getContext("2d");
                                ctx.strokeStyle = "#D5D5D5"; // 线条颜色
                                ctx.lineWidth = 1; // 线条宽度
                                ctx.setLineDash([6, 4]); // [虚线长度, 间隔长度]
                                ctx.beginPath();
                                ctx.moveTo(0, height / 2);
                                ctx.lineTo(width, height / 2);
                                ctx.stroke();
                            }
                        }
                    }
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
    }

    Component {
        id:tagBtn
        ButtonComplex {
            id:btncpx
            width:88
            height:32
            text:parent.tagName
            font.pixelSize:14
            deep:parent.deep
            onClicked:{
                if(parent.parent.fornt&&parent.parent.fornt!==btncpx) parent.parent.fornt.selected=false
                parent.parent.fornt=btncpx;
            }
        }
    }


    AnnotationIndexModel {
        id:dataTypeModel
    }

    AnnotationSingleIndexModel{
        id:dataSingleTypeModel
    }
}

