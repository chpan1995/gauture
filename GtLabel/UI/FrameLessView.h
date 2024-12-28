#ifndef FRAMELESSVIEW_H
#define FRAMELESSVIEW_H

#include <QObject>
#include <QQuickView>
#include <optional>

class FrameLessView : public QQuickView
{
    Q_OBJECT
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
    Q_ENUM(Edge);
    Q_DECLARE_FLAGS(Edges,Edge)
    Q_FLAGS(Edges);
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    std::optional<Edge> isInEdge(const QPoint &pos) const;
private:
    bool m_resizing {false};
    QRect m_windowStartGeometry;
    Edge m_resizeEdge;
    QPoint m_dragStartPos;
};

#endif // FRAMELESSVIEW_H
