#include "optionslider.h"
#include <QDebug>

OptionSlider::OptionSlider(QPixmap bkg, QPixmap sliderGraphic, QGraphicsItem *parent) :
    QGraphicsItemGroup(parent) {

    m_slider = new QGraphicsPixmapItem(sliderGraphic);
    m_bkg = new QGraphicsPixmapItem(bkg);


    m_slider->setZValue(-999);
    m_slider->setX(m_bkg->boundingRect().width()-m_slider->boundingRect().width()-15);

    m_bkg->setZValue(-1000);

    m_txt = new QGraphicsSimpleTextItem("100");
    m_txt->setScale(0.70);
    m_txt->setX(m_slider->pos().x()+5);
    m_txt->setY(20);
    m_txt->setZValue(-998);

    this->addToGroup(m_slider);
    this->addToGroup(m_bkg);
    this->addToGroup(m_txt);

    this->setHandlesChildEvents(true);
}

void OptionSlider::mousePressEvent(QGraphicsSceneMouseEvent *ev) {
    setSliderPos(ev);
}

void OptionSlider::mouseMoveEvent(QGraphicsSceneMouseEvent *ev) {
    setSliderPos(ev);
}

void OptionSlider::setSliderPos(QGraphicsSceneMouseEvent *ev) {
    if(ev->pos().x() < 35) {
        m_slider->setX(15);
        m_txt->setX(m_slider->pos().x()+10);
        m_txt->setText("0");
    }
    else if (ev->pos().x() > m_bkg->boundingRect().width()-m_slider->boundingRect().width()-15) {
        m_slider->setX(m_bkg->boundingRect().width()-m_slider->boundingRect().width()-15);
        m_txt->setText("100");
        m_txt->setX(m_slider->pos().x()+6);
    }
    else {
        m_slider->setX(ev->pos().x()-m_slider->boundingRect().width()/2);
        m_txt->setX(m_slider->pos().x()+8);
        m_txt->setText(QString::number(round(100*(m_slider->pos().x()-1)/(m_bkg->boundingRect().width()-15)+1)));
    }
}

void OptionSlider::mouseReleaseEvent(QGraphicsSceneMouseEvent *ev) {
    m_value = round(100*(m_slider->pos().x()-1)/(m_bkg->boundingRect().width()-15)+1);
}

int OptionSlider::optionState() {
    return m_value;
}

OptionSlider::~OptionSlider() {
}
