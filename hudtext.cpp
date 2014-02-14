#include "hudtext.h"

HUDText::HUDText(QGraphicsView *&view, QPointF screenPos, QGraphicsItem *parent) :
    QGraphicsSimpleTextItem(parent),
    m_screenPos(screenPos),
    m_view(view) {}

void HUDText::step(qint64 time, long delta) {
    this->setPos(m_view->mapToScene(m_screenPos.x(), m_screenPos.y()));
}
