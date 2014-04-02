#include "optionbutton.h"

OptionButton::OptionButton(QPixmap bkg, QPixmap sliderGraphic, QGraphicsItem *parent) :
    QGraphicsItemGroup(parent) {

    m_state = true;

    m_slider = new QGraphicsPixmapItem(sliderGraphic);
    m_bkg = new QGraphicsPixmapItem(bkg);


    m_slider->setZValue(-999);
    m_slider->setX(m_bkg->boundingRect().width()-m_slider->boundingRect().width()-15);

    m_bkg->setZValue(-1000);

    this->addToGroup(m_slider);
    this->addToGroup(m_bkg);

    this->setHandlesChildEvents(true);
}

void OptionButton::mousePressEvent(QGraphicsSceneMouseEvent *ev) {

}

void OptionButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *ev) {
    if (this->boundingRect().contains(ev->pos())) {
        if(m_state) m_slider->setX(m_bkg->boundingRect().width()-m_slider->boundingRect().width()-15);
        else m_slider->setX(15);

        this->m_state = !m_state;
        this->clicked();
    }
}

void OptionButton::clicked() {
    if (m_clickedCallback) {
        m_clickedCallback();
    }
}

bool OptionButton::optionState() {
    return(m_state);
}

OptionButton::~OptionButton() {
}
