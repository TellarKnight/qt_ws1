#ifndef CONTROLUDP_H
#define CONTROLUDP_H
#include <QObject>
#include <QThread>
#include <QDebug>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>

class ThreadSend;

class ControlUdp : public QObject
{
    Q_OBJECT
public:
    explicit ControlUdp(QObject *parent = 0);

public slots:
    void timeUpDate();
    void on_udpdataready();
    void setpara(float roll, float pitch, float yaw, float thrust);

public:
    void setspeed(qint16 x1,qint16 x2,qint16 x3,qint16 x4);
    void openudp();
    void closeudp();

private:

    void processTheDatagram(QByteArray datagram);
    uint8_t mavlink_msg_decode(float (*Pwork_data)[10], mavlink_message_t msg);

    //mavlink
    mavlink_message_t msg;
    mavlink_status_t status;
    float Pwork_data[1][10];

    float m_roll;
    float m_pitch;
    float m_yaw;
    float m_thrust;

    QUdpSocket *udp_socket_tx;
    QUdpSocket *udp_socket_rx;
    QHostAddress IP_local;
    QHostAddress IP_remote;
    int Port_Tx;
    int Port_Rx;


    ThreadSend *m_sendthread;
};
#endif // CONTROLUDP_H
