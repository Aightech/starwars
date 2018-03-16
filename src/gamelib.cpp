
#include <iostream>
#include <string>

#include "netlib.hpp"
#include "GUI/guilib.hpp"
#include "gamelib.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

/*#include "vector"
#include <time.h> 


#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <sys/wait.h>

#include <netdb.h>
#include <arpa/inet.h>

#include <fcntl.h>

#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>*/




using namespace sf;
using namespace std;

int WATCHPORT;


Game::Game()
{

}


void Game::init()
{   
       //m_state=0;
}





void Game::start()
{
       
       
}

/*

void Game::startServer()
{
       // Receiving server variables - Initialization//
       serv_sfd = socket(AF_INET, SOCK_STREAM, 0);
       if (serv_sfd < 0) printf("ERROR opening socket\n");
       bzero((char *) &serv_addr, sizeof(serv_addr));
       serv_portNo = SERVERPORT;
       serv_addr.sin_family = AF_INET;
       serv_addr.sin_addr.s_addr = INADDR_ANY;
       serv_addr.sin_port = htons(serv_portNo);

       if (bind(serv_sfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
              printf("error(ERROR on binding\n");

       listen(serv_sfd,5);
       serv_clilen = sizeof(serv_clientAddr[0]);
       
       serv_clientPortNo[0]=m_buffer.R_port;
       sprintf(serv_clientIPAddr[0],"%s","127.0.0.1");
       
       printf("TCP IP Host-server sh13 started on Port: %d\n", serv_portNo);
       // ------------------------------------------ //
       
       while (1)
       {    
              serv_cpsfd = accept(serv_sfd, (struct sockaddr *) &serv_clientAddr[0], &serv_clilen);      
              if (serv_cpsfd < 0) 
              {
                     printf("S:error(ERROR on accepting)\n");
                     exit(0);
              }

              bzero(serv_buff.Rx,SIZE_BUFF);
              if(read(serv_cpsfd,serv_buff.Rx,SIZE_BUFF)>0)
              {
                     printf("\nS: ## -Start of communication- ##\n");
                     printf("S: Address [%s] on port %d :\n",inet_ntoa(serv_clientAddr[0].sin_addr), ntohs(serv_clientAddr[0].sin_port));
                     printf("S: Request:%s\n",serv_buff.Rx);//strchr(serv_RxBuff,'G')+1);
                     switch(atoi(strchr(serv_buff.Rx,'G')+1))
                     {
                            case 0://if a client ask if we are a sh13 server
                                   printf("S: Want to know if we were a sh13 server. \n");
                                   sprintf(serv_buff.Tx,"G%dP%dU%dN%s;",0,serv_portNo,m_nbPlayer,"XxpartyxX");
                                   write(serv_cpsfd,serv_buff.Tx,strlen(serv_buff.Tx));
                                   printf("S: replied : %s \n",serv_buff.Tx);
                            break;
                            
                            
                            case 1://Joining
                            {  
                                   char *name=strtok((strchr(serv_buff.Rx,'N')+1),";");    //get the name of the player
                                   int port = atoi(strchr(serv_buff.Rx,'P')+1);            //get the port of the player
                                   
                                   printf("S: %s want to join the game.\n",name);
                                   
                                   if(m_nbPlayer<NB_MAX_PLAYERS)
                                   {
                                          setPlayer(m_nbPlayer,name,inet_ntoa(serv_clientAddr[0].sin_addr),port);
                                          
                                          sprintf(serv_buff.Tx,"G%d",1);
                                          write(serv_cpsfd,serv_buff.Tx,strlen(serv_buff.Tx));
                                          printf("S: replied : %s \n",serv_buff.Tx);
                                          
                                          m_nbPlayer++;
                                          
                                          for(int i=0;i<m_nbPlayer;i++)
                                          {
                                                 for(int j=0;j<m_nbPlayer;j++)
                                                 {
                                                        sprintf(serv_buff.Tx,"G%dH%dP%dU%dN%s;I%s;",11,m_players[i].no,m_players[j].portNo,j,m_players[j].name,m_players[j].IPaddress);
                                                        printf("S: send to player %d: info player %d : %s\n",i,j,serv_buff.Tx);
                                                        serv_buff.T_flag=1;
                                                        sendTCP(m_players[i].IPaddress, m_players[i].portNo, &serv_buff);
                                                 }
                                          }
                                          
                                          
                                   }
                                   else
                                   {
                                          strcpy(serv_buff.Tx,"G-1");
                                          write(serv_cpsfd,serv_buff.Tx,strlen(serv_buff.Tx));
                                          printf("S: replied : %s \n",serv_buff.Tx);
                                   }
                            }
                            break;
                            
                            case 99://Killing
                            {      
                                   
                                   int U = atoi(strchr(serv_buff.Rx,'U')+1);            //get the player to kill
                                   printf("S: Killing : %d \n",U);
                                   if(U==0)
                                   {
                                          for(int i=1;i<m_nbPlayer;i++)
                                          { 
                                                 //say to client he s been kicked out
                                                 sprintf(serv_buff.Tx,"G%d",99);
                                                 serv_buff.T_flag=1;
                                                 sendTCP(m_players[i].IPaddress, m_players[i].portNo, &serv_buff);
                                          }
                                          exit(0);
                                   }
                                          
                                   //say to client he s been kicked out
                                   sprintf(serv_buff.Tx,"G%d",99);
                                   serv_buff.T_flag=1;
                                   sendTCP(m_players[U].IPaddress, m_players[U].portNo, &serv_buff);
                                   
                                   
                                   
                                   for(int i=U;i<m_nbPlayer-1;i++)
                                          setPlayer(m_players[i+1].no,m_players[i+1].name,m_players[i+1].IPaddress,m_players[i+1].portNo);
                                    
                                   m_nbPlayer--;
                                         
                                          
                                   for(int i=0;i<m_nbPlayer;i++)
                                   {
                                          for(int j=0;j<m_nbPlayer;j++)
                                          {
                                                 sprintf(serv_buff.Tx,"G%dP%dU%dN%s;I%s;",11,m_players[j].portNo,j,m_players[j].name,m_players[j].IPaddress);
                                                 printf("S: send to player %d: info player %d : %s\n",i,j,serv_buff.Tx);
                                                 serv_buff.T_flag=1;
                                                 sendTCP(m_players[i].IPaddress, m_players[i].portNo, &serv_buff);
                                          }
                                   }
                                          
                            }
                            break;
                            case 20:
                                   m_turn=0;
                                   shareObj();
                                   sprintf(serv_buff.Tx,"G20");
                                   printf("S: send to player %d: info player : %s\n",0,serv_buff.Tx);
                                   write(serv_cpsfd,serv_buff.Tx,strlen(serv_buff.Tx));
                                   
                                   for(int i=0;i<m_nbPlayer;i++)
                                   {
                                          sprintf(serv_buff.Tx,"G%dC",20);
                                          for(int j=0;j<12/m_nbPlayer;j++)
                                                sprintf(serv_buff.Tx,"%s%d;",serv_buff.Tx,m_players[i].card[j]);

                                          printf("S: send to player %d: info player : %s\n",i,serv_buff.Tx);
                                          serv_buff.T_flag=1;
                                          sendTCP(m_players[i].IPaddress, m_players[i].portNo, &serv_buff);
                                   }
                                   sprintf(serv_buff.Tx,"G%d",25);
                                   serv_buff.T_flag=1;
                                   sendTCP(m_players[0].IPaddress, m_players[0].portNo, &serv_buff);
                                   
                            break;
                            case 21://GENPOLL
                                   if(strcmp(m_players[m_turn].IPaddress,inet_ntoa(serv_clientAddr[0].sin_addr))==0)
                                   {
                                          int obj=atoi(strchr(serv_buff.Rx,'Q')+1);
                                          sprintf(serv_buff.Tx,"G%dQ%dO",21,obj);
                                          for(int i=0;i<m_nbPlayer;i++)
                                          {
                                                 int o=0;
                                                 for(int j=0;j<12/m_nbPlayer;j++)
                                                 {
                                                        for(int k=0;k<3;k++) 
                                                        {
                                                               if(m_charactersArray[m_players[i].card[j]][k]==obj)  
                                                                      o=1;  
                                                        }      
                                                 }
                                                 sprintf(serv_buff.Tx,"%s%d;",serv_buff.Tx,o);
                                          }

                                          printf("S: send to player %d: info player : %s\n",m_turn,serv_buff.Tx);
                                          serv_buff.T_flag=1;
                                          sendTCP(m_players[m_turn].IPaddress, m_players[m_turn].portNo, &serv_buff);
                                          m_turn=(m_turn+1)%m_nbPlayer;
                                          
                                          sprintf(serv_buff.Tx,"G%d",25);
                                          printf("S: send to player %d: info player : %s\n",m_turn,serv_buff.Tx);
                                          serv_buff.T_flag=1;
                                          sendTCP(m_players[m_turn].IPaddress, m_players[m_turn].portNo, &serv_buff);
                                   }
                                   
                            case 22://INQUIRY
                                   if(strcmp(m_players[m_turn].IPaddress,inet_ntoa(serv_clientAddr[0].sin_addr))==0)
                                   {
                                          int obj=atoi(strchr(serv_buff.Rx,'Q')+1);
                                          int p=atoi(strchr(serv_buff.Rx,'P')+1);
                                          
                                          int o=0;
                                          for(int j=0;j<12/m_nbPlayer;j++)
                                          {
                                                 for(int k=0;k<3;k++) 
                                                 {
                                                        if(m_charactersArray[m_players[p].card[j]][k]==obj)  
                                                               o++;  
                                                 }      
                                          }
                                          sprintf(serv_buff.Tx,"G%dP%dO%d",21,p,o);
                                          

                                          printf("S: send to player %d: info player : %s\n",m_turn,serv_buff.Tx);
                                          serv_buff.T_flag=1;
                                          
                                          sendTCP(m_players[m_turn].IPaddress, m_players[m_turn].portNo, &serv_buff);
                                          m_turn=(m_turn+1)%m_nbPlayer;
                                   }
                                   
                                   
                            break;
                            case 23://DENOUNCE
                                   if(strcmp(m_players[m_turn].IPaddress,inet_ntoa(serv_clientAddr[0].sin_addr))==0)
                                   {
                                          int c=atoi(strchr(serv_buff.Rx,'C')+1);
                                          if(c==m_culprit)
                                          {
                                                 //win
                                          }
                                          else
                                          {
                                                 //lose
                                          }
                                   }
                            break;
                     }
                     
              printf("S: ## - End of communication - ##\n\n");
              }
              close(serv_cpsfd);
              
                     
     	}
     
     close(serv_sfd);
     
}

void * Game::tcpWatchdog(void * p_data)
{
       // Receiving server variables - Declaration//
       Buffer * buff= (Buffer *) p_data;
       int _sfd;
       int _cpsfd;
       struct sockaddr_in _addr;
       struct sockaddr_in _clientAddr;
       socklen_t _clilen;
       // --------------------------- //
       
       
       // Receiving server variables - Initialization //
       _sfd = socket(AF_INET, SOCK_STREAM, 0);
       if (_sfd < 0) printf("ERROR opening socket\n");
       
       bzero((char *) &_addr, sizeof(_addr));
       _addr.sin_family = AF_INET;
       _addr.sin_addr.s_addr = INADDR_ANY;
       _addr.sin_port = htons(buff->R_port);

       if (bind(_sfd, (struct sockaddr *) &_addr,sizeof(_addr)) < 0) 
              printf("watch: error(ERROR on binding)\n");
              
       printf("TCP IP thread started on Port: %d\n", buff->R_port);

       listen(_sfd,5);
       _clilen = sizeof(_clientAddr);
       buff->R_flag=0;
       // --------------------------------------------- //
       
       
       while (1)
       {    
              if(!buff->R_flag)//if the last data received has been process
              { 
                     
                     _cpsfd = accept(_sfd, (struct sockaddr *) &_clientAddr, &_clilen); //wait for a connection
                     if (_cpsfd < 0) //if the connection went wrong.
                     {
                            printf("W: error(ERROR on binding)\n");
                            exit(0);
                     }
                     
                     bzero(buff->Rx,SIZE_BUFF);
                     if(read(_cpsfd,buff->Rx,SIZE_BUFF)>0)
                     {      
                            printf("\nW: ## -Start of communication- ##\n");
                            printf("W: Address [%s] on port %d :\n",inet_ntoa(_clientAddr.sin_addr), ntohs(_clientAddr.sin_port));
                            printf("W: Request:%s\n",buff->Rx);
                            buff->R_flag=1;
                            printf("W: ## - End of communication - ##\n\n");
                     }
                     else
                            printf("W: Nothing was received.\n\n");
                     
                     
                     
                     close(_cpsfd);
              }
       }
 
       return NULL;
}

int Game::setPlayer(int no, char * name, char * IPaddr, int port)
{
       if(no<NB_MAX_PLAYERS)
       {
              m_players[no].no=no;
              strcpy(m_players[no].name,name);
              strcpy(m_players[no].IPaddress,IPaddr);
              m_players[no].portNo=port;
              return 1;
       }
       else 
              return -1;
}

int Game::processBuffer()
{
       
       if(m_buffer.R_flag==1)//double check if new stuff is to read 
       {
              printf("G: Buffer: %s \n",m_buffer.Rx);
              switch(atoi(strchr(m_buffer.Rx,'G')+1))
              {
                     case 0:
                            printf("G: Nothing to do \n");
                     break;
                     case 99:
                     {
                            printf("G: You've been banned from the game'\n");
                            m_state=MAIN_MENU;
                            createMenu();
                                
                     break;
                     
                     case 11:
                            printf("G: buff: %s \n",m_buffer.Rx);
                            
                            m_myNo=atoi(strchr(m_buffer.Rx,'H')+1);
                            printf("G: my no:%d\n",m_myNo);
                            
                            int no        = atoi(strchr(m_buffer.Rx,'U')+1);
                            printf("G: no:%d\n",no);
                            char *IP      = strtok((strchr(m_buffer.Rx,'I')+1),";");
                            printf("G: IP:%s\n",IP);
                            char *name    = strtok((strchr(m_buffer.Rx,'N')+1),";");
                            printf("G: name:%s\n",name);
                            int port      = atoi(strchr(m_buffer.Rx,'P')+1);
                            printf("G: port:%d\n",port);
                            setPlayer(no, name, IP, port);
                            
                            
                            
                            m_nbPlayer= no+1 ;
                            
                            createMenu();
                     }
                     break;
                     
                     case 20:
                     {
                            printf("G: get my card\n");
                            m_state=GAME_ONLINE;
                            char *ptr=strchr(m_buffer.Rx,'C')+1;
                            for(int i=0;i<12/m_nbPlayer;i++)
                            {
                                   printf("G: %d / i=%d / %d\n",m_myNo,i,atoi(ptr));
                                   m_players[m_myNo].card[i]=atoi(ptr);
                                   printf("G: %d\n",m_players[m_myNo].card[i]);
                                   ptr=strchr(ptr,';')+1;
                            }
                            m_turn=-1;
                     
                     }  
                     break;
                     
                    
                     
                     case 21://GENPOLL reply
                     {
                            int obj       = atoi(strchr(m_buffer.Rx,'Q')+1);
                            char *ptr=strchr(m_buffer.Rx,'O')+1;
                            
                            for(int i=0;i<m_nbPlayer;i++)
                            {
                                   String text= m_playersObjects[i]->getString();
                                   text[2*i]='?';
                                   m_players[m_myNo].card[i]=atoi(ptr);
                                   m_playersObjects[i]->setString(text);
                                   ptr=strchr(ptr,';')+1;
                            }
                            m_turn=-1;
                     }
                     break;
                     
                     case 23:
                            
                     break;
                     
                      case 25://my turn
                     
                            m_turn=m_myNo;
                            printf("G: My turn\n");//my turn
                            m_state=MAIN_GAME;
                            createGameContext();           
                     break;
              }
              printf("01\n");
              m_buffer.R_flag=0;
       }
}


void Game::shareObj()
{
       int obj,objArray[13],nbo=1;
       
       obj=rand()%13;
       objArray[0]=obj;
       
       while(nbo<13)
       {
              obj=rand()%13;
              int i=0;
              while(i<nbo)
              {
                     if(objArray[i]==obj)
                            break;
                     i++;
                     if(i==nbo)
                     {
                            objArray[nbo++]=obj;
                            break;
                     }
              }
       }
       
       m_culprit=objArray[12];
       
       for(int i=0;i<m_nbPlayer;i++)
              for(int j=0;j<12/m_nbPlayer;j++)
                     m_players[i].card[j]=objArray[i*12/m_nbPlayer+j];
                     
       for(int i=0;i<m_nbPlayer;i++)
              for(int j=0;j<12/m_nbPlayer;j++)
                     printf("P%d:%d\n",i,m_players[i].card[j]);
}*/

 
      
