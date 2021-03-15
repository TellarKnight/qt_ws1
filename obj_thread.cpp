#include "obj_thread.h"
#include <QThread>
#include <QDebug>
#include <QMutexLocker>
#include <QElapsedTimer>
#include <limits>
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
#include <QString>
#include<errno.h>

obj_thread::obj_thread(QObject *parent):QObject(parent)
{

}
obj_thread::~obj_thread()
{

}
//处理接收的数据
void obj_thread::dealMsg()
{
      int port = 7999;
      int sockfd;
      sockfd = socket(AF_INET, SOCK_DGRAM, 0);
      if(-1==sockfd){
      puts("Failed to create socket");
      }

      // 设置地址与端口
      struct sockaddr_in addr;
      socklen_t addr_len=sizeof(addr);
      memset(&addr, 0, sizeof(addr));
      addr.sin_family = AF_INET;       // Use IPV4
      addr.sin_port   = htons(port);    //
      addr.sin_addr.s_addr = htonl(INADDR_ANY);

      // Time out
      struct timeval tv;
      tv.tv_sec  = 0;
      tv.tv_usec = 2000000;  // 2000ms
      setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(struct timeval));

      // Bind 端口，用来接受之前设定的地址与端口发来的信息,作为接受一方必须bind端口，并且端口号与发送方一致
      if (bind(sockfd, (struct sockaddr*)&addr, addr_len) == -1){
          printf("Failed to bind socket on port %d\n", port);
          close(sockfd);
          //return false;
       }

       //memset(buffer, 0, 3);
       float buffer[3];
       int counter = 0;
       struct sockaddr_in src;
       socklen_t src_len = sizeof(src);
       memset(&src, 0, sizeof(src));

       int sz = recvfrom(sockfd, buffer, sizeof(buffer), 0, (sockaddr*)&src, &src_len);
       if (sz > 0){
            pos_z = buffer[2];
            pos_y = buffer[1];
            pos_x = buffer[0];
            //printf("Get Message %d: %f\n", counter++, pos_z);
            emit signal_show(pos_x,pos_y,pos_z);
            }
            else{
               puts("timeout");
       }
       close(sockfd);
}

void obj_thread::target_set(double num1, double num2, double num3)
{
        target[0]=num1;
        target[1]=num2;
        target[2]=num3;

        int port2 = 9999;
        int brdfd;
        // 创建socket
        brdfd = socket(PF_INET, SOCK_DGRAM, 0);
        if(-1==brdfd){
            puts("Failed to create socket");
        }

        // 设置地址与端口
        struct sockaddr_in addr2;
        socklen_t  addr_len2=sizeof(addr2);
        memset(&addr2, 0, sizeof(addr2));
        addr2.sin_family = AF_INET;
        addr2.sin_port   = htons(port2);
        addr2.sin_addr.s_addr = inet_addr("255.255.255");

        // Time out
        struct timeval tv;
        tv.tv_sec  = 0;
        tv.tv_usec = 200000;  // 200 ms
        setsockopt(brdfd, SOL_SOCKET, SO_BROADCAST | SO_REUSEADDR, (const char*)&tv, sizeof(struct timeval));

        /*if (bind(brdfd, (struct sockaddr*)&addr2, addr_len2) == -1){
            printf("Failed to bind socket on port %d\n", port2);
            close(brdfd);
        }*/

        if(sendto(brdfd, target, sizeof(target), 0, (sockaddr*)&addr2, addr_len2)==-1){
            printf("sendto fail, errno=%d\n", errno);
        }
        else{
            printf("Sended:%f\n",target[0]);
        }
        close(brdfd);
}
