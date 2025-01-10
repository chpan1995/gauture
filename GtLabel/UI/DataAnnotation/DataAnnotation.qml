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
                }
                Rectangle {
                    Layout.fillWidth:true
                    Layout.fillHeight:true
                    Layout.verticalStretchFactor:168
                    color:"#FFFFFF"
                    Layout.minimumHeight: 50
                    ButtonImgText{
                        anchors.left:parent.left
                        anchors.leftMargin:32
                        anchors.verticalCenter:parent.verticalCenter
                        width:114
                        height:40
                        urlNormal:"qrc:/images/requireData.png"
                        hovercolor:"#EEEEEE"
                        text:"获取数据"
                        radius:4
                        txtNormalColor:"#1C76E0"
                        font.pixelSize:16
                        pBorder:{
                            "width":1,
                            "color":"#1C76E0"
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
                                model:["tail.png","front10.png","front.png",
                                    "next.png","next10.png","end.png"]
                                Button {
                                    id:btn
                                    required property string modelData
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
                                            source: "qrc:/images/" + btn.modelData
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
                                        switch(btn.modelData){
                                            case "next.png":{
                                                imgview.source="http://192.168.1.108:8082/uploads/p600/paddy/20241210/G800-HY-202412101645-199.830/21-GQ-P600-8888_6_20241210170232_82_00_99_UAUBDADB_4_4707-2507-303-151-4713-2501-310-150_15-15-15-15_13-10.png"
                                            }
                                            default:break;
                                        }
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
            anchors.right:parent.right
            anchors.rightMargin:24
            anchors.top:parent.top
            anchors.topMargin:24
            anchors.left:leftpane.right
            anchors.leftMargin:30
            anchors.bottom:parent.bottom
            anchors.bottomMargin:24
        }
    }
}
