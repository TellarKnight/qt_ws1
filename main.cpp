#include "mainwindow.h"

#include <QApplication>
#include <opencv.hpp>

using namespace cv;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //cv::Mat image = cv::imread("/home/x/backups/illust_86966041_20210112_005838.png", cv::IMREAD_COLOR);
    //cv::imshow("image",image);
    //cv::waitKey(0);
    return a.exec();
}
