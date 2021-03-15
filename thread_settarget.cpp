#include "thread_settarget.h"
#include <QThread>
#include <QDebug>
#include <QMutexLocker>
#include <QElapsedTimer>
#include <limits>
#include <math.h>
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
#include <QString>
#include "mainwindow.h"

thread_settarget::thread_settarget(QObject *parent):QObject(parent)
{
    flag = false;
}

/*thread_settarget::~thread_settarget()
{

}*/

void thread_settarget::target_get(double num1, double num2, double num3){
   target[0]=num1;
   target[1]=num2;
   target[2]=num3;
   emit set_begin();
}

void thread_settarget::target_set()
{
        int port = 8999;
        int sockfd;
        // 创建socket
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if(-1==sockfd){
            puts("Failed to create socket");
        }

        // 设置地址与端口
        struct sockaddr_in addr;
        socklen_t  addr_len=sizeof(addr);
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;       // Use IPV4
        addr.sin_port   = htons(port);    //
        addr.sin_addr.s_addr = htonl(INADDR_ANY);

        // Time out
        struct timeval tv;
        tv.tv_sec  = 0;
        tv.tv_usec = 200000;  // 200 ms
        setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(struct timeval));

        if (bind(sockfd, (struct sockaddr*)&addr, addr_len) == -1){
            printf("Failed to bind socket on port %d\n", port);
            close(sockfd);
        }

        sendto(sockfd, target, sizeof(target), 0, (sockaddr*)&addr, addr_len);
        printf("Sended");
        close(sockfd);
        emit loopstart();
}
void thread_settarget::looprun(){
   emit set_begin2();
}



