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

int waitSec(float sec)
{
	clock_t start=clock();
	while(clock()-start < sec*CLOCKS_PER_SEC){}
}



NetAPI::NetAPI()
{

	for(int i =0; i<NB_BUFFERS; i++)
		m_RxBufferFilled[i]=false;
	//by default the mode verbose in unset
	m_verbose = false;    
};

int NetAPI::scan(int port,int IPmin ,int IPmax )
{
	//TODO
		// Set the socket as non-blocking 
	/*long arg = fcntl(m_Txfd, F_GETFL, NULL); 
	arg |= O_NONBLOCK; 
	fcntl(m_TxTCPfd, F_SETFL, arg);*/
	//set back to non bocking socket
	/*arg = fcntl(m_Txfd, F_GETFL, NULL); 
	arg &= (~O_NONBLOCK); 
	fcntl(m_Txfd, F_SETFL, arg);*/

		/*if ((n = this->sendTCP(port,IP,(char *)request,reply)) < 0)
	{ 
		//the socket is not blocking so it doesn't have the time to read the reply of the buffer
		if (errno == EWOULDBLOCK)
		{
			m_verboseMtx.lock();
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

					read(m_Txfd, reply, BUFSIZE);

				}
				else//if the connection failed
					printf("Connection failed.\n");

			}
			else
				printf("Timeout reached.\n"); 
			m_verboseMtx.unlock();
		}

	}*/
}

int NetAPI::sendUDP(int port, char * hostname, char * buf)
{
	/* gethostbyname: get the server's DNS entry */
	m_Txdest = gethostbyname(hostname);
	if (m_Txdest == NULL && m_verbose)
	{
		m_verboseMtx.lock();
		printf("Tx:ERROR, no such host as %s\n", hostname);
		m_verboseMtx.unlock();
		return -1;
	}
	/* build the server's Internet address */
	bzero((char *) &m_Txaddr, sizeof(m_Txaddr));
	m_Txaddr.sin_family = AF_INET;
	bcopy((char *)m_Txdest->h_addr, (char *)&m_Txaddr.sin_addr.s_addr, m_Txdest->h_length);
	m_Txaddr.sin_port = htons(port);
	//use the basic send methode
	return this->sendUDP(&m_Txaddr,buf);
}


int NetAPI::sendUDP(struct sockaddr_in * addr, char * buf)
{
	m_TxUDPfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (m_TxUDPfd < 0 && m_verbose) 
	{
		m_verboseMtx.lock();
		printf("ERROR opening Tx socket\n");
		m_verboseMtx.unlock();
	}
	/* send the message to the server */
	m_Txlen = sizeof(*addr);
	int n = sendto(m_TxUDPfd, buf, strlen(buf), MSG_DONTWAIT, (struct sockaddr *)addr, m_Txlen);
	m_verboseMtx.lock();
	if (n < 0 && m_verbose) 
		printf("Tx:ERROR in sendto\n");
	if(m_verbose)
		printf("Tx:Sent [%s], to %s:%d\n", buf, inet_ntoa(addr->sin_addr), ntohs(addr->sin_port));
	m_verboseMtx.unlock();
	close(m_TxUDPfd);
	return n;
}

int NetAPI::sendUDP(int port, char * hostname, char * buf, char recvBuff[])
{
	/* gethostbyname: get the server's DNS entry */
	m_Txdest = gethostbyname(hostname);
	if (m_Txdest == NULL && m_verbose)
	{
		m_verboseMtx.lock();
		printf("Tx:ERROR, no such host as %s\n", hostname);
		m_verboseMtx.unlock();
		return -1;
	}
	/* build the server's Internet address */
	bzero((char *) &m_Txaddr, sizeof(m_Txaddr));
	m_Txaddr.sin_family = AF_INET;
	bcopy((char *)m_Txdest->h_addr, (char *)&m_Txaddr.sin_addr.s_addr, m_Txdest->h_length);
	m_Txaddr.sin_port = htons(port);
	//use the basic send methode
	return this->sendUDP(&m_Txaddr,buf,recvBuff);
}



int NetAPI::sendUDP(struct sockaddr_in * addr, char * buf, char recvBuff[])
{
	m_TxUDPfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (m_TxUDPfd < 0 && m_verbose) 
	{
		m_verboseMtx.lock();
		printf("ERROR opening Tx socket\n");
		m_verboseMtx.unlock();
	}
	/* send the message to the server */
	m_Txlen = sizeof(*addr);
	int n = sendto(m_TxUDPfd, buf, strlen(buf), MSG_DONTWAIT, (struct sockaddr *)addr, m_Txlen);
	m_verboseMtx.lock();
	if (n < 0 && m_verbose) 
		printf("Tx:ERROR in sendto\n");
	if(m_verbose)
		printf("Tx:Sent [%s], to %s:%d\n", buf, inet_ntoa(addr->sin_addr), ntohs(addr->sin_port));
	m_verboseMtx.unlock();
	usleep(0.1);
	/* get the server's reply */
	int m = recvfrom(m_TxUDPfd, recvBuff, BUFSIZE, 0, (struct sockaddr*)addr, &m_Txlen);
	if(m_verbose)
	{
		m_verboseMtx.lock();
		if (m < 0) 
			printf("Tx:ERROR in recvfrom\n");
		else
			printf("Tx:data from server: [%s]\n", recvBuff);
		m_verboseMtx.unlock();
	}
	close(m_TxUDPfd);
	return m;
}

int NetAPI::sendTCP(int port, char * hostname, char * buf)
{
	/* gethostbyname: get the server's DNS entry */
	m_Txdest = gethostbyname(hostname);
	if (m_Txdest == NULL && m_verbose)
	{
		m_verboseMtx.lock();
		printf("Tx:ERROR, no such host as %s\n", hostname);
		m_verboseMtx.unlock();
		return -1;
	}
	/* build the server's Internet address */
	bzero((char *) &m_Txaddr, sizeof(m_Txaddr));
	m_Txaddr.sin_family = AF_INET;
	bcopy((char *)m_Txdest->h_addr, (char *)&m_Txaddr.sin_addr.s_addr, m_Txdest->h_length);
	m_Txaddr.sin_port = htons(port);
	//use the basic send methode
	return this->sendTCP(&m_Txaddr,buf);
}


int NetAPI::sendTCP(struct sockaddr_in * addr, char * buf)
{
	/* send the message to the server */
	m_TxTCPfd = socket(AF_INET, SOCK_STREAM, 0);
	if (m_TxTCPfd < 0 && m_verbose) 
	{
		m_verboseMtx.lock();
		printf("ERROR opening Tx socket\n");
		m_verboseMtx.unlock();
	}
	
	m_Txlen = sizeof(*addr);
	if (connect(m_TxTCPfd,(struct sockaddr *) addr,m_Txlen) < 0 && m_verbose) 
		printf("Tx:ERROR couldn't connect\n");
	
	int n = write(m_TxTCPfd, buf, strlen(buf));
	m_verboseMtx.lock();
	if (n < 0 && m_verbose) 
		printf("Tx:ERROR in sendto\n");
	if(m_verbose)
		printf("Tx:Sent [%s], to %s:%d\n", buf, inet_ntoa(addr->sin_addr), ntohs(addr->sin_port));
	m_verboseMtx.unlock();
	return n;
}

int NetAPI::sendTCP(int port, char * hostname, char * buf, char recvBuff[])
{
	//use the basic sending methode
	int n = this->sendTCP(port,hostname,buf);

	usleep(0.1);
	/* get the server's reply . WARNING: the function is blocking if the socket is set to be so */
	int m = read(m_TxTCPfd, recvBuff, BUFSIZE);
	close(m_TxTCPfd);
	if(m_verbose)
	{
		m_verboseMtx.lock();
		if (m < 0) 
			printf("Tx:ERROR in recvfrom\n");
		else
			printf("Tx:data from server: [%s]\n", recvBuff);
		m_verboseMtx.unlock();
	}
	return m;
}



int NetAPI::sendTCP(struct sockaddr_in * addr, char * buf, char recvBuff[])
{
	//use the basic sending method
	int n = this->sendTCP(addr,buf);
	usleep(0.1);
	/* get the server's reply */
	int m = read(m_TxTCPfd, recvBuff, BUFSIZE);
	close(m_TxTCPfd);
	if(m_verbose)
	{
		m_verboseMtx.lock();
		if (m < 0) 
			printf("Tx:ERROR in recvfrom\n");
		else
			printf("Tx:data from server: [%s]\n", recvBuff);
		m_verboseMtx.unlock();
	}
	return m;
}

int NetAPI::connectToServer(int port, char * IP)
{
	char request[30];
	sprintf(request,"C%dP%s",m_Rxport,m_connectionPhrase);
	char reply[50];

	int n = this->sendTCP(port,IP,(char *)request,reply);

	if(strcmp(reply,"accepted")==0)//if the server accepted the connection request by replying "accepted"
	{
		/* build the server's Internet address */
		m_Txdest = gethostbyname(IP);
		bzero((char *) &m_Serveraddr, sizeof(m_Serveraddr));
		m_Serveraddr.sin_family = AF_INET;
		bcopy((char *)m_Txdest->h_addr, (char *)&m_Serveraddr.sin_addr.s_addr, m_Txdest->h_length);
		m_Serveraddr.sin_port = htons(port);
		if(m_verbose)
		{
			m_verboseMtx.lock();
			printf("General:Connection success to %s:%d\n", inet_ntoa(m_Serveraddr.sin_addr), ntohs(m_Serveraddr.sin_port));
			m_verboseMtx.unlock();
		}
		return 1;
	}
	else
	{
		if(m_verbose)
		{
			m_verboseMtx.lock();
			printf("General:Connection fail to %s:%d\n", IP, port);
			m_verboseMtx.unlock();
		}
		return 0;
	}

}



int NetAPI::startReceiver(int port, char * protocol)
{
	m_Rxport = port;
	if (strcmp(protocol,"UDP")==0 || strcmp(protocol,"udp")==0)
	{
		m_Rxfd = socket(AF_INET, SOCK_DGRAM, 0);//socket: create the parent socket 
		if (m_Rxfd < 0 && m_verbose) 
		{
			m_verboseMtx.lock();
			printf("ERROR opening Rx socket\n");
			m_verboseMtx.unlock();
		}
		//start the receiver thread
		m_ReceiverThread = new std::thread(&NetAPI::receiverUDP, this);
	}
	else if (strcmp(protocol,"TCP")==0 || strcmp(protocol,"tcp")==0)
	{
		m_Rxfd = socket(AF_INET, SOCK_STREAM, 0);//socket: create the parent socket 
		if (m_Rxfd < 0 && m_verbose) 
		{
			m_verboseMtx.lock();
			printf("ERROR opening Rx socket\n");
			m_verboseMtx.unlock();
		}
		//start the receiver thread
		m_ReceiverThread = new std::thread(&NetAPI::receiverTCP, this);
	}
	else
	{
		if (m_verbose) 
		{
			m_verboseMtx.lock();
			printf("ERROR Protocol unknown\n");
			m_verboseMtx.unlock();
		}
	}

	waitSec(1.5);


}

int NetAPI::receiverUDP()
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
	{
		m_verboseMtx.lock();
		printf("Rx:ERROR on binding\n");
		m_verboseMtx.unlock();
	}

	/* 
	* main loop: wait for a datagram, then echo it
	*/
	m_cllen = sizeof(&m_clientaddr);

	m_ReceiverActive = true;
	if(m_verbose)
	{
		m_verboseMtx.lock();
		printf("Rx: Receiver UDP started listening on port %d:\n", m_Rxport);
		m_verboseMtx.unlock();
	}

	m_clientIndex =0;
	char tempBuf[BUFSIZE];
	int currentBuffer;
	while (m_ReceiverActive) 
	{
		currentBuffer=0;
		while(m_RxBufferFilled[currentBuffer] && ++currentBuffer < NB_BUFFERS){}
		if(currentBuffer < NB_BUFFERS)
		{
			m_bufferMtx[currentBuffer].lock();//lock the mutex of the new buffer
			/*
			* recvfrom: receive a UDP datagram from a client
			*/
			bzero(tempBuf, BUFSIZE);
			int n = recvfrom(m_Rxfd, tempBuf, BUFSIZE,MSG_WAITALL,(struct sockaddr *)& m_clientaddr, &m_cllen);

			m_verboseMtx.lock();
			if (n < 0 && m_verbose)
				printf("Rx:ERROR in recvfrom\n");
			if(m_verbose)
				printf("Rx:Received packet from %s:%d\nRx:Buffer temp: [%s]\n\n", inet_ntoa(m_clientaddr.sin_addr), ntohs(m_clientaddr.sin_port),tempBuf);
			m_verboseMtx.unlock();

			char reply[50]="nope";

			int port;
			switch(tempBuf[0])
			{
				case 'M'://message
				{
					strcpy(m_RxBuffer[currentBuffer],tempBuf);
					
					processReceiverMessage(tempBuf,reply);
					//printf("Rx:Buffer[%d]: [%s]\n\n",currentBuffer, m_RxBuffer[currentBuffer]);
					m_RxBufferFilled[currentBuffer]=true;
				}break;
				case 'C'://connection attempt :  C[portno]P[connectionPhrase]
				{
					if(m_connectable)//if the connections are allowed
					{
						port = atoi(tempBuf+1);
						if(m_connectionPhrase != NULL && strcmp(strchr(tempBuf,'P')+1,m_connectionPhrase)==0)
						{
							if(m_clientIndex<NB_MAX_CLIENT)
							{
								strcpy(reply,"accepted");

								m_claddr.push_back(new struct sockaddr_in);
								bzero((char *) m_claddr[m_clientIndex], sizeof(&m_claddr[m_clientIndex]));
								m_claddr[m_clientIndex]->sin_family = AF_INET;
								m_claddr[m_clientIndex]->sin_addr.s_addr = m_clientaddr.sin_addr.s_addr;
								m_claddr[m_clientIndex]->sin_port = htons((unsigned short)port);



								if(m_verbose) 
								{
									m_verboseMtx.lock();
									printf("Rx:Client %s:%d accepted.\n", inet_ntoa(m_claddr[m_clientIndex]->sin_addr),port);
									m_verboseMtx.unlock();
								}
								m_clientIndex++;


							}
							else
							{
								strcpy(reply,"full");
							}

						}
						else
						{
							m_verboseMtx.lock();
							if(m_connectionPhrase == NULL)
								printf("Rx:Connection phrase need to be set.\n");
							else
								printf("Rx:Connection phrase incorect\n"); 
							m_verboseMtx.unlock();
						}
					}
					else if(m_verbose)
					{
						m_verboseMtx.lock();
						printf("Rx:Api is not connectable. \n");
						m_verboseMtx.unlock(); 
					}

				}break;
				default:
				{

				}



			}

			m_bufferMtx[currentBuffer].unlock();

		}
	}
	
	if(m_verbose)
	{
		m_verboseMtx.lock();
		printf("Rx:Receiver stoped listening port %d\n", m_Rxport);
		m_verboseMtx.unlock();
	}

	return 0;

}

int NetAPI::receiverTCP()
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
	{
		m_verboseMtx.lock();
		printf("Rx:ERROR on binding\n");
		m_verboseMtx.unlock();
	}

	/* 
	* main loop: wait for a datagram, then echo it
	*/
	m_cllen = sizeof(m_clientaddr);
	listen(m_Rxfd,5);


	m_ReceiverActive = true;
	if(m_verbose)
	{
		m_verboseMtx.lock();
		printf("Rx: Receiver TCP started listening on port %d:\n", m_Rxport);
		m_verboseMtx.unlock();
	}

	
	m_clientIndex =0;
	char tempBuf[BUFSIZE];

	int newsocket;
	int currentBuffer;
	while (m_ReceiverActive) 
	{

		currentBuffer=0;
		while(m_RxBufferFilled[currentBuffer] && ++currentBuffer < NB_BUFFERS){}
		if(currentBuffer < NB_BUFFERS)
		{
			m_bufferMtx[currentBuffer].lock();//lock the mutex of the new buffer
			newsocket = accept(m_Rxfd,(struct sockaddr *)& m_clientaddr, &m_cllen);
			/*
			* recvfrom: receive a UDP datagram from a client
			*/
			bzero(tempBuf, BUFSIZE);
			int n = read(newsocket, tempBuf, BUFSIZE);

			m_verboseMtx.lock();
			if (n < 0 && m_verbose)
				printf("Rx:ERROR in recvfrom\n");
			if(m_verbose)
				printf("Rx:Received packet from %s:%d\nRx:Buffer temp: [%s]\n\n", inet_ntoa(m_clientaddr.sin_addr), ntohs(m_clientaddr.sin_port),tempBuf);
			m_verboseMtx.unlock();

			char reply[50]="nope";

			int port;
			switch(tempBuf[0])
			{
				case 'M'://message
				{
					
					strcpy(m_RxBuffer[currentBuffer],tempBuf);
					processReceiverMessage(tempBuf,reply);
					printf("Rx:Buffer[%d]: [%s]\n\n",currentBuffer, m_RxBuffer[currentBuffer]);
					m_RxBufferFilled[currentBuffer]=true;
					
				}break;
				case 'C'://connection attempt :  C[portno]P[connectionPhrase]
				{
					if(m_connectable)//if the connections are allowed
					{
						port = atoi(tempBuf+1);
						if(m_connectionPhrase != NULL && strcmp(strchr(tempBuf,'P')+1,m_connectionPhrase)==0)
						{
							if(m_clientIndex<NB_MAX_CLIENT)
							{
								strcpy(reply,"accepted");

								m_claddr.push_back(new struct sockaddr_in);
								bzero((char *) m_claddr[m_clientIndex], sizeof(&m_claddr[m_clientIndex]));
								m_claddr[m_clientIndex]->sin_family = AF_INET;
								m_claddr[m_clientIndex]->sin_addr.s_addr = m_clientaddr.sin_addr.s_addr;
								m_claddr[m_clientIndex]->sin_port = htons((unsigned short)port);



								if(m_verbose) 
								{
									m_verboseMtx.lock();
									printf("Rx:Client %s:%d accepted.\n", inet_ntoa(m_claddr[m_clientIndex]->sin_addr),port);
									m_verboseMtx.unlock();
								}
								m_clientIndex++;


							}
							else
							{
							strcpy(reply,"full");
							}

						}
						else
						{
							m_verboseMtx.lock();
							if(m_connectionPhrase == NULL)
								printf("Rx:Connection phrase need to be set.\n");
							else
								printf("Rx:Connection phrase incorect\n"); 
							m_verboseMtx.unlock();
						}
					}
					else if(m_verbose)
					{
						m_verboseMtx.lock();
						printf("Rx:Api is not connectable. \n");
						m_verboseMtx.unlock(); 
					}

				}break;
				default:
				{

				}



			}
			

			/*    
			* sendto: echo the input back to the client 
			*/
			n = write(newsocket, reply, sizeof(reply));
			m_verboseMtx.lock();
			if (n < 0 && m_verbose) 
				printf("Rx:ERROR in sendto\n");
			else if(m_verbose)
				printf("Rx:Replied to %s: [%s]\n", inet_ntoa(m_clientaddr.sin_addr),reply);
			m_verboseMtx.unlock();

			close(newsocket);
			m_bufferMtx[currentBuffer].unlock();//lock the mutex of the new buffer
		}
		
	}
	close(m_Rxfd);
	if(m_verbose)
	{
		m_verboseMtx.lock();
		printf("Rx:Receiver stoped listening port %d\n", m_Rxport);
		m_verboseMtx.unlock();
	}

	return 0;

}

int NetAPI::endReceiver()
{
	m_ReceiverActive = false;//end the loop of the receiver
	sendTCP(m_Rxport,(char *)"127.0.0.1",(char *)"halt");//send a stopping message to it 
	if(m_ReceiverThread->joinable())//wait for it
		m_ReceiverThread->join();
	return 1;
}

int NetAPI::getReceiverBuffer(char *buffer)
{
	int currentBuffer = 0;
	while(!m_RxBufferFilled[currentBuffer] && ++currentBuffer < NB_BUFFERS){}
	if(currentBuffer < NB_BUFFERS)//if some buffer is unread
	{
		m_bufferMtx[currentBuffer].lock();//mutex to ensure the well being of the data
		//printf("Buffer [%d]: %s  \n",currentBuffer,m_RxBuffer[currentBuffer]);
		strcpy(buffer,m_RxBuffer[currentBuffer]);//copy the buffer
		m_RxBufferFilled[currentBuffer]=false;
		m_bufferMtx[currentBuffer].unlock();
		return currentBuffer;
	}
	return -1;
}

void NetAPI::setConnectionPhrase(char * conPhr)
{
	if(m_connectionPhrase != NULL)	delete m_connectionPhrase;
	m_connectionPhrase = new char[strlen(conPhr)+1];
	strcpy(m_connectionPhrase,conPhr);
}

const vector<struct sockaddr_in *>& NetAPI::getClientAddr() const
{
	if(m_verbose)
	{
		(const_cast<NetAPI*> (this))->m_verboseMtx.lock();
		printf("General:Client Listing (%d clients)\n",(int)m_claddr.size());
		for(int i=0; i<m_claddr.size();i++)
		     printf("General:Client n°%d : %s:%d \n",i,  inet_ntoa(m_claddr[i]->sin_addr), ntohs(m_claddr[i]->sin_port));
		(const_cast<NetAPI*> (this))->m_verboseMtx.unlock();
	} 
	return   m_claddr;
}

int NetAPI::say(char * str)
{
	if(m_verbose)
	{
		m_verboseMtx.lock();
		printf("%s\n", str);
		m_verboseMtx.unlock();
	}
}
