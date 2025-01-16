pragma ComponentBehavior: Bound
import QtQuick
import ui_main 1.0
import QtQuick.Layouts
import QtQuick.Effects
import QtQuick.Controls

Item {
    id:rootDataAnnotation
    Item {
        height:56
        width:parent.width
        ButtonImgText{
            anchors.verticalCenter:parent.verticalCenter
            anchors.right:parent.right
            anchors.rightMargin:15
            width:110
            height:40
            urlNormal:"qrc:/images/upload.png"
            hovercolor:"#90FFFFFF"
            text:"数据上传"
            radius:8
            txtNormalColor:"#29364A"
            font.pixelSize:16
        }
    }

    Rectangle {
        anchors.top:parent.top
        anchors.topMargin:56
        anchors.left:parent.left
        anchors.leftMargin:24
        anchors.right:parent.right
        anchors.rightMargin:24
        anchors.bottom:parent.bottom
        anchors.bottomMargin:24
        radius:8
        color:"#FFFFFF"
        border.color:"#ECECEC"
        border.width:1
        Rectangle {
            id:leftpane
            anchors.left:parent.left
            anchors.leftMargin:24
            anchors.top:parent.top
            anchors.topMargin:24
            anchors.bottom:parent.bottom
            anchors.bottomMargin:24
            width:parent.width*0.6
            radius:4
            border{
                width:1
                color:"#E5E5E5"
            }
            ColumnLayout {
                anchors.fill:parent
                anchors.margins:1
                spacing:0
                Rectangle {
                    Layout.fillWidth:true
                    Layout.fillHeight:true
                    Layout.verticalStretchFactor:1000
                    color:"#ECECEC"
                    Layout.minimumHeight: 50
                    // clip:true
                    ImgView {
                        id:imgview
                        anchors.fill:parent
                        source:qmlLabelImgData.imgName
                        onStatusChanged:{
                            if (status === Image.Ready) {

                            }else if(status === Image.Error) {

                            }
                        }
                        onProgressChanged:{
                            progressBar.value=progress
                        }
                        onSourceChanged: {
                            progressBar.value=0;
                        }
                    }
                    GProgressBar {
                        id:progressBar
                        height:2
                        anchors.bottom:parent.bottom
                        width:parent.width
                    }

                    Flow {
                        anchors.left: parent.left
                        anchors.leftMargin: 8
                        anchors.top: parent.top
                        anchors.topMargin: 8
                        width:parent.width-16
                        spacing: 8
                        Repeater {
                            model:qmlLabelImgData.getLabelTags()
                            Rectangle {
                                id:rec
                                required property string inherName
                                required property string sapType
                                required property int trait
                                required property int firstIndex
                                required property int secondIndex
                                height: 32
                                width: childrenRect.width+16
                                radius: 4
                                z:5
                                color: {
                                    let tmp=trait
                                    if(tmp>13)
                                    tmp=tmp%13
                                    switch(tmp){
                                        case 1: return "#800000FF"
                                        case 2: return "#80FFA500"
                                        case 3: return "#CCCC00"
                                        case 4: return "#80FF0000"
                                        case 5: return "#8000FF00"
                                        case 6: return "#8000FFFF"
                                        case 7: return "#80800080"
                                        case 8: return "#FFC0CB"
                                        case 9: return "#A52A2A"
                                        case 10: return "#808080"
                                        case 11: return "#008B8B"
                                        case 12: return "#006400"
                                        case 13: return "#FF1493"
                                        default: return "#800000FF"
                                    }
                                }
                                Row {
                                    spacing: 16
                                    width: childrenRect.width
                                    height: 32
                                    anchors.left: parent.left
                                    anchors.leftMargin: 8
                                    Text {
                                        font.pixelSize: 14
                                        color: "#FFFFFF"
                                        height:implicitHeight
                                        width: implicitWidth
                                        anchors.verticalCenter: parent.verticalCenter
                                        text:inherName
                                    }
                                    Button {
                                        id:btnTagLabel
                                        anchors.verticalCenter: parent.verticalCenter
                                        width: 12
                                        height: 12
                                        background: Image {
                                            source: "qrc:/images/delete.png"
                                            layer.enabled: true
                                            layer.effect:MultiEffect {
                                                colorization:1
                                                brightness: 1.0
                                                colorizationColor: {
                                                    if(btnTagLabel.hovered){
                                                        return "#1C76E0";
                                                    }else {
                                                        return "#99999999";
                                                    }
                                                }
                                            }
                                        }
                                        onClicked:{
                                            qmlLabelImgData.getLabelTags().removeRow(sapType,inherName,trait,firstIndex,secondIndex,false);
                                        }
                                    }
                                }
                                state: "hidden"
                                states: [
                                    State {
                                        name: "hidden"
                                        PropertyChanges { target: rec;  opacity: 0; scale: 0 }
                                    },
                                    State {
                                        name: "visible"
                                        PropertyChanges { target: rec; opacity: 1; scale: 1 }
                                    }
                                ]
                                transitions: [
                                    Transition {
                                        from: "hidden"
                                        to: "visible"
                                        NumberAnimation { properties: "opacity, scale"; duration: 260; easing.type: Easing.OutQuad }
                                    }
                                ]
                                Component.onCompleted: {
                                    rec.state = "visible";
                                }
                            }
                        }
                    }

                }
                Rectangle {
                    Layout.fillWidth:true
                    Layout.fillHeight:true
                    Layout.verticalStretchFactor:168
                    color:"#FFFFFF"
                    Layout.minimumHeight: 50
                    ButtonImgText{
                        id:btnReuestInfo
                        anchors.left:parent.left
                        anchors.leftMargin:32
                        anchors.verticalCenter:parent.verticalCenter
                        width:114
                        height:40
                        urlNormal:"qrc:/images/requireData.png"
                        hovercolor:"#E8F1FC"
                        text:"获取数据"
                        radius:4
                        txtNormalColor:"#1C76E0"
                        font.pixelSize:16
                        pBorder:{
                            "width":1,
                            "color":"#1C76E0"
                        }
                        onClicked:{
                            qmlLabelImgData.requestImgInfo();
                            taskInfoPop.open();
                            taskInfoPop.loadStatus="loading"
                        }
                    }
                    Rectangle{
                        height:parent.height
                        width:childrenRect.width
                        anchors.centerIn:parent
                        Row {
                            height:parent.height
                            width:childrenRect.width
                            spacing:16
                            Repeater {
                                model:ListModel {
                                    ListElement {
                                        name:"tail.png"
                                        value:LabelImgNamespace.PageGo.Tail
                                    }
                                    ListElement {
                                        name:"front10.png"
                                        value:LabelImgNamespace.PageGo.Front10
                                    }
                                    ListElement {
                                        name:"front.png"
                                        value:LabelImgNamespace.PageGo.Front
                                    }
                                    ListElement {
                                        name:"next.png"
                                        value:LabelImgNamespace.PageGo.Next
                                    }
                                    ListElement {
                                        name:"next10.png"
                                        value:LabelImgNamespace.PageGo.Next10
                                    }
                                    ListElement {
                                        name:"end.png"
                                        value:LabelImgNamespace.PageGo.End
                                    }
                                }
                                Button {
                                    id:btn
                                    required property string name
                                    required property var value
                                    property string imageColor: "#666666"
                                    width:56
                                    height:40
                                    anchors.verticalCenter:parent.verticalCenter
                                    background: Rectangle {
                                        Image {
                                            id:img
                                            anchors.centerIn:parent
                                            layer.enabled: true
                                            smooth: true
                                            source: "qrc:/images/" + btn.name
                                            width:sourceSize.width
                                            height:sourceSize.height
                                            layer.effect:MultiEffect {
                                                colorization:1
                                                brightness: 1.0
                                                colorizationColor: btn.imageColor
                                            }
                                        }
                                        gradient: Gradient {
                                            orientation: Gradient.Horizontal
                                            GradientStop { position: 0.0; color: "#E5E5E5" }
                                            GradientStop { position: 1.0; color: "#F6F6F6" }
                                        }
                                        radius:8
                                        border{
                                            width:1
                                            color:btn.hovered ? "#661C76E0":"transparent"
                                        }
                                    }
                                    onClicked:{
                                        qmlLabelImgData.gotoImgs(value);
                                        rithPane.qmlDatatypeModelManage.reset();
                                    }
                                    onPressed:{ btn.imageColor = "#1C55FF" }
                                    onReleased:{ btn.imageColor = "#CC1C76E0" }
                                    onHoveredChanged: {
                                        if(hovered) btn.imageColor="#CC1C76E0"
                                        else btn.imageColor="#666666"
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        RightPane {
            id:rithPane
            anchors.right:parent.right
            anchors.rightMargin:24
            anchors.top:parent.top
            anchors.topMargin:24
            anchors.left:leftpane.right
            anchors.leftMargin:30
            anchors.bottom:parent.bottom
            anchors.bottomMargin:24
        }

        Connections {
            target: rithPane
            function onComplexBtnClicked(sapType,inherName,firstIndex,secondIndex,topName,selected) {
                let trait=0;
                if(selected) qmlLabelImgData.getLabelTags().appendRow(sapType,inherName,firstIndex,secondIndex,topName,trait);
                else qmlLabelImgData.getLabelTags().removeRow(sapType,inherName,trait,firstIndex,secondIndex,true);
            }
        }
    }

    TaskInfo{
        id:taskInfoPop
        taskmodel:qmlLabelImgData.taskInfoModel // C++ to qml 必须为指针才能取他的属性
        width:960
        height:680

        onRequestImageNames:(v)=>{
            btnReuestInfo.visible =!v;
            rithPane.qmlDatatypeModelManage.reset();
        }
    }

    GMessageBox {
        id:popGetImg
        title:"获取图片提示"
        // states: [
        //     State {
        //         name: "load"
        //         PropertyChanges {
        //             target: popGetImg
        //             sure:false
        //             cancle:false
        //             content:"正在获取图片数据..."
        //         }
        //     },
        //     State {
        //         name: "loaded"
        //         PropertyChanges {
        //             target: popGetImg
        //             sure:true
        //             cancle:false
        //             content:"获取图片数据完成"
        //         }
        //         StateChangeScript {
        //             script: taskinfo.requestImageNames(true)
        //         }
        //     },
        //     State {
        //         name: "loadError"
        //         PropertyChanges {
        //             target: popGetImg
        //             sure:true
        //             cancle:false
        //             content:"获取图片数据失败"
        //         }
        //         StateChangeScript {
        //             script: taskinfo.requestImageNames(false)
        //         }
        //     }
        // ]
    }

    QmlLabelImgData {
        id:qmlLabelImgData
    }

    // QmlLabelTags {
    //     id:qmlLabelTags
    // }
}
