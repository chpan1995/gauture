pragma ComponentBehavior: Bound
import QtQuick
import ui_main 1.0
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

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
                        property bool fold: modelData.title.fold
                        width:parent.width
                        spacing:12
                        RowLayout {
                            width:topData.width
                            height:20
                            Text{
                                text:modelData.title.title
                                font.pixelSize:16
                                color:"#333333"
                                Layout.fillWidth:true
                                Layout.fillHeight:true
                            }
                            Button {
                                id:tpbtn
                                Layout.preferredWidth:20
                                Layout.fillHeight:true
                                background:Image {
                                    anchors.centerIn:parent
                                    width:sourceSize.width
                                    height:sourceSize.height
                                    source: "qrc:/images/fold.png"
                                    rotation:topNode.fold ? 0 : 180
                                    layer.enabled: true
                                    layer.effect:MultiEffect {
                                        colorization:1
                                        brightness: 1.0
                                        colorizationColor: {
                                            if(tpbtn.hovered){
                                                return "#1C76E0";
                                            }else {
                                                return "#666666";
                                            }
                                        }
                                    }
                                    Behavior on rotation {
                                        NumberAnimation { duration:200 }
                                    }
                                }
                                onClicked:{
                                    modelData.fold(!modelData.title.fold);
                                }
                            }
                        }
                        Repeater{
                            id:topTagbtns
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
                                            visible:topNode.fold
                                            width: topNode.fold ? 88:0
                                            height: topNode.fold ? 32:0
                                            selected: {
                                                if(modelData.split(",").map(item => item.trim())[2]==="true"){
                                                    topNode.forntBtn=secBtn;
                                                    return true;
                                                }else {
                                                    return false;
                                                }
                                            }
                                            text:modelData.split(",").map(item => item.trim())[0]
                                            inheritsName:modelData.split(",").map(item => item.trim())[1]
                                            font.pixelSize:14
                                            deep:index===0 ? 1 :2
                                            onClicked:{
                                                if(topNode.forntBtn && topNode.forntBtn!==secBtn){
                                                    topNode.forntBtn.selected=false
                                                }
                                                topNode.forntBtn=secBtn;
                                                // console.log(inheritsName);

                                            }
                                            onSelectedChanged:{
                                                topNode.modelData.setSelected(parent.index,index,selected);
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
                        property Repeater repeater:btmTags
                        property bool selected: false
                        Repeater{
                            id:btmTags
                            model:modelData.treeNodes
                            Loader{
                                required property var modelData
                                required property int index
                                required property string tagName
                                required property int deep
                                required property var inheritsName
                                required property bool selected
                                required property bool visiable
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
                    id:btbtn
                    Layout.preferredWidth:20
                    Layout.fillHeight:true
                    background:Image {
                        id:btimg
                        anchors.centerIn:parent
                        width:sourceSize.width
                        height:sourceSize.height
                        source: "qrc:/images/fold.png"
                        layer.enabled: true
                        layer.effect:MultiEffect {
                            colorization:1
                            brightness: 1.0
                            colorizationColor: {
                                if(btbtn.hovered){
                                    return "#1C76E0";
                                }else {
                                    return "#666666";
                                }
                            }
                        }
                        Behavior on rotation {
                            NumberAnimation { duration:200 }
                        }
                    }
                    onClicked:{
                        // qml Repeater下的item 父类不是Repeater，他仅仅代表数据
                        // console.log(parent.parent);
                        parent.parent.parent.selected=!parent.parent.parent.selected;
                        if(parent.parent.parent.selected){
                            btimg.rotation=180
                            parent.parent.parent.modelData.fold(false);
                        }else {
                            parent.parent.parent.modelData.fold(true);
                            btimg.rotation=0
                        }
                    }
                }
            }
        }
    }

    Component {
        id:tagBtn
        ButtonComplex {
            visible:parent.visiable
            id:btncpx
            width:parent.visiable ? 88:0
            height:parent.visiable ? 32:0
            selected: {
                if(parent.selected) {
                    parent.parent.fornt=btncpx;
                    return true;
                }else {
                    return false;
                }
            }
            text:parent.tagName
            inheritsName: {
                let tmp="";
                for (let i = parent.inheritsName.length-1; i >=0 ; i--) {
                    tmp += parent.inheritsName[i];
                    if(i!==0) tmp+="-";
                }
                return tmp;
            }
            font.pixelSize:14
            deep:parent.deep
            onClicked:{
                if(parent.parent.fornt&&parent.parent.fornt!==btncpx) parent.parent.fornt.selected=false
                parent.parent.fornt=btncpx;
                // console.log(inheritsName);
            }
            onSelectedChanged:{
                parent.modelData.qmlSelected(selected);
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

