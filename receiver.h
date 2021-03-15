#ifndef RECEIVER_H
#define RECEIVER_H
#include <QWidget>
#include <QUdpSocket> //UDP套接字

namespace Ui {
class receiver;
}

class receiver : public QWidget
{
    Q_OBJECT

public:
    explicit receiver(QWidget *parent = 0);
    ~receiver();

    void dealMsg(); // 处理发过来的数据

private slots:
    void on_buttonSend_clicked();

    void on_buttonClose_clicked();

private:
    Ui::receiver *ui;
    QUdpSocket *udpSocket;
};
#endif // RECEIVER_H
