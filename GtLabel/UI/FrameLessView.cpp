#include "FrameLessView.h"
#include <qdebug.h>

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
        case Edge::TopEdge:
        case Edge::BottomEdge:
            setCursor(Qt::SizeVerCursor);
            break;
        case Edge::LeftEdge:
        case Edge::RightEdge:
            setCursor(Qt::SizeHorCursor);
            break;
        case Edge::LeftTopEdge:
        case Edge::RightBottomEdge:
            setCursor(Qt::SizeFDiagCursor);
            break;
        case Edge::RightTopEdge:
        case Edge::LeftBottomEdge:
            setCursor(Qt::SizeBDiagCursor);
            break;
        default:
            break;
        }
    }
    QPoint delta = event->globalPosition().toPoint() - m_dragStartPos;
    if (m_resizing) {
        QRect newGeometry = m_windowStartGeometry;
        // 根据拖拽方向调整窗口大小
        if (m_resizeEdge & Edge::TopEdge) {
            newGeometry.setTop(newGeometry.top() + delta.y());
        }
        else if (m_resizeEdge & Edge::BottomEdge) {
            newGeometry.setBottom(newGeometry.bottom() + delta.y());
        }
        else if (m_resizeEdge & Edge::LeftEdge) {
            newGeometry.setLeft(newGeometry.left() + delta.x());
        }
        else if (m_resizeEdge & Edge::RightEdge) {
            newGeometry.setRight(newGeometry.right() + delta.x());
        }


        else if(m_resizeEdge & Edge::LeftTopEdge) {
            newGeometry.setLeft(newGeometry.left() + delta.x());
            newGeometry.setTop(newGeometry.top() + delta.y());
        }else if(m_resizeEdge & Edge::RightBottomEdge) {
            newGeometry.setRight(newGeometry.right() + delta.x());
            newGeometry.setBottom(newGeometry.bottom() + delta.y());
        }else if(m_resizeEdge & Edge::RightTopEdge) {
            newGeometry.setRight(newGeometry.right() + delta.x());
            newGeometry.setTop(newGeometry.top() + delta.y());
        }else if(m_resizeEdge & Edge::LeftBottomEdge) {
            newGeometry.setLeft(newGeometry.left() + delta.x());
            newGeometry.setBottom(newGeometry.bottom() + delta.y());
        }


        setGeometry(newGeometry);
    }
}

void FrameLessView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_resizing = false;
        m_resizeEdge=unknow;
    }
}

std::optional<FrameLessView::Edge> FrameLessView::isInEdge(const QPoint &pos) const
{
    const int edgeThickness = 10; // 边缘宽度
    const int edgeroud = 10; // 周围
    QRect rects[8]  = {QRect(0+edgeroud, 0, width()-2*edgeroud, edgeThickness),                       // Top
                      QRect(0+edgeroud, height() - edgeThickness, width()-2*edgeroud, edgeThickness), // Bottom
                      QRect(0, 0+edgeroud, edgeThickness, height()-2*edgeroud),                       // Left
                      QRect(width() - edgeThickness, 0+edgeroud, edgeThickness, height()-2*edgeroud), // Right
                      QRect(0, 0, edgeroud, edgeThickness), // TopLEft
                      QRect(width()-edgeroud, 0, edgeroud, edgeThickness), // TopRight
                      QRect(0, height()-edgeroud, edgeroud, edgeThickness), // LEftBottom
                      QRect(width() - edgeroud, height()-edgeroud, edgeroud, edgeThickness) // rightbottom
    };
    Edge edg[8]={ Edge::TopEdge,Edge::BottomEdge,Edge::LeftEdge,Edge::RightEdge,
                Edge::LeftTopEdge,Edge::RightTopEdge,Edge::LeftBottomEdge,Edge::RightBottomEdge };
    std::pair<QRect*, Edge*> edges =std::make_pair<QRect*, Edge*>(rects,edg);


    for(int i=0;i<8;i++) {
        if(edges.first[i].contains(pos)){
            return edges.second[i];
        }
    }
    return {};
}
