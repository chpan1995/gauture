pragma ComponentBehavior: Bound
import QtQuick
import ui_main 1.0
import ui_dataAnnotation 1.0
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
import "componentCreationTagPane.js" as MyScript

Item {
    id: root
    property alias qmlDatatypeModelManage: qmlDatatypeModelManage
    signal complexBtnClicked(string sapType,string inherName,int firstIndex,int secondIndex,
                             string topName,bool selected);
    property var option: []

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
            model: ListModel {
                id: optmodel
            }
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
            onClicked:{
                if(qmlLabelImgData.lab()) {
                    // 右边种类数据重置
                    qmlDatatypeModelManage.reset();
                }
            }
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
            onClicked:{
                // 右边种类数据重置
                qmlDatatypeModelManage.reset();
                // 左边选中标签重置
                qmlLabelImgData.reset();
            }
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

    Connections {
        target:qmlLabelImgData.getLabelTags()
        function onClearSelectTag(sapType,inherName,firstIndex,secondIndex){
            qmlDatatypeModelManage.clearSelectBtn(sapType,inherName,firstIndex,secondIndex);
        }
    }

    Component.onCompleted: {
        option=[];
        for(var i=0;i<qmlDatatypeModelManage.graintypes.length;i++) {
            let tmp = qmlDatatypeModelManage.graintypes[i].split("-");
            optmodel.append({"text":tmp[1],"value":tmp[0]});
        }
        for(let i=0;i<cbxgraintype.count;i++) {
            // // console.log(qmlDatatypeModelManage.getAllDataModel(cbxgraintype.model[i].value).allDatas)
            MyScript.createTagPaneObjects(qmlDatatypeModelManage.getAllDataModel(optmodel.get(i).value)
                ,qmlDatatypeModelManage.getAllSingleDataModel(optmodel.get(i).value));
        }
        cbxgraintype.currentIndex=0;
    }
}
