﻿#include "FrameLessView.h"

#include <QCursor>
#include <QTimer>
#include <QScreen>

#ifdef Q_OS_WIN
#include <dwmapi.h>
#include <windows.h>
#endif

#if defined(Q_OS_UNIX) && !defined(Q_OS_MAC)
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <xcb/xcb.h>
#endif

FrameLessView::FrameLessView(QWindow *parent)
    : QQuickView(parent)
{
    setFlags(Qt::CustomizeWindowHint | Qt::Window | Qt::FramelessWindowHint
             | Qt::WindowMinMaxButtonsHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
    setResizeMode(SizeRootObjectToView);
    setColor(Qt::transparent); // 背景透明
    connect(this, &FrameLessView::windowStateChanged, this, [this](Qt::WindowState windowState) {
        if(windowState==Qt::WindowMaximized) {
            if(!m_isShowMax.toBool()) {
                setIsShowMax(true);
            }
        }
        // 当窗口即将最小化时
        if (windowState & Qt::WindowMinimized) {
            // 记录最小化前的状态
            bool flag = m_oldState & Qt::WindowMaximized;
            setIsShowMax(flag);
            // 保存当前的普通状态下的几何信息
            if (!(m_oldState & Qt::WindowMaximized)) {
                m_normalGeometry = geometry();
            }
        }
        // 当窗口从最小化恢复时
        else if (m_oldState & Qt::WindowMinimized) {
            // 如果之前是最大化状态，则恢复到最大化
            if (m_isShowMax.toBool()) {
                QTimer::singleShot(0, this, [this]() {
                    setWindowState(Qt::WindowMaximized);
                });
            }
            // 否则恢复到之前保存的普通状态
            else {
                QTimer::singleShot(0, this, [this]() {
                    setGeometry(m_normalGeometry);
                    setWindowState(Qt::WindowNoState);
                });
            }
        }
        m_oldState = windowState;
    });
    qApp->installEventFilter(this);

    m_normalGeometry = QRect({(screen()->availableGeometry().width()-1294)/2
                              ,(screen()->availableGeometry().height()-800)/2,1294,800});
}

void FrameLessView::moveing(QPoint start, QPoint end)
{
    if (m_isShowMax.toBool())
        return;
#if defined(Q_OS_UNIX) && !defined(Q_OS_MAC)
    QNativeInterface::QX11Application *x11App
        = qApp->nativeInterface<QNativeInterface::QX11Application>();
    Display *dpy = x11App->display();
    QQuickItem *rootobj = rootObject();
    if (rootobj) {
        QObject *obj = rootobj->findChild<QObject *>("titleBar");
        QQuickItem *titleBarItem = qobject_cast<QQuickItem *>(obj);
        // if(obj && titleBarItem) {
        // 检查鼠标是否在 titleBar 区域内
        QPoint mousePos = QCursor::pos();
        QPoint titleBarGlobalPos = titleBarItem->mapToGlobal(QPointF(0, 0)).toPoint();
        QRect titleBarRect(titleBarGlobalPos, QSize(titleBarItem->width(), titleBarItem->height()));

        if (!titleBarRect.contains(mousePos)) {
            return; // 如果不在 titleBar 区域内，不处理移动
        }
        XEvent even = {};
        even.xclient.type = ClientMessage;
        even.xclient.display = dpy;
        even.xclient.message_type = XInternAtom(dpy, "_NET_WM_MOVERESIZE", False);
        even.xclient.window = qobject_cast<QQuickItem *>(obj)->window()->winId();
        even.xclient.format = 32;
        even.xclient.data.l[0] = QCursor::pos().x();
        even.xclient.data.l[1] = QCursor::pos().y();
        even.xclient.data.l[2] = 8;
        even.xclient.data.l[3] = Button1;
        even.xclient.data.l[4] = 1;

        XUngrabPointer(dpy, CurrentTime);
        XSendEvent(dpy,
                   DefaultRootWindow(dpy),
                   False,
                   SubstructureNotifyMask | SubstructureRedirectMask,
                   &even);
        XFlush(dpy);
    }
#else
    QPoint topLeft = end - start;
    setGeometry({topLeft.x(), topLeft.y(), width(), height()});
    m_normalGeometry=geometry();
#endif
}

void FrameLessView::showNor()
{
    setIsShowMax(false);
    setWindowState(Qt::WindowNoState);
    setGeometry(m_normalGeometry);
}

void FrameLessView::showMax()
{
    setIsShowMax(true);
    showMaximized();
}

void FrameLessView::mousePressEvent(QMouseEvent *event)
{
    event->ignore();
    if (event->button() == Qt::LeftButton) {
        m_resizing = false;
        m_windowStartGeometry = geometry();
        // 判断是否在窗口边缘
        if (isInEdge(event->pos()).has_value()) {
            m_dragStartPos = event->globalPosition().toPoint();
            m_resizing = true;
            m_resizeEdge = isInEdge(event->pos()).value();
        }
    }
}

void FrameLessView::mouseMoveEvent(QMouseEvent *event)
{
    event->ignore();
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
        // qDebug() << "mouseMoveEvent"<<QDateTime::currentDateTime();
        QRect newGeometry = m_windowStartGeometry;
        // 根据拖拽方向调整窗口大小
        if (m_resizeEdge & Edge::TopEdge) {
            newGeometry.setTop(newGeometry.top() + delta.y());
        } else if (m_resizeEdge & Edge::BottomEdge) {
            newGeometry.setBottom(newGeometry.bottom() + delta.y());
        } else if (m_resizeEdge & Edge::LeftEdge) {
            newGeometry.setLeft(newGeometry.left() + delta.x());
        } else if (m_resizeEdge & Edge::RightEdge) {
            newGeometry.setRight(newGeometry.right() + delta.x());
        }

        else if (m_resizeEdge & Edge::LeftTopEdge) {
            newGeometry.setLeft(newGeometry.left() + delta.x());
            newGeometry.setTop(newGeometry.top() + delta.y());
        } else if (m_resizeEdge & Edge::RightBottomEdge) {
            newGeometry.setRight(newGeometry.right() + delta.x());
            newGeometry.setBottom(newGeometry.bottom() + delta.y());
        } else if (m_resizeEdge & Edge::RightTopEdge) {
            newGeometry.setRight(newGeometry.right() + delta.x());
            newGeometry.setTop(newGeometry.top() + delta.y());
        } else if (m_resizeEdge & Edge::LeftBottomEdge) {
            newGeometry.setLeft(newGeometry.left() + delta.x());
            newGeometry.setBottom(newGeometry.bottom() + delta.y());
        }
        if (newGeometry.width() <= 890 || newGeometry.height() <= 600)
            return;
        setGeometry(newGeometry);
    }
}

// void FrameLessView::mouseReleaseEvent(QMouseEvent *event)
// {
//     event->ignore();
//     if (event->button() == Qt::LeftButton) {
//         m_resizing = false;
//         m_resizeEdge=unknow;
//     }
// }

bool FrameLessView::eventFilter(QObject *watched, QEvent *event)
{
    if (m_resizing) {
        if (event->type() == QEvent::MouseMove) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            mouseMoveEvent(mouseEvent);
            return true;
        } else if (event->type() == QEvent::MouseButtonRelease) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            mouseEvent->ignore();
            if (mouseEvent->button() == Qt::LeftButton) {
                m_resizing = false;
                m_resizeEdge = unknow;
                return true;
            }
        }
    }
    return QQuickView::eventFilter(watched, event);
}

std::optional<FrameLessView::Edge> FrameLessView::isInEdge(const QPoint &pos) const
{
    const int edgeThickness = 6; // 边缘宽度
    const int edgeroud = 6;      // 周围
    QRect rects[8] = {
        QRect(0 + edgeroud, 0, width() - 2 * edgeroud, edgeThickness),                        // Top
        QRect(0 + edgeroud, height() - edgeThickness, width() - 2 * edgeroud, edgeThickness), // Bottom
        QRect(0, 0 + edgeroud, edgeThickness, height() - 2 * edgeroud), // Left
        QRect(width() - edgeThickness, 0 + edgeroud, edgeThickness, height() - 2 * edgeroud), // Right
        QRect(0, 0, edgeroud, edgeThickness),                                   // TopLEft
        QRect(width() - edgeroud, 0, edgeroud, edgeThickness),                  // TopRight
        QRect(0, height() - edgeroud, edgeroud, edgeThickness),                 // LEftBottom
        QRect(width() - edgeroud, height() - edgeroud, edgeroud, edgeThickness) // rightbottom
    };
    Edge edg[8] = {Edge::TopEdge,
                   Edge::BottomEdge,
                   Edge::LeftEdge,
                   Edge::RightEdge,
                   Edge::LeftTopEdge,
                   Edge::RightTopEdge,
                   Edge::LeftBottomEdge,
                   Edge::RightBottomEdge};
    std::pair<QRect *, Edge *> edges = std::make_pair<QRect *, Edge *>(rects, edg);

    for (int i = 0; i < 8; i++) {
        if (edges.first[i].contains(pos)) {
            return edges.second[i];
        }
    }
    return {};
}
