/* Author: Jesse Hairston
 * Added: Friday Feb 7 @ 4:25pm
 * Initial Commit: Wednesday Feb 5 @ 4:25pm
 * Use Case: 09: Heads-Up Display
 */

#include <QGraphicsScene>

#include "hudsprite.h"
#include <QDebug>

HUDsprite::HUDsprite(QGraphicsView *&view, QPointF screenPos, QGraphicsItem *parent) :
    Sprite(parent),
    m_screenPos(screenPos),
    m_view(view) {
}

void HUDsprite::step(qint64 time, long delta) {
    Sprite::step(time, delta);
    this->setPos(m_view->mapToScene(m_screenPos.x(), m_screenPos.y()));
}