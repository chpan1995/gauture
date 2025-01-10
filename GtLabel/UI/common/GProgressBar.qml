import QtQuick
import QtQuick.Controls.Basic

ProgressBar {
    id: control

    background: Rectangle {
        implicitWidth: 200
        implicitHeight: 6
        color: "#ECECEC"
        radius: 1
    }

    contentItem: Item {
        implicitWidth: 200
        implicitHeight: 4

        // Progress indicator for determinate state.
        Rectangle {
            width: control.visualPosition * parent.width
            height: parent.height
            radius: 1
            color: "#1C76E0"
            visible: !control.indeterminate
            Behavior on width {
                NumberAnimation {
                    duration:50
                }
            }
        }
    }

    Timer {
        id:tm
        repeat:false
        interval:50
        running:false
        onTriggered:{
            control.value=0;
        }
    }

    onValueChanged:{
        if(value==1){
            tm.start();
        }
    }
}
