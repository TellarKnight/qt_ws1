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

int main(){

    int port = 7999;
    int sockfd;

    // 创建socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(-1==sockfd){
        return false;
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
        return false;
    }

    float buffer[3];
    //memset(buffer, 0, 3);
    //float pos[3];
    int counter = 0;
    while(1){
        struct sockaddr_in src;
        socklen_t src_len = sizeof(src);
        memset(&src, 0, sizeof(src));

        int sz = recvfrom(sockfd, buffer, sizeof(buffer), 0, (sockaddr*)&src, &src_len);
        if (sz > 0){
            /*for (int i = 0; i < 3; ++i) {
                pos[i]=ntohl(buffer[i]) ;
            }*/
            printf("Get Message %d: %f\n", counter++, buffer[2]);
        }
        else{
            puts("timeout");
        }
    }
    close(sockfd);
    return 0;
}

