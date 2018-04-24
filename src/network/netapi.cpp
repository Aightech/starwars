#include "netapi.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#include <fcntl.h>



NetAPI::NetAPI()
{
       /* socket: create the socket */
       m_Rxfd = socket(AF_INET, SOCK_DGRAM, 0);
       if (m_Rxfd < 0 && m_verbose) 
              printf("ERROR opening Rx socket\n");
              
       //by default the mode verbose in unset
       m_verbose = false;    
};

int NetAPI::scan(int port,int IPmin ,int IPmax )
{
       //TODO
}

int NetAPI::send(int port, char * hostname, char * buf)
{

       /* gethostbyname: get the server's DNS entry */
       m_Txdest = gethostbyname(hostname);
       if (m_Txdest == NULL && m_verbose)
              printf("Tx:ERROR, no such host as %s\n", hostname);
       

       /* build the server's Internet address */
       bzero((char *) &m_Txaddr, sizeof(m_Txaddr));
       m_Txaddr.sin_family = AF_INET;
       bcopy((char *)m_Txdest->h_addr, (char *)&m_Txaddr.sin_addr.s_addr, m_Txdest->h_length);
       m_Txaddr.sin_port = htons(port);
       
       //use the basic send methode
       return this->send(&m_Txaddr,buf);
}


int NetAPI::send(struct sockaddr_in * addr, char * buf)
{

       /* send the message to the server */
       m_Txlen = sizeof(*addr);
       int n = sendto(m_Txfd, buf, strlen(buf), MSG_DONTWAIT, (struct sockaddr *)addr, m_Txlen);
       
       if (n < 0 && m_verbose) 
              printf("Tx:ERROR in sendto\n");
       if(m_verbose)
              printf("Tx:Sent [%s], to %s:%d\n", buf, inet_ntoa(addr->sin_addr), ntohs(addr->sin_port));
       
       
       return n;
}

int NetAPI::send(int port, char * hostname, char * buf, char recvBuff[])
{
       //use the basic sending methode
       int n = this->send(port,hostname,buf);
       
       usleep(0.1);
       /* get the server's reply . WARNING: the function is blocking if the socket is set to be so */
       int m = recvfrom(m_Txfd, recvBuff, BUFSIZE, 0, (struct sockaddr*)&m_Txaddr, &m_Txlen);
       
       if(m_verbose)
       {
              if (m < 0) 
                     printf("Tx:ERROR in recvfrom\n");
              else
                     printf("Tx:data from server: [%s]\n", recvBuff);
       }
       return m;
}



int NetAPI::send(struct sockaddr_in * addr, char * buf, char recvBuff[])
{
       //use the basic sending method
       int n = this->send(addr,buf);
       usleep(0.1);
       /* get the server's reply */
       int m = recvfrom(m_Txfd, recvBuff, BUFSIZE, 0, (struct sockaddr*)addr, &m_Txlen);
       
       if(m_verbose)
       {
              if (m < 0) 
                     printf("Tx:ERROR in recvfrom\n");
              else
                     printf("Tx:data from server: [%s]\n", recvBuff);
       }
       return m;
}

int NetAPI::connectToServer(int port, char * IP)
{
       char request[30];
       sprintf(request,"C%dP%s",m_Rxport,m_connectionPhrase);
       char reply[50];
       
       // Set the socket as non-blocking 
       long arg = fcntl(m_Txfd, F_GETFL, NULL); 
       arg |= O_NONBLOCK; 
       fcntl(m_Txfd, F_SETFL, arg);
       
       int n;
       if ((n = this->send(port,IP,(char *)request,reply)) < 0)
       { 
              //the socket is not blocking so it doesn't have the time to read the reply of the buffer
              if (errno == EWOULDBLOCK)
              {
	              printf("General:Connection in progress... : \t");
	              int valopt; 
                     long arg; 
                     fd_set myset; 
                     struct timeval tv; 
                     socklen_t lon;
                     
	              tv.tv_sec = 0; 
                     tv.tv_usec = 10000; 
                     FD_ZERO(&myset); 
                     FD_SET(m_Txfd, &myset); 
                     if (select(m_Txfd+1, NULL, &myset, NULL, &tv) > 0) //if the timeout is respected
                     { 
                            lon = sizeof(int); 
                            getsockopt(m_Txfd, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon); 
                            if(valopt==0)//if the server exists
                            { 
                                   printf("Connection established\n");
                                   
                                   //set back to blocking socket
                                   arg = fcntl(m_Txfd, F_GETFL, NULL); 
                                   arg &= (~O_NONBLOCK); 
                                   fcntl(m_Txfd, F_SETFL, arg);
                                    
                                   recvfrom(m_Txfd, reply, BUFSIZE, 0, (struct sockaddr*)&m_Txaddr, &m_Txlen);
                                   
                            }
                            else//if the connection failed
                                   printf("Connection failed.\n");  
                            
                     }
                     else
                         printf("Timeout reached.\n");     
              }
		
       }
       
       if(strcmp(reply,"accepted")==0)//if the server accepted the connection request by replying "accepted"
       {
              /* build the server's Internet address */
              m_Txdest = gethostbyname(IP);
              bzero((char *) &m_Serveraddr, sizeof(m_Serveraddr));
              m_Serveraddr.sin_family = AF_INET;
              bcopy((char *)m_Txdest->h_addr, (char *)&m_Serveraddr.sin_addr.s_addr, m_Txdest->h_length);
              m_Serveraddr.sin_port = htons(port);
              if(m_verbose)
                     printf("General:Connection success to %s:%d\n", inet_ntoa(m_Serveraddr.sin_addr), ntohs(m_Serveraddr.sin_port));
              return 1;
       }
       else
       {
              if(m_verbose)
                     printf("General:Connection fail to %s:%d\n", IP, port);
              return 0;
       }
       
       //set back to non bocking socket
       arg = fcntl(m_Txfd, F_GETFL, NULL); 
       arg &= (~O_NONBLOCK); 
       fcntl(m_Txfd, F_SETFL, arg);
       
}



int NetAPI::startReceiver(int port)
{
       m_Rxport = port;
       
       /* 
       * socket: create the parent socket 
       */
       m_Txfd = socket(AF_INET, SOCK_DGRAM, 0);
       if (m_Txfd < 0 && m_verbose) 
              printf("ERROR opening Tx socket\n");
              
       //start the receiver thread
       m_ReceiverThread = new std::thread(&NetAPI::receiver, this);
       
       
}

int NetAPI::receiver()
{
       /* setsockopt: Handy debugging trick that lets 
       * us rerun the server immediately after we kill it; 
       * otherwise we have to wait about 20 secs. 
       * Eliminates "ERROR on binding: Address already in use" error. 
       */
       int optval = 1;/* flag value for setsockopt */
       setsockopt(m_Rxfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int));

       /*
       * build the server's Internet address
       */
       bzero((char *) &m_Rxaddr, sizeof(m_Rxaddr));
       m_Rxaddr.sin_family = AF_INET;
       m_Rxaddr.sin_addr.s_addr = htonl(INADDR_ANY);
       m_Rxaddr.sin_port = htons((unsigned short)m_Rxport);

       /* 
       * bind: associate the parent socket with a port 
       */
       if(bind(m_Rxfd, (struct sockaddr *) &m_Rxaddr, sizeof(m_Rxaddr)) < 0 && m_verbose) 
              printf("Rx:ERROR on binding\n");

       /* 
       * main loop: wait for a datagram, then echo it
       */
       m_claddr.push_back(new struct sockaddr_in);
       m_cllen = sizeof(&m_claddr[0]);
       
       m_ReceiverActive = true;
       if(m_verbose)
              printf("Rx: Receiver started listening on port %d:\n", m_Rxport);

       m_RxBufferIndex = -1;
       m_clientIndex =0;
       
       while (m_ReceiverActive) 
       {

              if(m_RxBufferIndex < NB_BUFFERS)
              {
                     m_bufferMtx[++m_RxBufferIndex].lock();//lock the mutex of the new buffer
                     /*
                     * recvfrom: receive a UDP datagram from a client
                     */
                     bzero(m_RxBuffer[m_RxBufferIndex], BUFSIZE);
                     int n = recvfrom(m_Rxfd, m_RxBuffer[m_RxBufferIndex], BUFSIZE,MSG_WAITALL,(struct sockaddr *) m_claddr[m_clientIndex], &m_cllen);
                     
                     if (n < 0 && m_verbose)
                            printf("Rx:ERROR in recvfrom\n");
                     if(m_verbose)
                            printf("Rx:Received packet from %s:%d\nRx:Buffer[%d]: [%s]\n\n", inet_ntoa(m_claddr[m_clientIndex]->sin_addr), ntohs(m_claddr[m_clientIndex]->sin_port),m_RxBufferIndex,m_RxBuffer[m_RxBufferIndex]);
                     
                     char reply[50]="nope";
                     
                     int current = m_clientIndex;
                     int port;
                     switch(m_RxBuffer[m_RxBufferIndex][0])
                     {
                            case 'M'://message
                            {
                                   processReceiverMessage(m_RxBuffer[m_RxBufferIndex]);
                                   m_bufferMtx[m_RxBufferIndex].unlock();
                                   strcpy(reply,"rcvd");
                            }break;
                            case 'C'://connection attempt :  C[portno]P[connectionPhrase]
                            {
                                   if(m_connectable)//if the connections are allowed
                                   {
                                          port = atoi(m_RxBuffer[m_RxBufferIndex]+1);
                                          if(m_connectionPhrase != NULL && strcmp(strchr(m_RxBuffer[m_RxBufferIndex],'P')+1,m_connectionPhrase)==0)
                                          {
                                                 if(m_clientIndex<NB_MAX_CLIENT)
                                                 {
                                                        strcpy(reply,"accepted");
                                                        if(m_verbose) 
                                                               printf("Rx:Client %s:%d accepted.\n", inet_ntoa(m_claddr[m_clientIndex]->sin_addr),port);
                                                        
                                                        
                                                        
                                                        m_claddr.push_back(new struct sockaddr_in);
                                                        m_clientIndex++;
                                                 }
                                                 else
                                                 {
                                                        strcpy(reply,"full");
                                                 }
                                                 
                                          }
                                          else
                                          {
                                                 if(m_connectionPhrase == NULL)
                                                        printf("Rx:Connection phrase need to be set.\n");
                                                 else
                                                        printf("Rx:Connection phrase incorect\n"); 
                                          }
                                   }
                                   else if(m_verbose)
                                          printf("Rx:Api is not connectable. \n"); 
                                   m_bufferMtx[m_RxBufferIndex--].unlock();
                                          
                            }break;
                            default:
                                   m_bufferMtx[m_RxBufferIndex--].unlock();
                            
                            
                     
                     }
                     
                     
                     /*    
                     * sendto: echo the input back to the client 
                     */
                     n = sendto(m_Rxfd, reply, sizeof(reply), 0, (struct sockaddr *) m_claddr[current], m_cllen);
                     if (n < 0 && m_verbose) 
                            printf("Rx:ERROR in sendto\n");
                     else if(m_verbose)
                            printf("Rx:Replied to %s: [%s]\n", inet_ntoa(m_claddr[current]->sin_addr),reply);
                            
                     if(current!=m_clientIndex)
                            m_claddr[current]->sin_port = htons(port);
                     
                    
              }
       }
       if(m_verbose)
              printf("Rx:Receiver stoped listening port %d\n", m_Rxport);
              
       return 0;

}

int NetAPI::endReceiver()
{
       m_ReceiverActive = false;//end the loop of the receiver
       send(m_Rxport,(char *)"127.0.0.1",(char *)"halt");//send a stopping message to it 
       if(m_ReceiverThread->joinable())//wait for it
              m_ReceiverThread->join();
}

int NetAPI::getReceiverBuffer(char *buffer)
{
       if(m_RxBufferIndex>-1)//if some buffer is unread
       {
              m_bufferMtx[m_RxBufferIndex].lock();//mutex to ensure the well being of the data
              printf("test %s\n",m_RxBuffer[m_RxBufferIndex]);//copy the buffer
              strcpy(buffer,m_RxBuffer[m_RxBufferIndex]);
              m_bufferMtx[m_RxBufferIndex--].unlock();
              return m_RxBufferIndex+1;
       }
       return -1;
}

void NetAPI::setConnectionPhrase(char * conPhr)
{
       if(m_connectionPhrase != NULL)  delete m_connectionPhrase;
       m_connectionPhrase = new char[strlen(conPhr)+1];
       strcpy(m_connectionPhrase,conPhr);
}

vector<struct sockaddr_in *> NetAPI::getClientAddr()
{
       printf("General:Client Listing\n");
       for(int i=0; i<m_claddr.size()-1;i++)
              printf("General:Client n°%d : %s:%d \n",i,  inet_ntoa(m_claddr[i]->sin_addr), ntohs(m_claddr[i]->sin_port)); 
       return   m_claddr;
}
