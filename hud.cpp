#include "hud.h"

HUD::HUD(QGraphicsView * const&view, QPointF screenPos, QGraphicsItem *parent) :
    QGraphicsItemGroup(parent),
    m_screenPos(screenPos),
    m_view(view) {
}

void HUD::step(qint64 time, long delta) {
    if (m_view)
        this->setPos(m_view->mapToScene(m_screenPos.x(), m_screenPos.y()));
}

QRectF HUD::boundingRect() const {
    return QRectF(0, 0, 0, 0);
}

QPainterPath HUD::shape() const {
    return QPainterPath();
}
