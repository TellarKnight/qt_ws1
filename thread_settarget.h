#ifndef THREAD_SETTARGET_H
#define THREAD_SETTARGET_H

#include <QObject>
#include <QMutex>
#include <QString>

class thread_settarget : public QObject
{
        Q_OBJECT

public:
    explicit thread_settarget(QObject *parent = nullptr);
    double target[3];

public slots:
   void target_set();
   void looprun();
   void target_get(double num1, double num2, double num3);

signals:
   void set_begin();
   void loopstart();
   void set_begin2();

private:
   bool flag;
};

#endif // THREAD_SETTARGET_H
