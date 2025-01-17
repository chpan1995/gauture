#ifndef FRAMELESSVIEW_H
#define FRAMELESSVIEW_H

#include <QObject>
#include <QQuickView>
#include <optional>
#include <QQuickItem>

#if defined(Q_OS_UNIX) && !defined(Q_OS_MAC)
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <xcb/xcb.h>
#endif

class FrameLessView : public QQuickView
{
    Q_OBJECT
    Q_PROPERTY(QVariant isShowMax READ isShowMax  WRITE setIsShowMax NOTIFY isShowMaxChanged)
public:
    explicit FrameLessView(QWindow *parent=nullptr);
    enum Edge
    {
        unknow= 0x00,
        TopEdge = 0x01,
        BottomEdge = TopEdge << 1,
        LeftEdge = TopEdge << 2,
        RightEdge = TopEdge << 3,
        LeftTopEdge = TopEdge << 4,
        RightTopEdge = TopEdge << 5,
        LeftBottomEdge = TopEdge << 6,
        RightBottomEdge = TopEdge << 7,
    };
    Q_DECLARE_FLAGS(Edges,Edge)

    Q_INVOKABLE inline QPoint moveStart() { return geometry().topLeft(); }
    Q_INVOKABLE void moveing(QPoint start,QPoint end);
    Q_INVOKABLE inline void moveEnd() {
        m_resizing = false;
        m_resizeEdge=unknow;
    }
    // Q_INVOKABLE void showMin();
    Q_INVOKABLE void showNor();
    Q_INVOKABLE void showMax();

    QVariant isShowMax() const { return m_isShowMax; }
    void setIsShowMax(QVariant value) {
        if (m_isShowMax != value) {
            m_isShowMax = value;
            emit isShowMaxChanged();
        }
    }
signals:
    void isShowMaxChanged();
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    // void mouseReleaseEvent(QMouseEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;
private:
    std::optional<Edge> isInEdge(const QPoint &pos) const;
private:
    bool m_resizing {false};
    QRect m_windowStartGeometry;
    Edge m_resizeEdge;
    QPoint m_dragStartPos;
    bool m_borderSelecte { true };
    QVariant m_isShowMax {true};
    QRect m_normalGeometry {0,0,1294,800};  // 存储普通状态下的窗口大小和位置
    Qt::WindowState m_oldState {Qt::WindowState::WindowMaximized};
};
Q_DECLARE_OPERATORS_FOR_FLAGS(FrameLessView::Edges)
#endif // FRAMELESSVIEW_H
