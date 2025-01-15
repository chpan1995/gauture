pragma ComponentBehavior: Bound
import QtQuick
import ui_main 1.0
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

import "componentCreationTagPane.js" as MyScript
Item {
    id: root
    signal complexBtnClicked(string sapType,string inherName,int firstIndex,int secondIndex,
                             string topName,int trait,bool selected);
    Flow {
        id: topLeft

        clip: false
        spacing: 12
        width: parent.width * 0.53

        Text {
            id: graintype

            height: 40
            text: "谷物种类"
            verticalAlignment: Text.AlignVCenter
            width: implicitWidth
            font.pixelSize:16
        }
        GCombox {
            id: cbxgraintype

            height: 40
            leftPadding: 12
            model: [
                {
                    text: "小麦",
                    value: "wheat"
                },
                {
                    text: "玉米",
                    value: "corn"
                }
            ]
            textRole: "text"
            valueRole: "value"
            width: 144
            onCurrentIndexChanged:{
                stk.currentIndex=cbxgraintype.currentIndex;
            }
        }
        Rectangle {
            color: "#E5E5E5"
            height: 1
            width: root.width
        }
    }
    Row {
        anchors.right: parent.right
        clip: true
        height: 40
        spacing: 16

        ButtonText {
            font.pixelSize: 16
            height: 40
            hovercolor: "#DD1C76E0"
            normalcolor: "#1C76E0"
            selectedcolor:"#1C76E0"
            radius: 4
            text: "标注"
            txtNormalColor: "#FFFFFF"
            txtSelectedColor: "#FFFFFF"
            width: 88
        }
        ButtonText {
            font.pixelSize: 16
            height: 40
            hovercolor: "#E8F1FC"
            normalcolor: "#FFFFFF"
            selectedcolor:"#CCCCCC"
            pBorder: {
                "width": 1,
                "color": "#1C76E0"
            }
            radius: 4
            text: "重置"
            txtNormalColor: "#1C76E0"
            txtSelectedColor: "#1C76E0"
            width: 88
        }
    }

    StackLayout {
        id:stk
        anchors.left: parent.left

        height: parent.height - y
        width: root.width + 10
        y: topLeft.childrenRect.height + 12

    }
    //////////////////////////////////////

    //////////////////////////////////////

    // AnnotationIndexModel {
    //     id: dataTypeModel

    // }
    // AnnotationSingleIndexModel {
    //     id: dataSingleTypeModel

    // }

    QmlDatatypeModelManage {
        id:qmlDatatypeModelManage
    }

    Component.onCompleted: {
        for(let i=0;i<cbxgraintype.count;i++) {
            // console.log(qmlDatatypeModelManage.getAllDataModel(cbxgraintype.model[i].value).allDatas)

            MyScript.createTagPaneObjects(qmlDatatypeModelManage.getAllDataModel(cbxgraintype.model[i].value)
                ,qmlDatatypeModelManage.getAllSingleDataModel(cbxgraintype.model[i].value));
        }

    }
}
