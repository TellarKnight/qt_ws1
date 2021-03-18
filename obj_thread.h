#ifndef OBJ_THREAD_H
#define OBJ_THREAD_H
#include <QObject>
#include <QMutex>
#include <QString>

class obj_thread : public QObject
{
    Q_OBJECT

public:   
    obj_thread(QObject *parent = nullptr);
    ~obj_thread();
    float pos_x,pos_y,pos_z;
    float buffer[3];
    double target[4];

public slots:
    void dealMsg();
    //void target_set(double num1, double num2, double num3,double num4);
    void target_set(double num1, double num2, double num3,double num4);
    //void stop();


private:


signals:
   void signal_show(double num1,double num2,double num3);

};
#endif // OBJ_THREAD_H
