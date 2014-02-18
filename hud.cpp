#include <QGraphicsScene>

#include "hud.h"

HUD::HUD(QGraphicsView *&view, QPointF screenPos, QGraphicsItem *parent) :
    QGraphicsItem(parent),
    m_screenPos(screenPos),
    m_view(view) {
}

void HUD::step(qint64 time, long delta) {
    if (m_view)
        this->setPos(m_view->mapToScene(m_screenPos.x(), m_screenPos.y()));
}
