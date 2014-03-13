/***************************************************
** Filename: networkmanager.cpp
** Class: NetworkManager
** Created: 3/12/2014
** Author: mitchellge
** Use Case #XX
** Use Case Description: Lorem ipsum dolor sit amet
****************************************************/

#include "networkmanager.h"
#include <QDebug>
#include <QByteArray>

#define COM_PORT (quint16)12345
#define NETWORK_TIMEOUT 1500

NetworkManager* NetworkManager::m_instance = NULL;

NetworkManager::NetworkManager(QObject *parent) :
    QObject(parent)
{
    m_watchdog = new QTimer(this);
    m_watchdog->setInterval(NETWORK_TIMEOUT);
    m_watchdog->setSingleShot(true);
}

void NetworkManager::startListening() {
    if (!m_udpSocket.bind(QHostAddress::AnyIPv4)) {
        qDebug() << "Unable to bind socket";
    }
    else connect(&m_udpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

QHostAddress NetworkManager::getThisAddr() {
    return m_udpSocket.localAddress();
}

void NetworkManager::readyRead() {
    while (m_udpSocket.hasPendingDatagrams()) {
        DatagramFormat datagram;
        QHostAddress addr;
        quint16 port;
        char *data = (char*) malloc(sizeof(char) * m_udpSocket.pendingDatagramSize());

        m_udpSocket.readDatagram(data, m_udpSocket.pendingDatagramSize(), &addr, &port);
        datagram.deserialize(data, m_udpSocket.pendingDatagramSize());
        free(data);

        // Case 1: We are already connected so let's short circuit our checking and
        //         pass our datagram right onto our queue
        if (m_isConnected) {
            m_datagramQueue.push(datagram);
        }
        // Case 2: We initiated a connection to another host and are awaiting an ACK
        else if (m_isTryingToConnect && datagram.ackInitialConnection) {
            m_isTryingToConnect = false;
            m_isConnected = true;
        }
        // Case 3: They are initiating a communication with us so we should reply with an ACK
        else if (datagram.initiateConnection) {
            // We aren't connected to a peer and someone has requested access to communicate with us so let's respond!
            datagram.ackInitialConnection = true;
            this->sendDatagram(datagram, addr);
        }
        // Case 4: We tried connecting but the host declined our request or they responded
        //         after our connection attempt timed out
        else {
            qDebug() << "THE OTHER HOST IS A JERKFACE";
        }

        // Pet the dog
        m_watchdog->start();

        qDebug() << "Datagram from: " << addr << ":" << port;
        qDebug() << datagram.toString();
    }
}

void NetworkManager::connectToHost(QHostAddress &address) {
    DatagramFormat dg;
    dg.initiateConnection = true;
    if (this->sendDatagram(dg, address) == sizeof(dg)) {
        qDebug() << "Size matches!!";
    }
    m_isTryingToConnect = true;

    m_watchdog->start();

    this->startListening();
}

qint64 NetworkManager::sendDatagram(NetworkManager::DatagramFormat d, QHostAddress host) {
    if (!m_udpSocket.isValid() || !m_udpSocket.isWritable()) {
        qDebug() << "Error!!!  QUdpSocket is not good!! >:O";
        return -1;
    }

    return m_udpSocket.writeDatagram(d.serialize(), host, COM_PORT);
}

void NetworkManager::connectionTimeout() {
    emit networkPlayerConnectionLost();
    m_isConnected = false;
    m_isTryingToConnect = false;

    qDebug() << "Fuck";
}

