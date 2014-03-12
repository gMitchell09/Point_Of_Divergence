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

#define MAX_DATAGRAM_SIZE 4096
#define COM_PORT 12345

NetworkManager* NetworkManager::m_instance = NULL;

NetworkManager::NetworkManager(QObject *parent) :
    QObject(parent)
{
}

void NetworkManager::startListening() {
    if (!udpSocket.bind(QHostAddress::AnyIPv4)) {
        qDebug() << "Unable to bind socket";
        return;
    }
    connect(&udpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

QHostAddress NetworkManager::getThisAddr() {
    return udpSocket.localAddress();
}

void NetworkManager::readyRead() {
    while (udpSocket.hasPendingDatagrams()) {
        char *data = (char*) malloc(sizeof(char) * udpSocket.pendingDatagramSize());
        QHostAddress addr;
        quint16 port;
        udpSocket.readDatagram(data, udpSocket.pendingDatagramSize(), &addr, &port);
        qDebug() << "Datagram from: " << addr << ":" << port;
        qDebug() << "Data: " << data;
        free(data);
    }
}

void NetworkManager::connectToHost(QHostAddress &address) {
    char datagram[256] = "HELLO WORLD!!!";
    udpSocket.writeDatagram(datagram, 256, address, COM_PORT);
    this->startListening();
}
