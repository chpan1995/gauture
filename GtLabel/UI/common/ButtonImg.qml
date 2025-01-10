pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import QtQuick.Effects

Button {
    id:btn
    required property string url
    property string hovercolor: "transparent"
    property string normalcolor: "transparent"

    property string imagecolor: "#666666"
    property string imagehovercolor: "#1C76E0"

    property int radius:0
    hoverEnabled : true
    background: Rectangle {
        Image {
            id:img
            anchors.centerIn:parent
            layer.enabled: true
            smooth: true
            source: btn.url
            width:sourceSize.width
            height:sourceSize.height
            layer.effect:MultiEffect {
                colorization:1
                brightness: 1.0
                colorizationColor: {
                    if(btn.hovered){
                        return btn.imagehovercolor;
                    }else {
                        return btn.imagecolor;
                    }
                }
            }
        }
        color:{
            if(btn.hovered){
                return btn.hovercolor;
            }else {
                return btn.normalcolor;
            }
        }
        radius:btn.radius
    }
}
