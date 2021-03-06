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
#include <QAbstractSocket>
#include <QFile>
#include <QHostAddress>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QHostAddress>
#include <QPointF>
#include <QTimer>
#include <QByteArray>
#include <QDataStream>
#include <QTcpServer>

#include <queue>
#include <Box2D.h>

/// NetworkManager
class NetworkManager : public QObject
{
    Q_OBJECT
public:
    /// DatagramFormat
    /*! This defines the packet format used in PoD and also defines serialization and deserialization methods
        to convert datagrams (QByteArray's) to and from this format */
    struct DatagramFormat {
        b2Vec2 pos;
        b2Vec2 vel;
        int mainCharFrame;
        int mainCharAnim;
        qint64 timestamp;

        /// Convert our struct into a QByteArray
        QByteArray serialize() {
            char* ret = (char*) malloc(sizeof(DatagramFormat));
            memcpy(ret, this, sizeof(DatagramFormat));

            QByteArray ret_byte(ret, sizeof(DatagramFormat));

            free(ret);

            return ret_byte;
        }

        /// Deconstruct a packet into our struct
        void deserialize(char* data, size_t size) {
            Q_UNUSED(size);
            memcpy(this, data, sizeof(DatagramFormat));
        }

        QString toString() {
            return "DatagramFormat: Pos: (" + QString::number(this->pos.x) +
                    ", " + QString::number(this->pos.y) + ")\nVel: (" + QString::number(this->vel.x) +
                    ", " + QString::number(this->vel.y) + ")\nTimestamp: " + QString::number(timestamp);
        }
    };

    void connectToPeer();

private:
    /// Our shared QUdpSocket
    QUdpSocket m_udpSocket;

    QHostAddress m_peerAddress;

    /// TCP socket
    QTcpSocket *m_tcpSocket;
    QTcpServer m_tcpServer;

    /// The queue that all received datagrams are inserted into upon receipt
    std::queue<DatagramFormat> m_datagramQueue;

    bool m_isConnected;
    bool m_isTryingToConnect;

    QString m_tmxPath;
    QString m_tmpFilePath;

    /// Functions for TXing
    void sendTmx();

    QTimer *m_watchdog;

public:
    explicit NetworkManager(QString tmx, QString tmpFilePath, QObject *parent = 0);

    /// Bind and open our socket to allow the receipt of incoming datagrams
    void startListeningUDP();

    /// Transmit "hello world" packet and startListening
    void connectToPeer(QHostAddress &address);

    /// Open TCP port for connection
    void startListeningTCP();

    /// Get the local machine QHostAddress
    QHostAddress getThisAddr();

    /// Transmit a datagram to host
    qint64 sendDatagram(DatagramFormat d);

    /// Check if our incoming datagram queue is empth
    bool hasPendingDatagrams() { return !m_datagramQueue.empty(); }

    /// Pull the next datagram from our queue
    DatagramFormat nextDatagram() {
        DatagramFormat dg = m_datagramQueue.front();
        m_datagramQueue.pop();
        return dg;
    }

    bool isConnected() { return m_isConnected; }

    ~NetworkManager() {}

signals:
    /// Emitted when a player connects to us
    void networkPlayerConnected();

    /// Emitted when the time since a packet has been received exceeds a certain threshold
    void networkPlayerConnectionLost();

public slots:
    /// Hooked to our socket to signal when we can consume a datagram
    void readyReadTCP();
    void connectionTimeout();
    void peerConnected();
    void readyReadUDP();
    void tcpSocketStateChanged(QAbstractSocket::SocketState);
    void acceptConnection();
};

#endif // NETWORKMANAGER_H
