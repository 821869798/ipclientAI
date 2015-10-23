#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>
#include<iostream>
#include<fstream>
#include"md5.h"
using namespace std;

#define SERVER_IP "172.16.12.11"

#define CONSTANT_1 0x0a
#define CONSTANT_2 0x0d10
#define CONSTANT_3 0x05
#define CONSTANT_4 0x05dc
#define CONSTANT_5 0x0f
#define CONSTANT_6 0x2382

unsigned char mac[6] = {0xb8,0x70,0xf4,0xff,0x6e,0xa6};

unsigned char temp1[] = {0x21,0x40,0x23,0x24,0x25,0x25,0x5e,0x26,0x2a,0x28,0x29};

unsigned char temp2[] = {0x71,0x77,0x65,0x72,0x74,0x79,0x75,0x39,0x30};

unsigned char temp3[] = {0x41,0x53,0x44,0x46,0x47,0x48};

unsigned char temp4[] = {0x39,0x67,0x64,0x74,0x34,0x33,0x37,0x34,0x35,0x77,0x72,0x77,0x71,0x72};

unsigned char temp5[] = {0x31,0x31,0x3a,0x32,0x32,0x3a,0x33,0x33,0x3a,0x34,0x34,0x3a,0x35,0x35,0x3a,0x36,0x36,0x2d,0x1f,0xd6,0x03,0xcc,0xf2,0x24};

unsigned char temp6[] = {0x71,0x77,0x65,0x72,0x74,0x79,0x75,0x69,0x6f,0x70};

unsigned char open_succeed[] = {0xbf,0xaa,0xb7,0xc5,0x49,0x50,0xb3,0xc9,0xb9,0xa6};


void ipclient_packet_init();
void add_data(void *,int );
void set_point(int );
void fill_with_0x00(int );
void reset();

//连接服务器函数
void build_0x05();

int recv_0x06();


//开放IP函数

void build_0x1f();
void recv_0x20();
void build_0x21();
int recv_0x22();


//心跳包函数
void build_0x1e();
double recv_0x1f();

int MD5(unsigned char *out,unsigned char *in);
int hex_to_str(char *str,char *hex,int len);
int generate_md5_string(char *str);

int strcpy_ex (void* dest ,void* src)
{
    if (dest== NULL || src == NULL)
    return -1 ;
    unsigned char* to = (unsigned char*) dest ;
    unsigned char* from = (unsigned char*) src ;
    while (*to++ = *from++) ;
    return 0 ;
}

struct send_data_t
{
    unsigned int data_len;
    char data[100];


};

 struct ipclient_packet_head_t
 {
     unsigned short sign;//ipclient标志
     unsigned char number;//数据包序号
     char unknow[8]; //似乎恒为0


 };

  struct ipclient_packet_t
  {
      char *current;
      char data[500];


  };

struct UserAndPass //保存账号密码
{
    string user;
    string pass;
};

char user_name[32];
char pass_word[32];

struct sockaddr_in client_addr1;
struct sockaddr_in client_addr2;

struct sockaddr_in server_addr1;
struct sockaddr_in server_addr2;

int client_socket_1;
int client_socket_2;

typedef struct sockaddr SA;
ipclient_packet_t packet;
ipclient_packet_head_t head;

pthread_t tid;

unsigned short key;

void conect_config()
{
    struct timeval tv;
    socklen_t addr_len = sizeof(SA);

    tv.tv_sec = 5;
    tv.tv_usec = 0;


    client_socket_1 = socket(AF_INET,SOCK_DGRAM,0);
    client_socket_2 = socket(AF_INET,SOCK_DGRAM,0);

    client_addr1.sin_family = AF_INET;
    client_addr1.sin_addr.s_addr = htons(INADDR_ANY);
    client_addr1.sin_port = htons(5200);

    server_addr1.sin_family = AF_INET;
    server_addr1.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr1.sin_port = htons(5300);

    client_addr2.sin_family = AF_INET;
    client_addr2.sin_addr.s_addr = htons(INADDR_ANY);
    client_addr2.sin_port = htons(5201);

    server_addr2.sin_family = AF_INET;
    server_addr2.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr2.sin_port = htons(5301);


    bind(client_socket_1,(SA *)&client_addr1,sizeof(SA));
    setsockopt(client_socket_1,SOL_SOCKET,SO_RCVTIMEO,(const char *)&tv,sizeof(struct timeval));

    bind(client_socket_2,(SA *)&client_addr2,sizeof(SA));
    setsockopt(client_socket_2,SOL_SOCKET,SO_RCVTIMEO,(const char *)&tv,sizeof(struct timeval));
}

void* heart(void *arg)
{
    pthread_detach(pthread_self());
    double money;
    char buf[100];
    while(1)
    {
        build_0x1e();
        sendto(client_socket_2,packet.data,500,0,(SA *)&server_addr2,sizeof(server_addr2));
        reset();
        recvfrom(client_socket_2,packet.data,sizeof(packet.data),0,NULL,NULL);
        money = recv_0x1f();
        memset(buf,0,sizeof(buf));
        sprintf(buf,"剩余%.3f元",money);
        cout<<buf<<"\n\n";
        sleep(40);
    }
}

int open_ip()
{
    int result = 0;
    if(user_name[0] == 0 || pass_word[0] == 0)
    {
        //create_message_dialog(GTK_MESSAGE_INFO,"用户或密码为空.");
        cout<<"null"<<endl;
        return 1;
    }
    build_0x1f();
    sendto(client_socket_1,packet.data,300,0,(SA *)&server_addr1,sizeof(server_addr1));

    reset();
    recvfrom(client_socket_1,packet.data,300,0,NULL,NULL);
    recv_0x20();

    build_0x21();
    sendto(client_socket_1,packet.data,300,0,(SA *)&server_addr1,sizeof(server_addr1));

    reset();
    recvfrom(client_socket_1,packet.data,300,0,NULL,NULL);
    result = recv_0x22();
    if(result == 0)
    {
        //create_message_dialog(GTK_MESSAGE_INFO,"开放失败.");
        cout<<"fail"<<endl;
        return 0;
    }
    else
    {
        cout<<"success"<<endl;
        int ret;
        if((ret = pthread_create(&tid,NULL,heart,NULL))!=0)
        {
            fprintf(stderr,"pthread_create:%s\n",strerror(ret));
            exit(EXIT_FAILURE);
        }
        /*
        if((ret = pthread_join(tid,NULL))!=0)
        {
        fprintf(stderr,"pthread_create:%s\n",strerror(ret));
        exit(EXIT_FAILURE);
    }*/
        return 1;
        //create_message_dialog(GTK_MESSAGE_INFO,"开放成功.");
    }
    //g_thread_create((GThreadFunc)heart, NULL, FALSE, NULL);
    return 0;
}
void readNumber()
{
    ifstream inUser("users.txt");
    ifstream inData("Start.ini");
    int index;
    inData>>index;
    inData.close();
    int c = 0;
    string user;
    string passw;
    while(true)
    {
        if(!(inUser>>user>>passw))
        {
            cout<<"没有账号可用了!"<<endl;
            inUser.close();
            break;
        }
        if(c++<index)
        continue;
        strcpy(user_name,user.c_str());
        strcpy(pass_word,passw.c_str());
        cout<<c<<": "<<user_name<<" "<<pass_word<<endl;
        int ok = open_ip();
        if(ok==1)
        {
            inUser.close();
            ofstream outData("Start.ini");
            outData<<c-1;
            outData.close();
            break;
        }
        //usleep(100000);
    }
}

void checkINI()
{
    time_t now;
    struct tm *fmt;
    time(&now);
    fmt = localtime(&now);
    int mon = ++fmt->tm_mon;
    ifstream inDate("Month.ini");
    if (!inDate)
    {
        inDate.close();
        ofstream outDate("Month.ini");
        outDate << mon;
        outDate.close();
    }
    else
    {
        int premon;
        inDate >> premon;
        premon;
        inDate.close();
        if (premon != mon)
        {
            ofstream outDate("Month.ini");
            outDate << mon;
            ofstream outA("Start.ini");
            outA.put('0');
            outDate.close();
            outA.close();
        }
    }
    ifstream isIn("Start.ini");
    if (!isIn)
    {
        ofstream out("Start.ini");
        out.put('0');
        out.close();
        isIn.close();
    }
    isIn.close();
}

bool isFirst = true;

int main()
{
    conect_config();
    checkINI();
    while(1)
    {
        int net = system("ping -c 2 8.8.8.8");
        cout<<net<<"\n\n";
        if(net)
        {
            if(isFirst==true)
            {
                isFirst=false;   
            }
            else
            {
                pthread_cancel(tid);
            }
            readNumber();
        }
        sleep(15);
    }
}


void build_0x1f()
{
    int i;

    reset();

    head.sign = htons(0x8223);
    head.number = 0x1f;
    memset(&head.unknow,0,8);
    add_data(&head,11);


    struct send_data_t send_data;
    send_data.data_len = strlen(user_name);  //0a
    send_data.data_len<<24;

    //每个用户名减去0x0a
    for(i = 0; i < strlen(user_name); i++)
    send_data.data[i] = user_name[i] - CONSTANT_1;

    add_data(&send_data,4+strlen(user_name));

    send_data.data_len = 0x0b;
    send_data.data_len<<24;
    strcpy_ex(send_data.data,temp1);
    add_data(&send_data,4+sizeof(temp1));

    send_data.data_len = 0x07;
    send_data.data_len<<24;
    strcpy_ex(send_data.data,temp2);
    add_data(&send_data,4+sizeof(temp2));
    packet.data[packet.current-&packet.data[0]] = 0x00;
    set_point(1);
    packet.data[packet.current-&packet.data[0]] = 0x00;
    set_point(1);


    send_data.data_len = 0x01;
    send_data.data_len<<24;
    add_data(&send_data,4);

    send_data.data_len = 0x06;
    send_data.data_len<<24;
    strcpy_ex(send_data.data,temp3);
    add_data(&send_data,4+sizeof(temp3));

    fill_with_0x00(300);
}

void ipclient_packet_init()
{
    memset(packet.data,0,500);
    packet.current = packet.data;
    open_ip();
}

void recv_0x20()
{
    char *p = packet.data;
    unsigned char ch;
    p += 51;

    key = *((unsigned short *)(p));

}

void build_0x21()
{
    int i;
    char md5[32];
    struct send_data_t send_data;

    reset();

    head.number = 0x21;
    memset(&head.unknow,0,8);
    add_data(&head,11);


    send_data.data_len = 0x0e;
    send_data.data_len<<24;
    strcpy_ex(send_data.data,temp4);
    add_data(&send_data,4 + sizeof(temp4));

    send_data.data_len = 0x1e;
    send_data.data_len<<24;
    add_data(&send_data,4);


    generate_md5_string(md5);//加密
    add_data(&md5,0x1e);

    send_data.data_len = 0x11;
    send_data.data_len<<24;
    strcpy_ex(send_data.data,temp5);
    add_data(&send_data,4 + sizeof(temp5));


    send_data.data_len = 0x0a;
    packet.data[packet.current-&packet.data[0]] = 0x00;
    set_point(1);
    send_data.data_len<<16;
    strcpy_ex(send_data.data,temp6);
    add_data(&send_data,4 + sizeof(temp6));


    fill_with_0x00(300);

}

int recv_0x22()
{
    char *p = packet.data;
    int i = 0;
    p += 0x02;
    if(0x22 != *p)
    {
        return 0;
    }
    p+=0x01;
    cout<<int(*p)<<endl;
    p+=0x08;
    unsigned char len;
    while (p <= &packet.data[299])
    {
        len = *p;
        p += 4;
        p += len;
        if (0x11 == len)
        break;
    }
    p += 0x0c;
    len = *p;
    p += 0x04;
    unsigned char Str[64] = {0};
    for (int i=0;i<len;i++,p++)
    {
        Str[i] = *p;
    }
    //cout<<Str<<endl;
    p = packet.data + 0x03;
    if(0x00!=*p)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

 void build_0x1e()
 {
     int i;
     unsigned short identity;
     double number;
     struct send_data_t send_data;
     unsigned char t7[] = {0xe4,0x3e,0x86};

     unsigned char t8[] = {0x5c,0x8f,0xc2,0xf5,0xf0,0xa9,0xdf,0x40};

     unsigned char t9[] = {0x31,0x31,0x30,0x30,0x33,0x36,0x30,0x32,0x30,0x31};

     unsigned char t[] = {0x53,0x70,0x69,0x64,0x65,0x72,0x6d,0x61,0x6e};

     reset();

     head.sign = htons(0x8223);
     head.number = 0x1e;

     identity = key - CONSTANT_2 + CONSTANT_4;

     memcpy(head.unknow,&identity,2);
     memset(&head.unknow[2],0,6);
     add_data(&head,11);

     add_data(t7,3);

     send_data.data_len = 0x0a;
     send_data.data_len<<24;
     strcpy_ex(send_data.data,t8);
     add_data(&send_data,4 + sizeof(t8));

     send_data.data_len = 0x09;
     send_data.data_len<<24;
     strcpy_ex(send_data.data,t9);
     add_data(&send_data,4 + sizeof(t9));

     fill_with_0x00(500);

 }

double recv_0x1f()
{
    double flow = 0;
    double balance = 0;
    char *p = packet.data;

    p += 0x02;
    if(0x1f != *p)
    return 0.0;

    p += 0x09;
    memcpy(&flow,p,0x08);
    p += 0x08;
    memcpy(&balance,p,0x08);

    p += 0x08;

    return balance;
}

 void add_data(void *buffer,int len)
 {
     memcpy(packet.current,buffer,len);
     packet.current += len;

 }

  void set_point(int move)
  {
      packet.current += move;

  }

void fill_with_0x00(int n)
{
    while(&packet.data[n-1] != packet.current)
    {
        *packet.current = (char )0x00;
        packet.current ++;

    }

    *packet.current = (char)0x00;

}

 void reset()
 {
     memset(packet.data,0,500);
     packet.current = packet.data;

 }

int MD5(unsigned char *out,unsigned char *in)
{
    MD5_CTX mctx;
    MD5Init(&mctx);
    MD5Update(&mctx,in,strlen((char *)in));
    MD5Final(&mctx,out);

    return 0;
}

int hex_to_str(char *str,char *hex,int len)
{
    int i;

    for(i = 0; i < len; i++)
    sprintf(str + i*2,"%02X",(unsigned char)hex[i]);

    return 0;

}

int generate_md5_string(char *str)  //用户名和密码加密
//存于str中
{
    char sz_str[64];
    char sz_md5[64];

    memset(sz_str,0x00,sizeof(sz_str));
    sprintf(sz_str,"%d",key - CONSTANT_2);

    strcat(sz_str,pass_word);

    memset(sz_md5,0x00,sizeof(sz_md5));
    MD5((unsigned char *)sz_md5,(unsigned char *)sz_str);

    memset(sz_str,0x00,sizeof(sz_str));
    hex_to_str(sz_str,sz_md5,0x10);

    memset(sz_md5,0x00,sizeof(sz_md5));
    strcpy(sz_md5,sz_str);

    memset(sz_str,0x00,sizeof(sz_str));
    strncpy(sz_str,sz_md5,CONSTANT_3);
    strcat(sz_str,user_name);

    MD5((unsigned char *)sz_md5,(unsigned char *)sz_str);
    memset(sz_str,0x00,sizeof(sz_str));
    hex_to_str(sz_str,sz_md5,0x10);

    memcpy(str,sz_str,strlen(sz_str));

    return 0;
}
