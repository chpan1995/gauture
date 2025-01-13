pragma ComponentBehavior: Bound
import QtQuick
import ui_main 1.0
import QtQuick.Layouts
import QtQuick.Effects
import QtQuick.Controls

Popup {
    id:taskinfo
    required property var taskmodel;
    background:Rectangle{
        color:"#FFFFFF"
        radius:8
    }
    contentItem:ListView {
        model:taskinfo.taskmodel
        spacing:16
        delegate:Text{
            required property var taskInfoName
            required property var taskInfoCreateTime
            required property var taskInfoCount
            required property var taskInfoLabCount
            required property var taskContinueCount
            width:taskinfo.width
            height:20
            text:taskInfoName
            color:"red"
        }
    }
}

