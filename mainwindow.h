#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets/QMainWindow>
#include <QImage>
#include <QProcess>
#include <QComboBox>
#include <QSpinBox>
#include <QUdpSocket>
#include <QTimer>
#include <QTime>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <math.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_grid.h>
#include <qwt_scale_draw.h>
#include<sys/select.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <obj_thread.h>
#include <QThread>
#include <QCheckBox>
#include <QSlider>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QVector<double> xdata_pos;
    QVector<double> ydata_pos;
    QVector<double> xdata_vision;
    QVector<double> ydata_vision;
    QVector<double> zdata_pos;
    QVector<double> zdata_vision;
    QTimer *Timer_get_pos;
    QTimer *Timer_set_pos;
    QTimer updateTimer;
    QString demoName;
    QwtPlotCurve *curve;
    QwtPlotCurve *curve_vision;
    double getData(double inteval);
    void setupRealtimeDataDemo(QwtPlot *qwtplot);  
    //void SendStart();
    QThread* m_objThread;
    QThread* thread_settarget;
    obj_thread* th_sub;
    obj_thread* th_set;
    QString t_str1;
    QString t_str2;
    QString t_str3;
    int count_tset=0;
    int count_pos=0;
    double dst_arrived=2;
signals:
    //void Signal_sub(bool);
    void signal_show(double num1,double num2,double num3,double num4,double num5,double num6,double num7);
    //void Signal_close(bool);
    //void signal_set_pos(double num1,double num2,double num3,double num4)
    void signal_set_pos(double num1,double num2,double num3,double num4);
    //void signal_back(double num1,double num2,double num3,double num4);

public slots:
    //void on_buttonClose_clicked();
    void updatedataSlot(double num1,double num2,double num3,double num4,double num5,double num6,double num7);
//    void pos_show(double num1,double num2,double num3);
    void ThreadCtrl();
    void ThreadCtrl2();
    void ThreadStop();
    //void ThreadStop2();
    void t_set();
    void data_clear();
    //void disarm();
    void get_back();
    void show_speed(int num);
    void save_log();

private:
    Ui::MainWindow *ui;
    int size=0;

};
#endif // MAINWINDOW_H
