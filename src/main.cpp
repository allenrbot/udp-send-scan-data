#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#define SERVER_PORT 5000
#define BUFF_LEN 2048
//#define SERVER_IP "192.168.28.1"
#define SERVER_IP "210.72.140.22"
using namespace std;
struct Scan
{
    float ranges[720];
    float angle_min;
    float angle_max;
    float range_min;
    float range_max;
    float angle_increment;

};

void udp_msg_sender(int fd, struct sockaddr* dst)
{
    Scan scan;
    socklen_t len;
    ifstream inFile;
    inFile.open("scandata.txt");
    string lines;
    //char *cline;
    if (inFile)
    {
        while (!inFile.eof())
        {
            getline(inFile, lines);
            char seg[] = ",";
            char *substr = strtok((char*)lines.data(), seg);
            int m = 0;
            while (substr != NULL)
            {
                float data = atof(substr);
                if (m == 1)
                    scan.range_min = data;
                if (m == 2)
                    scan.range_max = 10;
                if (m == 3)
                    scan.angle_min = data;
                if (m == 4)
                    scan.angle_max = data;
                if (m == 5)
                    scan.angle_increment = data;

                substr = strtok(NULL, seg);
                m++;
            }

            getline(inFile, lines);
            char seg1[] = ",";
            char *substr1 = strtok((char*)lines.data(), seg1);
            int n = 0;
//            scan.ranges.clear();
//            scan.ranges.resize(720);
            while (substr1 != NULL)
            {

                float data = atof(substr1);
                scan.ranges[n] = data;
                substr1 = strtok(NULL, seg1);
                n++;
            }
            len = sizeof(*dst);
            cout<<scan.ranges[720]<<endl;
            char buf[BUFF_LEN] = "Send Scan Data!";
            printf("client:%s\n",buf);
            sendto(fd, (char*)&scan, sizeof(scan) + 1, 0, dst, len) ;
            usleep(50000);
           //sleep(1);
        }
    }
    /*
    while(1)
    {
        char buf[BUFF_LEN] = "Send Scan scan!";


        len = sizeof(*dst);
        printf("client:%s\n",buf);  //打印自己发送的信息
        sendto(fd, (char*)&scan, sizeof(scan) + 1, 0, dst, len) ;
        //sendto(fd, buf, BUFF_LEN, 0, dst, len);
//        memset(buf, 0, BUFF_LEN);
//        recvfrom(fd, buf, BUFF_LEN, 0, (struct sockaddr*)&src, &len);  //接收来自server的信息
//        printf("server:%s\n",buf);
        sleep(1);  //一秒发送一次消息
    }
    */
}

/*
    client:
            socket-->sendto-->revcfrom-->close
*/

int main()
{
    int client_fd;
    struct sockaddr_in ser_addr;

    client_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(client_fd < 0)
    {
        printf("create socket fail!\n");
        return -1;
    }

    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    //ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);  //注意网络序转换
    ser_addr.sin_port = htons(SERVER_PORT);  //注意网络序转换

    udp_msg_sender(client_fd, (struct sockaddr*)&ser_addr);

    //close(client_fd);

    return 0;
}

