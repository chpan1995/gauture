pragma ComponentBehavior: Bound
import QtQuick
import ui_main 1.0
import QtQuick.Controls
import Qt.labs.qmlmodels


Popup {
    id:taskinfo
    anchors.centerIn: Overlay.overlay
    property string loadStatus
    required property var taskmodel;
    signal requestImageNames(bool v);
    modal: true
    closePolicy:Popup.NoAutoClose
    background:Rectangle {
        color:"#FFFFFF"
        radius:8
        Text {
            anchors.centerIn:parent
            text:{
                if(taskinfo.loadStatus==="loading") {
                    return "正在请求数据..."
                }else if(taskinfo.loadStatus==="loadError") {
                    return "请求错误"
                }else {
                    return ""
                }
            }
            font.pixelSize:20
        }
    }
    contentItem:Item {
        Item {
            id:dele
            anchors.fill: parent
            anchors.margins: 24
            Text {
                id:toptitle
                anchors.left:parent.left
                height:implicitHeight
                text: "获取数据列表"
                color: "#333333"
                font.pixelSize: 16
                visible: {
                    if(taskinfo.loadStatus==="loading") {
                        return false
                    } else{
                        return true
                    }
                }
            }

            ButtonImg {
                anchors.top:parent.top
                anchors.right:parent.right
                width:14
                height:14
                url:"qrc:/images/close.png"
                onClicked: {
                    taskinfo.close();
                }
                visible: {
                    if(taskinfo.loadStatus==="loading") {
                        return false
                    } else{
                        return true
                    }
                }
            }

            ListView {
                visible: {
                    if(taskinfo.loadStatus==="loading" ||
                       taskinfo.loadStatus==="loadError") {
                        return false
                    } else{
                        return true
                    }
                }
                anchors.left:parent.left
                anchors.top:toptitle.bottom
                anchors.topMargin:24
                anchors.bottom:parent.bottom
                width:parent.width
                model:taskinfo.taskmodel
                spacing:16
                clip:true
                delegate:popdelegat
                ScrollBar.vertical: ScrollBar {
                }
            }
        }


    }

    Component{
        id:popdelegat
        Rectangle {
            id:itm
            required property var taskInfoName
            required property var taskInfoCreateTime
            required property var taskInfoCount
            required property var taskInfoLabCount
            required property var taskContinueCount
            required property var taskType
            required property int taskId
            width:dele.width
            height:72
            border {
                width:1
                color:"#E5E5E5"
            }
            radius:4
            Column {
                spacing:6
                anchors.left:parent.left
                anchors.leftMargin:20.
                height:childrenRect.height
                anchors.verticalCenter:parent.verticalCenter
                Text {
                    text: itm.taskInfoName
                    font.pixelSize:14
                    color:"#333333"
                    height:implicitHeight
                }
                Text {
                    text: itm.taskInfoCreateTime
                    font.pixelSize:14
                    color:"#999999"
                    height:implicitHeight
                }
            }
            Text {
                anchors.left:parent.left
                anchors.leftMargin:438
                text: itm.taskType === TaskInfoItemEnum.DataTypes.TaskInfoRole ?
                          "共" + itm.taskInfoCount + "张"
                        : "共" + itm.taskInfoLabCount+"，已长传"+itm.taskContinueCount
                font.pixelSize:14
                color:"#333333"
                height:implicitHeight
                anchors.verticalCenter:parent.verticalCenter
            }

            // 只能跟在 delegate: DelegateChooser
            // DelegateChooser {
            //     role: "taskType"
            //     DelegateChoice {
            //         roleValue:TaskInfoItemEnum.DataTypes.TaskInfoRole
            //         Text {
            //             anchors.left:parent.left
            //             anchors.leftMargin:438
            //             text: {
            //                 console.log(11111123)
            //                 return "共" + itm.taskInfoCount + "张"
            //             }
            //             font.pixelSize:14
            //             color:"#333333"
            //             height:implicitHeight
            //             anchors.verticalCenter:parent.verticalCenter

            //         }
            //     }
            //     DelegateChoice {
            //         roleValue:TaskInfoItemEnum.DataTypes.TaskContinueRole
            //         Text {
            //             anchors.left:parent.left
            //             anchors.leftMargin:438
            //             text: "继续任务：" + itm.taskInfoCount
            //             font.pixelSize:14
            //             color:"#333333"
            //             height:implicitHeight
            //             anchors.verticalCenter:parent.verticalCenter
            //         }
            //     }
            // }

            MouseArea {
                id:mousea
                anchors.fill:parent
                hoverEnabled:true
                onEntered:{
                    itm.color="#80E5E5E5"

                }
                onExited: {
                    itm.color="#FFFFFF"

                }
            }

            ButtonText {
                anchors.verticalCenter:parent.verticalCenter
                anchors.right:parent.right
                anchors.rightMargin:20
                width:80
                height:32
                text:itm.taskType===TaskInfoItemEnum.DataTypes.TaskInfoRole ? "获取":"继续"
                txtNormalColor:"#1C76E0"
                hovercolor:"#E8F1FC"
                txtSelectedColor:"#1C76E0"
                selectedcolor:"#CCCCCC"
                normalcolor:"#FFFFFF"
                pBorder:{
                    "width":1,"color":"#1C76E0"
                }
                radius:4
                onClicked:{
                    itm.taskType===TaskInfoItemEnum.DataTypes.TaskInfoRole
                    ? qmlLabelImgData.requestImgName(taskInfoName)
                    :qmlLabelImgData.requestImgName(taskInfoName,taskId)
                    taskinfo.close();
                    // pop不能嵌套，还是main pop close导致的？
                    popGetImg.title="获取图片提示"
                    popGetImg.sure=false
                    popGetImg.cancle=false
                    popGetImg.content="正在获取图片数据..."
                    popGetImg.open();

                }
            }

        }
    }

    Connections {
        target:qmlLabelImgData
        function onRequest(v,method){
            if(method===LabelImgNamespace.RequestMethod.TasksInfo) {
                v ? taskinfo.loadStatus="loaded":taskinfo.loadStatus="loadError";
            } else if(method===LabelImgNamespace.RequestMethod.TasksPull) {
                if(v) {
                    popGetImg.sure=true
                    popGetImg.cancle=false
                    popGetImg.content="获取图片数据完成"
                    taskinfo.requestImageNames(true)

                }else {
                    popGetImg.sure=true
                    popGetImg.cancle=false
                    popGetImg.content="获取图片数据失败"
                    taskinfo.requestImageNames(false)
                }
            }
        }
    }
}

