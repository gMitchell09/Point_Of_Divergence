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
public: // Our Defined types
    struct DatagramFormat {
        qint32 pos_x;
        qint32 pos_y;
        qint32 vel_x;
        qint32 vel_y;
        qint64 timestamp;

        QByteArray serialize() {
            char* ret = (char*) malloc(sizeof(this));
            memcpy(ret, this, sizeof(this));

            QByteArray ret_byte(ret, sizeof(this));

            free(ret);

            return ret_byte;
        }

        void deserialize(char* data, size_t size) {
            if (size == sizeof(this))
                memcpy(this, data, sizeof(this));
        }

        QString toString() {
            return "DatagramFormat: Pos: (" + QString::number(pos_x) +
                    ", " + QString::number(pos_y) + ")\nVel: (" + QString::number(vel_x) +
                    ", " + QString::number(vel_y) + ")\nTimestamp: " + QString::number(timestamp);
        }
    };

private:
    explicit NetworkManager(QObject *parent = 0);
    static NetworkManager *m_instance;
    QUdpSocket udpSocket;
    std::vector<DatagramFormat> datagramVector;

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

    void sendDatagram(DatagramFormat d, QHostAddress host);

    // Inlines:
    bool hasPendingDatagrams() { return datagramVector.empty(); }
    DatagramFormat nextDatagram() {
        DatagramFormat dg = datagramVector.back();
        datagramVector.pop_back();
        return dg;
    }

signals:
    void networkPlayerConnected();
    void networkPlayerConnectionLost();

public slots:
    void readyRead();
};

#endif // NETWORKMANAGER_H
