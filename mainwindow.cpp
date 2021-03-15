#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostAddress>
#include "obj_thread.h"
#include "thread_settarget.h"
#include <QThread>
#include <QString>
#include <cstring>
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_objThread= new QThread();
    th_sub = new obj_thread();
    th_sub->moveToThread(m_objThread);
    t_str1 = ui->label_8->text();
    t_str2 = ui->label_10->text();
    t_str3 = ui->label_12->text();

    myTimer = new QTimer(this);
    myTimer2 = new QTimer(this);

    connect(myTimer, &QTimer::timeout, th_sub, &obj_thread::dealMsg);
    connect(myTimer2, &QTimer::timeout, this, &MainWindow::t_set);
    //connect(m_objThread,&QThread::finished,m_objThread,&QObject::deleteLater);
    //connect(this,&MainWindow::Signal_close,th_sub,&obj_thread::stop);
    //connect(ui->checkBox_2, SIGNAL(stateChanged(int)), this,SLOT(&obj_thread::dealMsg));
    connect(ui->pushButton, SIGNAL(clicked(bool)), this,SLOT(ThreadCtrl()));
    //connect(this,&MainWindow::Signal_sub,th_sub,&obj_thread::dealMsg);
    //connect(this,SIGNAL(Signal_sub(bool)),th_sub,SLOT(dealMsg()));
    connect(ui->pushButton_2, SIGNAL(clicked(bool)),this,SLOT(ThreadStop()));
    connect(ui->pushButton_5, SIGNAL(clicked(bool)),this,SLOT(ThreadStop2()));
    //connect(this,SIGNAL(Signal_sub(bool)),th_sub,SLOT(dealMsg()));
    connect(th_sub,SIGNAL(signal_show(double,double,double)),this,SLOT(updatedataSlot(double,double,double)));
    connect(th_sub,SIGNAL(signal_show(double,double,double)),this,SLOT(pos_show(double,double,double)));
    connect(ui->pushButton_6,SIGNAL(clicked(bool)),this,SLOT(ThreadCtrl2()));
    connect(this,&MainWindow::signal_set_pos,th_sub,&obj_thread::target_set);

    //m_objThread->start();
    /**start**/
    //ThreadCtrl(int state);
    //SendStart();
    setupRealtimeDataDemo(ui->qwtPlot);
}


MainWindow::~MainWindow()
{
    delete ui;
}

/*void MainWindow::SendStart()
{
    emit Signal_sub();
}*/

void MainWindow::setupRealtimeDataDemo(QwtPlot *qwtplot)
{

    //xdata.append(0);
    //ydata.append(0);

    demoName = "trajectory";
    qwtplot->setTitle(demoName);
    qwtplot->setCanvasBackground(Qt::black);//背景
    qwtplot->insertLegend(new QwtLegend(),QwtPlot::RightLegend);//标签

    curve = new QwtPlotCurve();
    curve->setTitle("position");//曲线名字
    curve->setPen( Qt::yellow, 1 );//曲线的颜色 宽度;

    QTime curtime;
    curtime=curtime.currentTime();
    qwtplot->setAxisTitle(QwtPlot::xBottom, " X/m");
    qwtplot->setAxisTitle(QwtPlot::yLeft,"Y/m");
    qwtplot->setAxisScale(QwtPlot::yLeft,-3,3,1);
    qwtplot->setAxisScale(QwtPlot::xBottom,-3,3,1);

    QwtPlotZoomer *zoomer = new QwtPlotZoomer( qwtplot->canvas() );
    zoomer->setRubberBandPen( QColor( Qt::blue ) );
    zoomer->setTrackerPen( QColor( Qt::black ) );
    zoomer->setMousePattern(QwtEventPattern::MouseSelect2,Qt::RightButton, Qt::ControlModifier );
    zoomer->setMousePattern(QwtEventPattern::MouseSelect3,Qt::RightButton );
    QwtPlotMagnifier *magnifier = new QwtPlotMagnifier( qwtplot->canvas() );                 //默认的滑轮及右键缩放功能  图形的整体缩放
    //    magnifier->setMouseButton(Qt::LeftButton);     //设置哪个按钮与滑轮为缩放画布  如果不设置(注册掉当前行)按钮默认为滑轮以及右键为缩放

    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->enableX( true );//设置网格线
    grid->enableY( true );
    grid->setMajorPen( Qt::white, 0, Qt::SolidLine );
    grid->attach(qwtplot);
    //connect(&updateTimer,SIGNAL(timeout()),this,SLOT(updatedataSlot()));
    updateTimer.start(0);
}

//更新xdata，ydata
void MainWindow::updatedataSlot(double num1,double num2,double num3){
    static QTime dataTime(QTime::currentTime());
    long int eltime = dataTime.elapsed();
    static int lastpointtime = 0;
    int size = (eltime - lastpointtime);
    if(size>0){//有数据传入
        //xdata.erase(xdata.begin(),xdata.begin()+size);//擦除多余的数据
        //ydata.erase(ydata.begin(),ydata.begin()+size);
        for(int i=1;i<size+1;i++){
            xdata.append(num1);
            ydata.append(num2);
        }
        lastpointtime = eltime;
    }

    curve->setSamples(xdata,ydata);
    curve->attach(ui->qwtPlot);
    ui->qwtPlot->replot();

    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    double fpstime = (double)eltime/1000.0-lastFpsKey;
    if ( fpstime> 2) // average fps over 2 seconds
    {
        ui->statusbar->showMessage(
                    QString("%1 FPS")
                    .arg(frameCount/fpstime, 0, 'f', 0)
                    , 0);
        lastFpsKey = (double)eltime/1000.0;
        frameCount = 0;
    }
}
void MainWindow::pos_show(double num1,double num2,double num3){
   ui->label_6->setText(QString::number(num1));
   ui->label_4->setText(QString::number(num2));
   ui->label->setText(QString::number(num3));
}

//调用线程的runSomeBigWork2
/*void MainWindow::onButtonObjectMove2Thread2Clicked()
{
    if(!m_objThread)
    {
        startObjThread();
    }
    emit startObjThreadWork2();//主线程通过信号换起子线程的槽函数
    ui->textBrowser->append("start Obj Thread work 2");
}*/


//开启定时器，使线程运行
void MainWindow::ThreadCtrl()
{

    //若定时器没有工作
    if(myTimer->isActive() == false)
    {
        myTimer->start(50);
    }
    //启动线程，处理数据

    m_objThread->start();
}
void MainWindow::ThreadCtrl2()
{

    //若定时器没有工作
    if(myTimer2->isActive() == false)
    {
        myTimer2->start(500);
    }
    //启动线程，处理数据
    m_objThread->start();
    ui->label_14->setText("sending");
}

void MainWindow::ThreadStop()
{
    myTimer->stop(); //关闭定时器
}
void MainWindow::ThreadStop2()
{
    myTimer2->stop(); //关闭定时器
    ui->label_14->setText("no order");
}
void MainWindow::t_set(){
   emit signal_set_pos(t_str1.toFloat(),t_str2.toFloat(),t_str3.toFloat());
}


