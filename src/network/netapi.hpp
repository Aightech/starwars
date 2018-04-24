#ifndef NETAPI_H
#define NETAPI_H

#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

#include <vector>
#include <thread>
#include <atomic>
#include <mutex>

#define BUFSIZE 1024
#define NB_BUFFERS 4
#define NB_MAX_CLIENT 4

using namespace std;

class NetAPI
{
       public:
              
       NetAPI();
       
       ~NetAPI()
       {
              for(int i=0; i<m_claddr.size();i++)
                     m_claddr.pop_back();
       };
       
       int scan(int port,int IPmin =0,int IPmax=255 );
       
       //------ SENDING METHODES ---------// 
       int send(int port, char * IP, char * buf);
       int send(struct sockaddr_in *, char * buf);
       
       int send(int port, char * IP, char * buf, char receivingBuf[]);
       int send(struct sockaddr_in *, char * buf, char receivingBuf[]);
       
       int sentToServer(char * buf){return send(&m_Serveraddr,buf);};
       int sentToServer(char * buf,char receivingBuf[]){return send(&m_Serveraddr,buf,receivingBuf);};
       
       int sentToClient(int index, char * buf){
              if(m_claddr.size()>index)   return send(m_claddr[index],buf);
              else return -1;};
       int sentToClient(int index, char * buf,char receivingBuf[]){
              if(m_claddr.size()>index)   return send(m_claddr[index],buf,receivingBuf);
              else return -1;};
              
              
       //--------------- SERVER METHODES ------------//
       int connectToServer(int port, char * IP);
       
       int startReceiver(int port);
       
       virtual int processReceiverMessage(char * buffer){};
       
       int getReceiverBuffer(char *buffer);
       
       int endReceiver();
       
       void setConnectionPhrase(char * conPhr);
       
       vector<struct sockaddr_in *> getClientAddr();
       
       
       //----------- OTHERS -------------//
       void verbose(){m_verbose=true;};
       
       protected:
       
       bool m_verbose;
       
       int receiver();
       
       //transmiting socket, address and its lenght
       int m_Txfd;
       struct sockaddr_in m_Txaddr;
       struct hostent *m_Txdest;
       socklen_t m_Txlen;
       
       struct sockaddr_in m_Serveraddr;
       
       //receiving socket, address and its lenght
       int m_Rxfd;
       struct sockaddr_in m_Rxaddr;
       socklen_t m_Rxlen;
       //listener thread
       thread* m_ReceiverThread;
       int m_Rxport;
       atomic<bool> m_ReceiverActive;
       
       int m_RxBufferIndex;
       char m_RxBuffer[NB_BUFFERS][BUFSIZE];
       
       //clients addresses and their lenght
       vector<struct sockaddr_in *> m_claddr;
       int m_clientIndex;
       socklen_t m_cllen;
       
       char *m_connectionPhrase=NULL;
       
       mutex m_bufferMtx[NB_BUFFERS];

};


#endif /* guard */
