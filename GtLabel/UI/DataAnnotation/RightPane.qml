pragma ComponentBehavior: Bound
import QtQuick
import ui_main 1.0
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

Item {
    id: root
    signal complexBtnClicked(ButtonComplex complexBtn,bool selected,string inheritsName);
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
    Flickable {
        id: cont

        anchors.left: parent.left
        clip: true
        contentHeight: topData.childrenRect.height + bottoData.childrenRect.height + 12
        height: parent.height - y
        rightMargin: 12
        width: root.width + 10
        y: topLeft.childrenRect.height + 12

        ScrollBar.vertical: ScrollBar {
            policy: cont.contentHeight > cont.height ? ScrollBar.AlwaysOn : ScrollBar.AsNeeded
        }

        Item {
            height: topData.childrenRect.height + bottoData.childrenRect.height + 12
            width: parent.width

            Column {
                id: topData

                // 二级结构
                anchors.left: parent.left
                anchors.top: parent.top
                spacing: 12
                width: parent.width

                Repeater {
                    model: dataTypeModel.allDatas

                    Column {
                        id: topNode

                        property bool fold: modelData.title.fold
                        property ButtonComplex forntBtn
                        required property var modelData

                        spacing: 12
                        width: parent.width

                        RowLayout {
                            height: 20
                            width: topData.width

                            Text {
                                Layout.fillHeight: true
                                Layout.fillWidth: true
                                color: "#333333"
                                font.pixelSize: 16
                                text: modelData.title.title
                            }
                            Button {
                                id: tpbtn

                                Layout.fillHeight: true
                                Layout.preferredWidth: 20

                                background: Image {
                                    anchors.centerIn: parent
                                    height: sourceSize.height
                                    layer.enabled: true
                                    rotation: topNode.fold ? 0 : 180
                                    source: "qrc:/images/fold.png"
                                    width: sourceSize.width

                                    layer.effect: MultiEffect {
                                        brightness: 1.0
                                        colorization: 1
                                        colorizationColor: {
                                            if (tpbtn.hovered) {
                                                return "#1C76E0";
                                            } else {
                                                return "#666666";
                                            }
                                        }
                                    }
                                    Behavior on rotation {
                                        NumberAnimation {
                                            duration: 200
                                        }
                                    }
                                }

                                onClicked: {
                                    modelData.fold(!modelData.title.fold);
                                }
                            }
                        }
                        Repeater {
                            id: topTagbtns

                            model: modelData.sortNodes

                            Column {
                                required property int index
                                required property var modelData

                                spacing: 12
                                width: topData.width

                                Flow {
                                    spacing: 12
                                    width: topData.width

                                    Repeater {
                                        model: modelData

                                        ButtonComplex {
                                            id: secBtn
                                            required property int index
                                            required property var modelData
                                            deep: index === 0 ? 1 : 2
                                            font.pixelSize: 14
                                            height: topNode.fold ? 32 : 0
                                            inheritsName: modelData.split(",").map(item => item.trim())[1]
                                            selected: {
                                                if (modelData.split(",").map(item => item.trim())[2] === "true") {
                                                    // topNode.forntBtn = secBtn;
                                                    return true;
                                                } else {
                                                    return false;
                                                }
                                            }
                                            text: modelData.split(",").map(item => item.trim())[0]
                                            visible: topNode.fold
                                            width: topNode.fold ? 88 : 0

                                            onClicked: {
                                                complexBtnClicked(secBtn,!selected,inheritsName);
                                                topNode.modelData.setSelected(parent.parent.index, secBtn.index, !secBtn.selected);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        Canvas {
                            height: 2
                            width: topNode.width

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
                id: bottoData

                // // 一级结构
                anchors.left: parent.left
                anchors.top: topData.bottom
                anchors.topMargin: 12
                spacing: 12
                width: parent.width

                Repeater {
                    model: dataSingleTypeModel.allDatas
                    width: parent.width

                    Flow {
                        property ButtonComplex fornt
                        property int forntSelect: -1
                        required property int index
                        required property var modelData
                        property Repeater repeater: btmTags
                        property bool selected: false

                        spacing: 12
                        width: parent.width

                        Repeater {
                            id: btmTags

                            model: modelData.treeNodes

                            Loader {
                                required property int deep
                                required property int index
                                required property var inheritsName
                                required property var modelData
                                required property bool selected
                                required property string tagName
                                required property bool visiable

                                sourceComponent: {
                                    index === 0 ? foldBtn : tagBtn;
                                }
                            }
                        }
                        Canvas {
                            height: 2
                            width: parent.width

                            onPaint: {
                                if (parent.index === dataSingleTypeModel.allDatas.length - 1)
                                    return;
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
            id: foldBtn

            RowLayout {
                property string tagName: parent.tagName

                height: 20
                width: topData.width

                Text {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    color: "#333333"
                    font.pixelSize: 16
                    text: parent.tagName
                }
                Button {
                    id: btbtn

                    Layout.fillHeight: true
                    Layout.preferredWidth: 20

                    background: Image {
                        id: btimg

                        anchors.centerIn: parent
                        height: sourceSize.height
                        layer.enabled: true
                        source: "qrc:/images/fold.png"
                        width: sourceSize.width

                        layer.effect: MultiEffect {
                            brightness: 1.0
                            colorization: 1
                            colorizationColor: {
                                if (btbtn.hovered) {
                                    return "#1C76E0";
                                } else {
                                    return "#666666";
                                }
                            }
                        }
                        Behavior on rotation {
                            NumberAnimation {
                                duration: 200
                            }
                        }
                    }

                    onClicked: {
                        // qml Repeater下的item 父类不是Repeater，他仅仅代表数据
                        // console.log(parent.parent);
                        parent.parent.parent.selected = !parent.parent.parent.selected;
                        if (parent.parent.parent.selected) {
                            btimg.rotation = 180;
                            parent.parent.parent.modelData.fold(false);
                        } else {
                            parent.parent.parent.modelData.fold(true);
                            btimg.rotation = 0;
                        }
                    }
                }
            }
        }
    }
    Component {
        id: tagBtn
        ButtonComplex {
            id: btncpx
            deep: parent.deep
            font.pixelSize: 14
            height: parent.visiable ? 32 : 0
            inheritsName: {
                let tmp = "";
                for (let i = parent.inheritsName.length - 1; i >= 0; i--) {
                    tmp += parent.inheritsName[i];
                    if (i !== 0)
                        tmp += "-";
                }
                return tmp;
            }
            selected: {
                if (parent.selected) {
                    return true;
                } else {
                    return false;
                }
            }
            text: parent.tagName
            visible: parent.visiable
            width: parent.visiable ? 88 : 0

            onClicked: {
                parent.modelData.qmlSelected(!selected);
            }
        }
    }
    AnnotationIndexModel {
        id: dataTypeModel

    }
    AnnotationSingleIndexModel {
        id: dataSingleTypeModel

    }
}
