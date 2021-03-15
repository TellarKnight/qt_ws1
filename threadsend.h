#ifndef THREADSEND_H
#define THREADSEND_H
#include <QThread>

class ThreadSend : public QThread
{
    Q_OBJECT
public:
    explicit ThreadSend(QObject *parent = 0);
    ~ThreadSend();
public slots:
    void setmsec(QString msecstr);
signals:
    void sendba();

public:
    void run();

private:
    int m_msec;
};

#endif // THREADSEND_H
