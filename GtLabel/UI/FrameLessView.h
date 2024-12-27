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
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    std::optional<Qt::Edge> isInEdge(const QPoint &pos) const;
private:
    bool m_resizing {false};
    QRect m_windowStartGeometry;
    Qt::Edge m_resizeEdge;
    QPoint m_dragStartPos;
};

#endif // FRAMELESSVIEW_H
