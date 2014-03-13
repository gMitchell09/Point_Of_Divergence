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
#include <QPointF>
#include <QTimer>

#include <queue>

/// NetworkManager
class NetworkManager : public QObject
{
    Q_OBJECT
public:
    /// DatagramFormat
    /*! This defines the packet format used in PoD and also defines serialization and deserialization methods
        to convert datagrams (QByteArray's) to and from this format */
    struct DatagramFormat {
        QPointF pos;
        QPointF vel;
        qint64 timestamp;
        bool initiateConnection;
        bool ackInitialConnection;

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
            if (size == sizeof(DatagramFormat))
                memcpy(this, data, sizeof(DatagramFormat));
        }

        QString toString() {
            return "DatagramFormat: Pos: (" + QString::number(this->pos.x()) +
                    ", " + QString::number(this->pos.y()) + ")\nVel: (" + QString::number(this->vel.x()) +
                    ", " + QString::number(this->vel.y()) + ")\nTimestamp: " + QString::number(timestamp);
        }
    };

private:
    explicit NetworkManager(QObject *parent = 0);
    static NetworkManager *m_instance;

    /// Our shared QUdpSocket
    QUdpSocket m_udpSocket;

    /// The queue that all received datagrams are inserted into upon receipt
    std::queue<DatagramFormat> m_datagramQueue;

    bool m_isConnected;
    bool m_isTryingToConnect;

    QTimer *m_watchdog;

public:

    /// Get the shared instance of our NetworkManager
    static NetworkManager * Instance() {
        if (!m_instance) {
            m_instance = new NetworkManager;
        }
        return m_instance;
    }

    ~NetworkManager() {
        delete m_instance;
    }

    /// Bind and open our socket to allow the receipt of incoming datagrams
    void startListening();

    /// Transmit "hello world" packet and startListening
    void connectToHost(QHostAddress &address);

    /// Get the local machine QHostAddress
    QHostAddress getThisAddr();

    /// Transmit a datagram to host
    qint64 sendDatagram(DatagramFormat d, QHostAddress host);

    /// Check if our incoming datagram queue is empth
    bool hasPendingDatagrams() { return m_datagramQueue.empty(); }

    /// Pull the next datagram from our queue
    DatagramFormat nextDatagram() {
        DatagramFormat dg = m_datagramQueue.front();
        m_datagramQueue.pop();
        return dg;
    }

signals:
    /// Emitted when a player connects to us
    void networkPlayerConnected();

    /// Emitted when the time since a packet has been received exceeds a certain threshold
    void networkPlayerConnectionLost();

public slots:
    /// Hooked to our socket to signal when we can consume a datagram
    void readyRead();
    void connectionTimeout();
};

#endif // NETWORKMANAGER_H
