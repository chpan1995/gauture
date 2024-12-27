#include "FrameLessView.h"

FrameLessView::FrameLessView(QWindow *parent):QQuickView(parent)
{
    setFlags(Qt::CustomizeWindowHint | Qt::Window | Qt::FramelessWindowHint |
             Qt::WindowMinMaxButtonsHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
    setResizeMode(SizeRootObjectToView);
}

void FrameLessView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_resizing = false;
        m_windowStartGeometry = geometry();
        // 判断是否在窗口边缘
        if (isInEdge(event->pos()).has_value()) {
            m_dragStartPos = event->globalPosition().toPoint();
            m_resizing = true;
            m_resizeEdge=isInEdge(event->pos()).value();
        }
    }
}

void FrameLessView::mouseMoveEvent(QMouseEvent *event)
{
    setCursor(Qt::ArrowCursor);
    // 判断是否在窗口边缘
    if (isInEdge(event->pos()).has_value()) {
        switch (isInEdge(event->pos()).value()) {
        case Qt::TopEdge:
        case Qt::BottomEdge:
            setCursor(Qt::SizeVerCursor);
            break;
        case Qt::LeftEdge:
        case Qt::RightEdge:
            setCursor(Qt::SizeHorCursor);
            break;
        default:
            break;
        }
    }
    QPoint delta = event->globalPosition().toPoint() - m_dragStartPos;
    if (m_resizing) {
        QRect newGeometry = m_windowStartGeometry;

        // 根据拖拽方向调整窗口大小
        if (m_resizeEdge & Qt::TopEdge) {
            newGeometry.setTop(newGeometry.top() + delta.y());
        }
        if (m_resizeEdge & Qt::BottomEdge) {
            newGeometry.setBottom(newGeometry.bottom() + delta.y());
        }
        if (m_resizeEdge & Qt::LeftEdge) {
            newGeometry.setLeft(newGeometry.left() + delta.x());
        }
        if (m_resizeEdge & Qt::RightEdge) {
            newGeometry.setRight(newGeometry.right() + delta.x());
        }

        setGeometry(newGeometry);
    }
}

void FrameLessView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_resizing = false;
    }
}

std::optional<Qt::Edge> FrameLessView::isInEdge(const QPoint &pos) const
{
    const int edgeThickness = 10; // 边缘宽度
    QRect rects[4]  = {QRect(0, 0, width(), edgeThickness),                       // Top
                      QRect(0, height() - edgeThickness, width(), edgeThickness), // Bottom
                      QRect(0, 0, edgeThickness, height()),                       // Left
                      QRect(width() - edgeThickness, 0, edgeThickness, height())};
    Qt::Edge edg[4]={ Qt::TopEdge,Qt::BottomEdge,Qt::LeftEdge,Qt::RightEdge };
    std::pair<QRect*, Qt::Edge*> edges =std::make_pair<QRect*, Qt::Edge*>(rects,edg);


    for(int i=0;i<4;i++) {
        if(edges.first[i].contains(pos)){
            return edges.second[i];
        }
    }
    return {};
}
