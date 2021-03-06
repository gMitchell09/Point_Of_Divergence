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

#define COM_PORT (quint16)1500
#define NETWORK_TIMEOUT 1500

NetworkManager::NetworkManager(QString tmxPath, QString tmpFilePath, QObject *parent) :
    QObject(parent),
    m_tcpSocket(NULL),
    m_isConnected(false)
{
    m_tmxPath = tmxPath;
    m_tmpFilePath = tmpFilePath;
    m_watchdog = new QTimer(this);
    m_watchdog->setInterval(NETWORK_TIMEOUT);
    m_watchdog->setSingleShot(true);
    m_isConnected = false;
}

QHostAddress NetworkManager::getThisAddr() {
    QTcpSocket tmp;
    tmp.connectToHost(QHostAddress("74.125.131.105"), 80);
    if (!tmp.waitForConnected(5000)) {
        qDebug() << tmp.errorString();
        qDebug() << "FUCK";
    }
    return tmp.localAddress();
}

void NetworkManager::startListeningUDP() {
    m_watchdog->start();

    if (!m_udpSocket.bind(QHostAddress::AnyIPv4, COM_PORT)) {
        qDebug() << "Unable to bind socket";
    }
    else {
        connect(&m_udpSocket, SIGNAL(readyRead()), this, SLOT(readyReadUDP()));
        m_isConnected = true;
    }
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

        qDebug() << "Datagram: " << datagram.toString();

        m_datagramQueue.push(datagram);

        // Pet the dog
        m_watchdog->start();
    }
}

qint64 NetworkManager::sendDatagram(NetworkManager::DatagramFormat d) {
    QUdpSocket mySock;

    qDebug() << "Host Address: " << m_peerAddress;

    return mySock.writeDatagram(d.serialize(), m_peerAddress, COM_PORT);
}

void NetworkManager::connectionTimeout() {
    emit networkPlayerConnectionLost();
    m_isConnected = false;

    QMessageBox::critical(0, "Connection Timed Out", "Connection to peer has been lost", QMessageBox::Ok);
}

////////////////////////////////////     TCP     //////////////////////////////////
/// Peer 1 Flow
void NetworkManager::startListeningTCP() {
    m_tcpServer.listen(QHostAddress::AnyIPv4, COM_PORT);

    while (!m_tcpServer.isListening() && !m_tcpServer.listen(QHostAddress::Any, COM_PORT)) {
        qDebug() << "Error: " << m_tcpServer.errorString();
    }

    connect(&m_tcpServer, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
    m_watchdog->start();
}

void NetworkManager::peerConnected() {
    m_isConnected = true;
    connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(readyReadTCP()));

    m_peerAddress = m_tcpSocket->peerAddress();

    this->sendTmx();
}

void NetworkManager::sendTmx() {
    QFile fileTmx(m_tmxPath);
    fileTmx.open(QIODevice::ReadOnly);
    if(!fileTmx.isOpen() || !fileTmx.exists()) qDebug() << "Error: TMX File Nonexistent";
    else {

        QByteArray line = fileTmx.readAll();
        fileTmx.close();

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_1);
        out << (quint32)0;
        out << line;
        out.device()->seek(0);
        out << (quint32)(block.size() - sizeof(quint32));

        m_tcpSocket->write(block);

        m_tcpSocket->flush();
    }
    m_tcpSocket->close();
    this->startListeningUDP();
    emit networkPlayerConnected();
}

/// Peer 2 Flow
void NetworkManager::connectToPeer(QHostAddress &address) {

    m_watchdog->start();

    if (!m_tcpSocket)
        m_tcpSocket = new QTcpSocket();

    m_tcpSocket->connectToHost(address, COM_PORT);

    if (!m_tcpSocket->waitForConnected(5000)) {
        qDebug() << "Error: " << m_tcpSocket->errorString();
        QMessageBox::critical(0, "Connection Timed Out", m_tcpSocket->errorString(), QMessageBox::Ok);
        m_tcpSocket->disconnectFromHost();
    } else {
        m_isConnected = true;
    }

    m_peerAddress = address;

    connect(m_tcpSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(tcpSocketStateChanged(QAbstractSocket::SocketState)));
    connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(readyReadTCP()));
}

void NetworkManager::readyReadTCP() {

    QDataStream in(m_tcpSocket);
    static quint32 blockSize = 0;

    in.setVersion(QDataStream::Qt_5_1);
    if (blockSize == 0) {
        if (m_tcpSocket->bytesAvailable() < (int)sizeof(quint16)) return;
        in >> blockSize;
    }

    if (m_tcpSocket->bytesAvailable() < blockSize) {
        qDebug() << "Not enough bytes";
        return;
    }

    QByteArray line;
    in >> line;

    qDebug() << "Line: " << line;

    QFile file(m_tmpFilePath);
    if(!(file.open(QIODevice::WriteOnly)))
        qDebug() << "Couldn't open file";

    file.write(line);

    blockSize = 0;
    file.close();

    m_tcpSocket->close();

    // Start listening on the UDP side
    this->startListeningUDP();
    emit networkPlayerConnected();
}

void NetworkManager::tcpSocketStateChanged(QAbstractSocket::SocketState state) {
    if (state == QAbstractSocket::SocketState::ConnectedState) {
        m_isConnected = true;
    }
}

void NetworkManager::acceptConnection() {
    m_tcpSocket = m_tcpServer.nextPendingConnection();
    this->peerConnected();
}
