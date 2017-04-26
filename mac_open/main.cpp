#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

#define PORT 20015
#define SERVER_IP "172.16.1.1"

int* get_ip_array(char *ip)
{
    char * myip = (char *)malloc(sizeof(char)*(strlen(ip)+1));
    strcpy(myip,ip);
    int * ip_array = (int *)malloc(sizeof(int)*4);
    int i=0;
    char * p;
    p = strtok(myip,".");
    while(p!=NULL) {
        if(i==4)
            break;
        ip_array[i++] = atoi(p);
        p = strtok(NULL,".");
    }
    return ip_array;
}

//ip
//mac
//type

void open_mac(char *ip,char * myMac,int type)
{
    int i,s,len;
    struct sockaddr_in addr;
    socklen_t addr_len =sizeof(struct sockaddr_in);
    char buffer[256];
    /* 建立socket*/
    if((s = socket(AF_INET,SOCK_DGRAM,0))<0){
        perror("socket");
        exit(1);
    }
    bzero(&addr,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    unsigned char localInfo[]={0x00,0x00,0x00,0x00,0x00,0x00,
                               0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                               0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                               0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                               0xac,0x10,0x40,0x12,0x30,0x30,0x3a,0x31,
                               0x46,0x3a,0x31,0x36,0x3a,0x32,0x32,0x3a,
                               0x42,0x38,0x3a,0x45,0x43,0x00,0x00,0x00,
                               0x02,0x00};
    //tianjialeixing
    int nInfo=sizeof(localInfo);
    localInfo[nInfo-2]=(unsigned char)type;

    //mac

    int nMac=strlen(myMac);
    for (i=0;i<nMac;i++)
    {
        localInfo[i+34]=(unsigned char)myMac[i];
    }
    localInfo[0]='1';
    //ip

    int *ip_array = get_ip_array(ip);
    for (i=0;i<4;i++)
    {
        localInfo[i+30]=(unsigned char)ip_array[i];
    }
    /****************Calculating Key************/
    int ispKey=0x4e67c6a7;
    int ECX;
    int ESI;
    int EBX;
    int EAX;
    ECX=ispKey;
    for (i=0;i<nInfo;i++)
    {
        ESI=ECX;
        ESI=ESI<<5;
        if (ECX>0)
        {
            EBX=ECX;
            EBX=EBX>>2;
        }
        else
        {
            EBX=ECX;
            EBX=EBX>>2;
            EBX=EBX|(0xC0000000);
        }
        ESI=ESI+localInfo[i];
        EBX=EBX+ESI;
        ECX=ECX^EBX;
    }
    ECX=ECX&0x7FFFFFFF;
    unsigned char send_pack[60];
    for (i=0;i<56;i++)
        send_pack[i] = localInfo[i];
    for (i=0;i<4;i++)
    {
        unsigned char keypart;
        keypart=(unsigned char)(ECX>>(i*8))&0x000000FF;
        send_pack[56+i] = keypart;
    }

    len =sizeof(send_pack);
    len = sendto(s,send_pack,len,0,(struct sockaddr *)&addr,addr_len);
    printf("%d\n",len);
}

//使用命令行参数 第一个ip，第二个mac地址（:间隔），第三个类型（1 -> 联通，2 -> 电信, 3 -> 移动）
int main(int argc,char **argv)
{
    if(argc==4)
    {
        printf("ip: %s\n",argv[1]);
        printf("mac: %s\n",argv[2]);
        int type = argv[3][0] - '0';
        printf("type: %d\n",type);
        open_mac(argv[1],argv[2],type);
    }
    else
    {
        printf("参数错误 Parameter error\n");
    }

    return 0;
}
