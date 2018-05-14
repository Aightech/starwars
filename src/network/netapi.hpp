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
	/*! \class NetAPI
	* \brief This class is an API to interface different program through IP.
	*/
	public:

	/*! \brief Constructor */
	NetAPI();

	/*! \brief Destructor */
	~NetAPI()
	{
		for(int i=0; i<m_claddr.size();i++)
		m_claddr.pop_back();
	};

	/*! \brief Scans the network by attempting to connect to servers whose IP addresses ranged from IPmin to IPmax on port port */
	int scan(int port,int IPmin =0,int IPmax=255 );

	//------ SENDING METHODES ---------// 
	/*! \brief UDP Sending methodes */
	int sendUDP(int port, char * IP, char * buf);
	int sendUDP(struct sockaddr_in *, char * buf);

	int sendUDP(int port, char * IP, char * buf, char receivingBuf[]);
	int sendUDP(struct sockaddr_in *, char * buf, char receivingBuf[]);
	
	int sendToServerUDP(char * buf){return sendUDP(&m_Serveraddr,buf);};
	int sendToServerUDP(char * buf,char receivingBuf[]){return sendUDP(&m_Serveraddr,buf,receivingBuf);};

	int sendToClientUDP(int index, char * buf){
		if(m_claddr.size()>index)   return sendUDP(m_claddr[index],buf);
		else return -1;};
	int sendToClientUDP(int index, char * buf,char receivingBuf[]){
		if(m_claddr.size()>index)   return sendUDP(m_claddr[index],buf,receivingBuf);
		else return -1;};

	/*! \brief TCP Sending methodes */
	int sendTCP(int port, char * IP, char * buf);
	int sendTCP(struct sockaddr_in *, char * buf);

	int sendTCP(int port, char * IP, char * buf, char receivingBuf[]);
	int sendTCP(struct sockaddr_in *, char * buf, char receivingBuf[]);

	int sendToServerTCP(char * buf){return sendTCP(&m_Serveraddr,buf);};
	int sendToServerTCP(char * buf,char receivingBuf[]){return sendTCP(&m_Serveraddr,buf,receivingBuf);};

	int sendToClientTCP(int index, char * buf){
		if(m_claddr.size()>index)   return sendTCP(m_claddr[index],buf);
		else return -1;};
	int sendToClientTCP(int index, char * buf,char receivingBuf[]){
		if(m_claddr.size()>index)   return sendTCP(m_claddr[index],buf,receivingBuf);
		else return -1;};


	//--------------- SERVER METHODES ------------//
	/*! \brief Attempts to connect to the server. If the attempt is successful the address is set as the api server address. */
	int connectToServer(int port, char * IP);

	/*! \brief Launch a thread managing the reception of message from other netapi */
	int startReceiver(int port,char* protocol);

	/*! \brief vitual methode to implement in extented object in order to have a direct access to the message received by the receiver. The message need to start with a 'M' */
	virtual int processReceiverMessage(char * buffer, char * reply){strcpy(reply,"rcvd");};

	/*! \brief Store the last filled received buffer in the buffer passed in argument. and returns the number of unread buffer  */
	int getReceiverBuffer(char *buffer);

	/*! \brief Stops the Receiver thread */
	int endReceiver();

	/*! \brief Set the connection phrase use as a identification when trying to connect to other netapi */
	void setConnectionPhrase(char * conPhr);

	/*! \brief returns the list of aknowledged clients */
	const vector<struct sockaddr_in *>& getClientAddr() const;




	//----------- OTHERS -------------//
	/*! \brief Set the net api to printf info in the console */
	void verbose(){m_verbose=true;};
	void setConnectable(){m_connectable=true;};
	void unsetConnectable(){m_connectable=false;};

	protected:

	bool m_verbose;
	
	int say(char * str);

	int receiverTCP();
	int receiverUDP();

	//transmiting socket, address and its lenght
	int m_TxUDPfd;
	int m_TxTCPfd;
	struct sockaddr_in m_Txaddr;
	struct hostent *m_Txdest;
	socklen_t m_Txlen;

	struct sockaddr_in m_Serveraddr;

	//receiving socket, address and its lenght
	int m_Rxfd;
	struct sockaddr_in m_Rxaddr;
	socklen_t m_Rxlen;
	//Receiver thread
	thread* m_ReceiverThread;
	int m_Rxport;
	atomic<bool> m_ReceiverActive;
	int m_connectable=false;

	int m_RxBufferIndex;
	char m_RxBuffer[NB_BUFFERS][BUFSIZE];
	bool m_RxBufferFilled[NB_BUFFERS];

	//clients addresses and their lenght
	struct sockaddr_in m_clientaddr;
	vector<struct sockaddr_in *> m_claddr;
	int m_clientIndex;
	socklen_t m_cllen;

	char *m_connectionPhrase=NULL;

	mutex m_bufferMtx[NB_BUFFERS];
	mutex m_verboseMtx;

};




#endif /* guard */
