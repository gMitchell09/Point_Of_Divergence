/***************************************************
** Filename: ITriggerable.h
** Class: 
** Created: 3/31/2014
** Author: mitchellge
** Use Case #XX
** Use Case Description: Lorem ipsum dolor sit amet
****************************************************/

#ifndef ITRIGGERABLE_H
#define ITRIGGERABLE_H

#include <QObject>
#include <QPixmap>

class ITriggerable : public QObject {
    Q_OBJECT

public:
    virtual bool isController() { return false; }
    virtual bool isReceiver() { return true; }

    /// Signals to be sent by "Controller" object to receivers in response to a
    ///     trigger event
signals:
    void stateChanged(bool state);

    /// Slots handled by "Receiver" object when "Controller" toggles state
public slots:
    virtual void controllerStateChanged(bool state) = 0;
};

#endif // ITRIGGERABLE_H
