/***************************************************
** Filename: networkmanager.h
** Class: NetworkManager
** Created: 3/12/2014
** Author: mitchellge
** Use Case #XX
** Use Case Description: Lorem ipsum dolor sit amet
****************************************************/

#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>

class NetworkManager : public QObject
{
    Q_OBJECT
private:
    explicit NetworkManager(QObject *parent = 0);
    static NetworkManager *m_instance;
    QUdpSocket udpSocket;

public:
    static NetworkManager * Instance() {
        if (!m_instance) {
            m_instance = new NetworkManager;
        }
        return m_instance;
    }

    ~NetworkManager() {
        delete m_instance;
    }

    void startListening();
    void connectToHost(QHostAddress &address);
    QHostAddress getThisAddr();

signals:
    void networkPlayerConnected();
    void networkPlayerConnectionLost();

public slots:
    void readyRead();
};

#endif // NETWORKMANAGER_H
