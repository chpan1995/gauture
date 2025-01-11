#include "FrameLessView.h"

#include <QTimer>
#include <QCursor>

#ifdef Q_OS_WIN
#include <windows.h>
#include <dwmapi.h>
#endif

FrameLessView::FrameLessView(QWindow *parent) : QQuickView(parent)
{
    setFlags(Qt::CustomizeWindowHint | Qt::Window | Qt::FramelessWindowHint |
             Qt::WindowMinMaxButtonsHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
    setResizeMode(SizeRootObjectToView);
    setColor(Qt::transparent); // 背景透明
    connect(this,&FrameLessView::windowStateChanged,this,[this](Qt::WindowState windowState){
        if(windowState==Qt::WindowNoState && m_ShowMax) {
            showMaximized();
        }
    });
    qApp->installEventFilter(this);
}

void FrameLessView::moveing(QPoint start, QPoint end)
{
#if defined(Q_OS_UNIX) && !defined(Q_OS_MAC)
    QNativeInterface::QX11Application *x11App = qApp->nativeInterface<QNativeInterface::QX11Application>();
    Display *dpy = x11App->display();
    QQuickItem *rootobj = rootObject();
    if (rootobj)
    {
        QObject *obj = rootobj->findChild<QObject *>("titleBar");
        QQuickItem *titleBarItem = qobject_cast<QQuickItem *>(obj);
        // if(obj && titleBarItem) {
        // 检查鼠标是否在 titleBar 区域内
        QPoint mousePos = QCursor::pos();
        QPoint titleBarGlobalPos = titleBarItem->mapToGlobal(QPointF(0, 0)).toPoint();
        QRect titleBarRect(titleBarGlobalPos, QSize(titleBarItem->width(), titleBarItem->height()));

        if (!titleBarRect.contains(mousePos))
        {
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
        XSendEvent(dpy, DefaultRootWindow(dpy), False,
                   SubstructureNotifyMask | SubstructureRedirectMask, &even);
        XFlush(dpy);
    }
#else
    QPoint topLeft = end - start;
    setGeometry({topLeft.x(), topLeft.y(), width(), height()});
#endif
}

void FrameLessView::showNor() {
    m_ShowMax=false;
    showNormal();
}

void FrameLessView::showMax() {
    m_ShowMax=true;
    showMaximized();
}

void FrameLessView::mousePressEvent(QMouseEvent *event)
{
    event->ignore();
    if (event->button() == Qt::LeftButton)
    {
        m_resizing = false;
        m_windowStartGeometry = geometry();
        // 判断是否在窗口边缘
        if (isInEdge(event->pos()).has_value())
        {
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
    if (isInEdge(event->pos()).has_value())
    {
        switch (isInEdge(event->pos()).value())
        {
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
    if (m_resizing)
    {
        // qDebug() << "mouseMoveEvent"<<QDateTime::currentDateTime();
        QRect newGeometry = m_windowStartGeometry;
        // 根据拖拽方向调整窗口大小
        if (m_resizeEdge & Edge::TopEdge)
        {
            newGeometry.setTop(newGeometry.top() + delta.y());
        }
        else if (m_resizeEdge & Edge::BottomEdge)
        {
            newGeometry.setBottom(newGeometry.bottom() + delta.y());
        }
        else if (m_resizeEdge & Edge::LeftEdge)
        {
            newGeometry.setLeft(newGeometry.left() + delta.x());
        }
        else if (m_resizeEdge & Edge::RightEdge)
        {
            newGeometry.setRight(newGeometry.right() + delta.x());
        }

        else if (m_resizeEdge & Edge::LeftTopEdge)
        {
            newGeometry.setLeft(newGeometry.left() + delta.x());
            newGeometry.setTop(newGeometry.top() + delta.y());
        }
        else if (m_resizeEdge & Edge::RightBottomEdge)
        {
            newGeometry.setRight(newGeometry.right() + delta.x());
            newGeometry.setBottom(newGeometry.bottom() + delta.y());
        }
        else if (m_resizeEdge & Edge::RightTopEdge)
        {
            newGeometry.setRight(newGeometry.right() + delta.x());
            newGeometry.setTop(newGeometry.top() + delta.y());
        }
        else if (m_resizeEdge & Edge::LeftBottomEdge)
        {
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
    if (m_resizing)
    {
        if (event->type() == QEvent::MouseMove)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            mouseMoveEvent(mouseEvent);
            return true;
        }
        else if (event->type() == QEvent::MouseButtonRelease)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            mouseEvent->ignore();
            if (mouseEvent->button() == Qt::LeftButton)
            {
                m_resizing = false;
                m_resizeEdge = unknow;
                return true;
            }
        }
    }
#ifdef Q_OS_WIN
    if (event->type() == QEvent::WinIdChange) {
        initializeWindow();
        return false;
    }

    if (event->type() == QEvent::Show) {
        initializeWindow();
        return false;
    }
#endif
    return QQuickView::eventFilter(watched, event);
}

void FrameLessView::initializeWindow() {
#ifdef Q_OS_WIN
    // 获取窗口句柄
    HWND hwnd = (HWND)winId();

    // 启用 Snap Layout
    BOOL enable = TRUE;
    DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &enable, sizeof(enable));

    // 设置圆角和 Mica 效果
    DWM_SYSTEMBACKDROP_TYPE backdropType = DWMSBT_MAINWINDOW;
    DwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE, &backdropType, sizeof(backdropType));

    // 安装原生事件过滤器
    installEventFilter(this);
#endif
}

std::optional<FrameLessView::Edge> FrameLessView::isInEdge(const QPoint &pos) const
{
    const int edgeThickness = 6; // 边缘宽度
    const int edgeroud = 6;      // 周围
    QRect rects[8] = {
        QRect(0 + edgeroud, 0, width() - 2 * edgeroud, edgeThickness),                        // Top
        QRect(0 + edgeroud, height() - edgeThickness, width() - 2 * edgeroud, edgeThickness), // Bottom
        QRect(0, 0 + edgeroud, edgeThickness, height() - 2 * edgeroud),                       // Left
        QRect(width() - edgeThickness, 0 + edgeroud, edgeThickness, height() - 2 * edgeroud), // Right
        QRect(0, 0, edgeroud, edgeThickness),                                                 // TopLEft
        QRect(width() - edgeroud, 0, edgeroud, edgeThickness),                                // TopRight
        QRect(0, height() - edgeroud, edgeroud, edgeThickness),                               // LEftBottom
        QRect(width() - edgeroud, height() - edgeroud, edgeroud, edgeThickness)               // rightbottom
    };
    Edge edg[8] = {Edge::TopEdge, Edge::BottomEdge, Edge::LeftEdge, Edge::RightEdge,
                   Edge::LeftTopEdge, Edge::RightTopEdge, Edge::LeftBottomEdge, Edge::RightBottomEdge};
    std::pair<QRect *, Edge *> edges = std::make_pair<QRect *, Edge *>(rects, edg);

    for (int i = 0; i < 8; i++)
    {
        if (edges.first[i].contains(pos))
        {
            return edges.second[i];
        }
    }
    return {};
}
