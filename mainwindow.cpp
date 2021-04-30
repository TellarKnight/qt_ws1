#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostAddress>
#include "obj_thread.h"
#include "thread_settarget.h"
#include <QThread>
#include <QString>
#include <cstring>
#include <sys/select.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/TwistStamped.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <fstream>
#include <stdlib.h>
#include <sstream>
#include <iomanip>
#include <chrono>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_objThread= new QThread();
    th_sub = new obj_thread();
    th_sub->moveToThread(m_objThread);
    t_str1 = ui->lineEdit->text();
    t_str2 = ui->lineEdit_2->text();
    t_str3 = ui->lineEdit_3->text();

    Timer_get_pos = new QTimer(this);
    Timer_set_pos = new QTimer(this);

    connect(Timer_get_pos, &QTimer::timeout, th_sub, &obj_thread::dealMsg);
    connect(Timer_set_pos, &QTimer::timeout, this, &MainWindow::t_set);
    //connect(m_objThread,&QThread::finished,m_objThread,&QObject::deleteLater);
    connect(ui->pushButton, SIGNAL(clicked(bool)), this,SLOT(ThreadCtrl()));
    connect(ui->pushButton_2, SIGNAL(clicked(bool)),this,SLOT(ThreadStop()));
    //connect(ui->pushButton_5, SIGNAL(clicked(bool)),this,SLOT(ThreadStop2()));
    connect(th_sub,SIGNAL(signal_show(double,double,double,double,double,double,double)),this,SLOT(updatedataSlot(double,double,double,double,double,double,double)));
    //connect(th_sub,SIGNAL(signal_show(double,double,double)),this,SLOT(pos_show(double,double,double)));
    connect(ui->pushButton_6,SIGNAL(clicked(bool)),this,SLOT(ThreadCtrl2()));
    connect(this,&MainWindow::signal_set_pos,th_sub,&obj_thread::target_set);
    connect(ui->pushButton_7,SIGNAL(clicked(bool)),this,SLOT(data_clear()));
    connect(ui->pushButton_4,SIGNAL(clicked(bool)),this,SLOT(get_back()));
    //connect(this,&MainWindow::signal_back,th_sub,&obj_thread::target_set);
    connect(ui->verticalSlider,SIGNAL(valueChanged(int)),this,SLOT(show_speed(int)));
    connect(ui->btn_log,SIGNAL(clicked(bool)),this,SLOT(save_log()));

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
    demoName = "trajectory";
    qwtplot->setTitle(demoName);
    qwtplot->setCanvasBackground(Qt::black);//背景
    qwtplot->insertLegend(new QwtLegend(),QwtPlot::RightLegend);//标签

    curve = new QwtPlotCurve();
    curve->setTitle("position_fused");//曲线名字
    curve->setPen( Qt::yellow, 1 );//曲线的颜色 宽度;

    curve_vision = new QwtPlotCurve();
    curve_vision->setTitle("position_vision");//曲线名字
    curve_vision->setPen( Qt::red, 1 );//曲线的颜色 宽度;

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
void MainWindow::updatedataSlot(double num1,double num2,double num3, double num4,double num5,double num6,double num7){
    ui->PosxValueLabel->setText(QString::number(num1,'f',4));
    ui->PosyValueLabel->setText(QString::number(num2,'f',4));
    ui->PoszValueLabel->setText(QString::number(num3,'f',4));
    ui->label_vision_x->setText(QString::number(num4,'f',4));
    ui->label_vision_y->setText(QString::number(num5,'f',4));
    ui->label_vision_z->setText(QString::number(num6,'f',4));
    dst_arrived=num7;
    if(dst_arrived==1){
        ui->lineEdit_offb_state->setText("arrived");
        //printf("Sended:%f\n",dst_arrived);
        }
        else if(dst_arrived==0){
            ui->lineEdit_offb_state->setText("not yet");
        }
        else{
          ui->lineEdit_offb_state->setText("no offboard");
    }
    static QTime dataTime(QTime::currentTime());
    long int eltime = dataTime.elapsed();
    static int lastpointtime = 0;
    size = (eltime - lastpointtime);
    if(size>0){//有数据传入
       xdata_pos.append(num1);
       ydata_pos.append(num2);
       zdata_pos.append(num3);
       xdata_vision.append(num4);
       ydata_vision.append(num5);
       zdata_vision.append(num6);
       count_pos=count_pos+1;
       lastpointtime = eltime;
    }
    if(count_pos>1){
      if((xdata_pos[count_pos-2]-xdata_pos[count_pos-1]>0.2)||(ydata_pos[count_pos-2]-ydata_pos[count_pos-1]>0.2)||(zdata_pos[count_pos-2]-zdata_pos[count_pos-1]>0.2)){
        emit signal_set_pos(0,0,0,3);
      }
    }
    curve->setSamples(xdata_pos,ydata_pos);
    curve->attach(ui->qwtPlot);
    curve_vision->setSamples(xdata_vision,ydata_vision);
    curve_vision->attach(ui->qwtPlot);
    ui->qwtPlot->replot();

    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    double fpstime = (double)eltime/1000.0-lastFpsKey;
    if ( fpstime> 2) // average fps over 2 seconds
    {
        ui->statusbar->showMessage(QString("%1 FPS").arg(frameCount/fpstime, 0, 'f', 0), 0);
        lastFpsKey = (double)eltime/1000.0;
        frameCount = 0;
    }
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
    if(Timer_get_pos->isActive() == false)
    {
        Timer_get_pos->start(50);
    }
    //启动线程，处理数据
    m_objThread->start();
}
void MainWindow::ThreadCtrl2()
{

    if(Timer_set_pos->isActive() == false)
    {
        Timer_set_pos->start(50);
    }
    //启动线程，处理数据
    m_objThread->start();
    ui->lineEdit_4->setText("sending");
}

void MainWindow::ThreadStop()
{
    Timer_get_pos->stop(); //关闭当前位置获取定时器
    m_objThread->exit();
}

/*void MainWindow::ThreadStop2()
{
    Timer_set_pos->stop(); //关闭目标地点发送定时器
    ui->lineEdit_4->setText("waiting orders");
}*/

void MainWindow::t_set(){
   count_tset=count_tset+1;
   if(count_tset<21)
   {
     emit signal_set_pos(ui->lineEdit->text().toDouble(),ui->lineEdit_2->text().toDouble(),ui->lineEdit_3->text().toDouble(),ui->lineEdit_tkf->text().toDouble());
     printf("%d\n",count_tset);
   }
   else
   {
      count_tset=0;
      Timer_set_pos->stop();
      ui->lineEdit_4->setText("waiting orders");
   }
   //printf("%f",ui->lineEdit_tkf->text().toDouble());
   //emit signal_set_pos(ui->lineEdit->text().toFloat(),ui->lineEdit_2->text().toFloat(),ui->lineEdit_3->text().toFloat());
}

void MainWindow::data_clear(){
    ui->PosxValueLabel->setText("0.00");
    ui->PosyValueLabel->setText("0.00");
    ui->PoszValueLabel->setText("0.00");
    ui->label_vision_x->setText("0.00");
    ui->label_vision_y->setText("0.00");
    ui->label_vision_z->setText("0.00");
    ui->lineEdit->setText("0.00");
    ui->lineEdit_2->setText("0.00");
    ui->lineEdit_3->setText("0.00");
    curve->setSamples({},{});
    curve_vision->setSamples({},{});
    ui->qwtPlot->replot();
    xdata_pos={};
    ydata_pos={};
    zdata_pos={};
    xdata_vision={};
    ydata_vision={};
    zdata_vision={};
    ui->verticalSlider->setValue(0);
    ui->lineEdit_tkf->setText("0");
    dst_arrived=2;
    ui->lineEdit_offb_state->setText("no offboard");
    count_tset=0;
    count_pos=0;
}

void MainWindow::get_back(){
    ui->lineEdit->setText("0.00");
    ui->lineEdit_2->setText("0.00");
    ui->lineEdit_3->setText("0.00");
}

void MainWindow::show_speed(int value){
    ui->label_8->setText(QString::number((double)value/50));
}

void MainWindow::save_log(){
    double M[xdata_pos.count()][6];
  for(int i=0;i<xdata_pos.count();++i){
      M[i][0]=xdata_pos[i];
  }
  for(int i=0;i<xdata_pos.count();++i){
      M[i][1]=xdata_vision[i];
  }
  for(int i=0;i<xdata_pos.count();++i){
      M[i][2]=ydata_pos[i];
  }
  for(int i=0;i<xdata_pos.count();++i){
      M[i][3]=ydata_vision[i];
  }
  for(int i=0;i<xdata_pos.count();++i){
      M[i][4]=zdata_pos[i];
  }
  for(int i=0;i<xdata_pos.count();++i){
      M[i][5]=zdata_vision[i];
  }
  auto t = chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  stringstream ss;
  ss << "/home/x/qt_ws1/logs/"<<"log_"<<put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S")<<".txt";
  string nameLogStr = ss.str();
  ofstream LogStream(nameLogStr);
  for(int i=0;i<xdata_pos.count();i++)
      for(int j=0;j<6;j++)
          if(j!=5){
             LogStream<<M[i][j]<<" ";
          }
          else{
             LogStream<<M[i][j]<<"\n";
          }
  LogStream.close();
}
