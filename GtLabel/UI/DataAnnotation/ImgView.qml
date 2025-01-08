import QtQuick
Flickable {
    id: imgview
    contentWidth: width
    contentHeight: height
    clip:true
    property var currentscale: {"scaleX":1,"scaleY":1,"oldWidth":-1,"oldHeight":-1}
    // onWidthChanged : {
    //     currentscale.scaleX=width/640;
    // }
    // onHeightChanged: {
    //     currentscale.scaleY=height/480;
    // }

    Rectangle {
        id: imgname
        width: parent.width
        height: parent.height
        color: "#000000"
        property real currentScale: 1.0
        property real scaleStep: 0.12
        property real mouseX: width/2
        property real mouseY: height/2

        property var drawingPaths: []
        property var currentPath: []

        // 添加属性存储原始图片尺寸
        property real originalImageWidth: 0
        property real originalImageHeight: 0

        // 添加属性存储图片实际显示区域
        property rect displayRect: Qt.rect(0, 0, 0, 0)

        Image {
            id: sourceImage
            width: parent.width
            height: parent.height
            fillMode: Image.PreserveAspectFit
            source: "http://www.baidu.com/img/PCtm_d9c8750bed0b3c7d089fa7d55720d6cf.png"

            // 添加所有可能的尺寸变化监听
            onWidthChanged: updateDisplayRect();
            onHeightChanged: updateDisplayRect();

            // 图片加载完成后获取原始尺寸
            onStatusChanged: {
                if (status === Image.Ready) {
                    imgname.originalImageWidth = sourceImage.sourceSize.width
                    imgname.originalImageHeight = sourceImage.sourceSize.height
                    updateDisplayRect()
                }
            }


            // 计算图片实际显示区域
            function updateDisplayRect() {
                let imageAspect = imgname.originalImageWidth / imgname.originalImageHeight
                let containerAspect = width / height


                let tmpwidth=width/imgname.currentScale;
                let tmpheigt=height/imgname.currentScale;

                if (imageAspect > containerAspect) {
                    // 图片比容器更宽，上下有空白
                    let displayHeight = tmpwidth / imageAspect;
                    imgname.displayRect = Qt.rect(0, (tmpheigt - displayHeight) / 2, tmpwidth, displayHeight)
                } else {
                    // 图片比容器更高，左右有空白
                    let displayWidth = tmpheigt * imageAspect
                    imgname.displayRect = Qt.rect((tmpwidth - displayWidth) / 2, 0, displayWidth, tmpheigt)
                }
            }

            // 转换显示坐标到原始图片坐标
            function displayToImageCoordinates(displayX, displayY) {
                console.log(imgname.displayRect);
                // 计算相对于显示区域的坐标
                let relativeX = (displayX - imgname.displayRect.x) / imgname.displayRect.width
                let relativeY = (displayY - imgname.displayRect.y) / imgname.displayRect.height

                // 转换为原始图片坐标
                return {
                    x: Math.round(relativeX * imgname.originalImageWidth),
                    y: Math.round(relativeY * imgname.originalImageHeight)
                }
            }

            MouseArea {
                id: mouseArea
                anchors.fill: parent
                hoverEnabled: true

                acceptedButtons: /*Qt.LeftButton | Qt.RightButton*/ Qt.NoButton
                onPositionChanged: {
                    imgname.mouseX = mouseX / imgname.currentScale
                    imgname.mouseY = mouseY / imgname.currentScale
                    if(imgname.mouseX < imgname.displayRect.x) imgname.mouseX=imgname.displayRect.x;
                    if(imgname.mouseX > imgname.displayRect.x+imgname.displayRect.width)
                        imgname.mouseX=imgname.displayRect.x+imgname.displayRect.width;
                    if(imgname.mouseY < imgname.displayRect.y) imgname.mouseY= imgname.displayRect.y;
                    if(imgname.mouseY > imgname.displayRect.y+imgname.displayRect.height)
                        imgname.mouseY= imgname.displayRect.y+imgname.displayRect.height;
                    if(canvas.isDrawing) {
                        // 获取原始图片坐标
                        let imageCoords = sourceImage.displayToImageCoordinates(
                            imgname.mouseX,
                            imgname.mouseY
                        )

                        // 记录显示坐标和原始图片坐标
                        imgname.currentPath.push({
                            displayX: imgname.mouseX,
                            displayY: imgname.mouseY,
                            imageX: imageCoords.x,
                            imageY: imageCoords.y,

                            displayXRatio: imgname.mouseX/imgview.contentWidth,
                            displayYRatio: imgname.mouseY/imgview.contentHeight
                        });
                        canvas.requestPaint();

                    }
                }
                onPressed:(mouse)=> {
                    if(mouse.button === Qt.LeftButton) {
                        canvas.isDrawing = true;
                        imgname.mouseX = mouseX / imgname.currentScale
                        imgname.mouseY = mouseY / imgname.currentScale
                        if(imgname.mouseX < imgname.displayRect.x) imgname.mouseX=imgname.displayRect.x;
                        if(imgname.mouseX > imgname.displayRect.x+imgname.displayRect.width)
                            imgname.mouseX=imgname.displayRect.x+imgname.displayRect.width;
                        if(imgname.mouseY < imgname.displayRect.y) imgname.mouseY= imgname.displayRect.y;
                        if(imgname.mouseY > imgname.displayRect.y+imgname.displayRect.height)
                            imgname.mouseY= imgname.displayRect.y+imgname.displayRect.height;
                        // 获取起点的原始图片坐标
                        let imageCoords = sourceImage.displayToImageCoordinates(
                            imgname.mouseX,
                            imgname.mouseY
                        )

                        // 记录起点的显示坐标和原始图片坐标
                        imgname.currentPath = [{
                            displayX: imgname.mouseX,
                            displayY: imgname.mouseY,
                            imageX: imageCoords.x,
                            imageY: imageCoords.y,

                            displayXRatio: imgname.mouseX/imgview.contentWidth,
                            displayYRatio: imgname.mouseY/imgview.contentHeight
                        }];
                    } else {
                        imgview.interactive = false
                    }
                }
                onReleased: {
                    if(canvas.isDrawing) {
                        imgname.drawingPaths.push(imgname.currentPath);
                        canvas.isDrawing = false;

                        // 输出完整路径的原始图片坐标
                        for(let point of imgname.currentPath) {
                            // if(point && point.imageX !== undefined && point.imageY !== undefined)
                            // console.log(`Point: (${point.imageX}, ${point.imageY})`);
                        }
                    }
                }
            }

            Canvas {
                id: canvas
                anchors.fill: parent
                property bool isDrawing: false

                onPaint: {
                    var ctx = getContext("2d");
                    ctx.reset();
                    ctx.lineWidth = 2;
                    ctx.strokeStyle = "blue";
                    ctx.scale(imgname.currentScale, imgname.currentScale);

                    // 绘制所有已保存的路径
                    for(var i = 0; i < imgname.drawingPaths.length; i++) {
                        var path = imgname.drawingPaths[i];
                        if(path.length > 1) {
                            ctx.beginPath();
                            ctx.moveTo(path[0].displayX, path[0].displayY);
                            for(var j = 1; j < path.length; j++) {
                                ctx.lineTo(path[j].displayX, path[j].displayY);
                            }
                            ctx.stroke();
                        }
                    }
                    // for(var i = 0; i < imgname.drawingPaths.length; i++) {
                    //     var path = imgname.drawingPaths[i];
                    //     if(path.length > 1) {
                    //         ctx.beginPath();
                    //         ctx.moveTo(path[0].displayXRatio*imgview.contentWidth, path[0].displayYRatio*imgview.contentHeight);
                    //         for(var j = 1; j < path.length; j++) {
                    //             ctx.lineTo(path[j].displayXRatio*imgview.contentWidth, path[j].displayYRatio*imgview.contentHeight);
                    //         }
                    //         ctx.stroke();
                    //     }
                    // }

                    // 绘制当前正在绘制的路径
                    if(canvas.isDrawing && imgname.currentPath.length > 1) {
                        ctx.beginPath();
                        ctx.moveTo(imgname.currentPath[0].displayX, imgname.currentPath[0].displayY);
                        for(var k = 1; k < imgname.currentPath.length; k++) {
                            ctx.lineTo(imgname.currentPath[k].displayX, imgname.currentPath[k].displayY);
                        }
                        ctx.stroke();
                    }
                }
            }

            WheelHandler {
                target: imgname
                onWheel: (event)=> {
                    let mouseViewX = imgname.mouseX
                    let mouseViewY = imgname.mouseY
                    let mouseContentX = mouseViewX + imgview.contentX
                    let mouseContentY = mouseViewY + imgview.contentY
                    let ratioX = mouseContentX / (imgview.contentWidth)
                    let ratioY = mouseContentY / (imgview.contentHeight)

                    if (event.angleDelta.y > 0) {
                        imgname.currentScale += imgname.scaleStep
                    } else if (event.angleDelta.y < 0 && imgname.currentScale > 0.1) {
                        imgname.currentScale = Math.max(0.1, imgname.currentScale - imgname.scaleStep)
                    }

                    let newContentWidth = Math.max(imgview.width * imgname.currentScale, imgview.width)
                    let newContentHeight = Math.max(imgview.height * imgname.currentScale,imgview.height)

                    imgname.width = imgview.width * imgname.currentScale
                    imgname.height = imgview.height * imgname.currentScale

                    if (imgname.currentScale >= 1) {
                        let newMouseContentX = newContentWidth * ratioX
                        let newMouseContentY = newContentHeight * ratioY
                        imgview.contentX = newMouseContentX - mouseViewX
                        imgview.contentY = newMouseContentY - mouseViewY
                        imgname.x = 0
                        imgname.y = 0
                    } else {
                        imgname.x = (imgview.width - imgname.width) / 2
                        imgname.y = (imgview.height - imgname.height) / 2
                        imgview.contentX = 0
                        imgview.contentY = 0
                    }

                    imgview.contentWidth = newContentWidth
                    imgview.contentHeight = newContentHeight

                    // 更新显示区域并重绘
                    sourceImage.updateDisplayRect()
                    canvas.requestPaint()
                }
            }
        }
    }
}
