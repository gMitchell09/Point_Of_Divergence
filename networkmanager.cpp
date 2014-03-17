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
#include <QMessageBox>

#define COM_PORT (quint16)12345
#define NETWORK_TIMEOUT 1500

NetworkManager::NetworkManager(QString tmxPath, QString tmpFilePath, QObject *parent) :
    QObject(parent)
{
    m_tmxPath = tmxPath;
    m_tmpFilePath = tmpFilePath;
    m_watchdog = new QTimer(this);
    m_watchdog->setInterval(NETWORK_TIMEOUT);
    m_watchdog->setSingleShot(true);
}

QHostAddress NetworkManager::getThisAddr() {
    QHostAddress addr;
    addr = m_tcpSocket.localAddress();
    return addr;
}

void NetworkManager::startListeningUDP() {
    if (!m_udpSocket.bind(QHostAddress::AnyIPv4)) {
        qDebug() << "Unable to bind socket";
    }
    else connect(&m_udpSocket, SIGNAL(readyRead()), this, SLOT(readyReadUDP()));
}

void NetworkManager::readyReadUDP() {
    while (m_udpSocket.hasPendingDatagrams()) {
        DatagramFormat datagram;
        QHostAddress addr;
        quint16 port;
        char *data = (char*) malloc(sizeof(char) * m_udpSocket.pendingDatagramSize());

        m_udpSocket.readDatagram(data, m_udpSocket.pendingDatagramSize(), &addr, &port);
        datagram.deserialize(data, m_udpSocket.pendingDatagramSize());
        free(data);

        m_datagramQueue.push(datagram);

        // Pet the dog
        m_watchdog->start();

        qDebug() << "Datagram from: " << addr << ":" << port;
        qDebug() << datagram.toString();
    }
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

    qDebug() << "Fuck";
}

/// Peer 1 Flow
void NetworkManager::startListeningTCP() {
    m_isRuler = true;
    m_tcpSocket.bind(QHostAddress::AnyIPv4, COM_PORT);
    connect(&m_tcpSocket, SIGNAL(connected()), this, SLOT(peerConnected()));
}

void NetworkManager::peerConnected() {
    emit networkPlayerConnected();
    m_isConnected = true;
    connect(&m_tcpSocket, SIGNAL(readyRead()), this, SLOT(packetReceived()));

    this->sendTmx();
}

void NetworkManager::sendTmx() {
    QFile fileTmx(m_tmxPath);
    fileTmx.open(QIODevice::ReadOnly);
    if(!fileTmx.isOpen()) qDebug() << "Error: TMX File Nonexistent";
    else {
        QDataStream in(&fileTmx);
        QByteArray line;
        in >> line;
        fileTmx.close();

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_0);
        out << (quint16)0;
        out << line;
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));
    }
}

/// Peer 2 Flow
void NetworkManager::connectToPeer(QHostAddress &address) {
    m_isRuler = false;
    m_watchdog->start();

    m_tcpSocket.connectToHost(address, COM_PORT);

    if (!m_tcpSocket.waitForConnected(5000)) {
        qDebug() << "Error: " << m_tcpSocket.errorString();
    } else {
        qDebug() << "Connected?";
    }

    connect(&m_tcpSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(tcpSocketStateChanged(QAbstractSocket::SocketState)));
    connect(&m_tcpSocket, SIGNAL(readyRead()), this, SLOT(readyReadTCP()));
}

void NetworkManager::readyReadTCP() {
    qDebug() << "Packet Received!";

    QDataStream in(&m_tcpSocket);
    static quint16 blockSize = 0;

    in.setVersion(QDataStream::Qt_5_0);
    if (blockSize == 0) {
        if (m_tcpSocket.bytesAvailable() < (int)sizeof(quint16)) return;
        in >> blockSize;
    }

    if (m_tcpSocket.bytesAvailable() < blockSize) return;

    QByteArray line;
    in >> line;

    QFile file(m_tmpFilePath);
    if(!(file.open(QIODevice::Append)))
        qDebug() << "Couldn't open file";

    blockSize = 0;
    file.close();
}

void NetworkManager::tcpSocketStateChanged(QAbstractSocket::SocketState state) {
    qDebug() << "State changed: " << state;
    if (state == QAbstractSocket::SocketState::ConnectedState) {
        m_isConnected = true;
        emit networkPlayerConnected();
    } else if (state == QAbstractSocket::SocketState::UnconnectedState) {
        m_isConnected = false;
        emit networkPlayerConnectionLost();
    }
}
