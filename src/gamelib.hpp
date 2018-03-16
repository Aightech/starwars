#ifndef GAME_LIB_H
#define GAME_LIB_H



//#define WATCHPORT 4001

#include "netlib.hpp"
#include "GUI/guilib.hpp"
#include "gamelib.hpp"




#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <sys/wait.h>

#include <netdb.h>
#include <arpa/inet.h>

using namespace sf;
using namespace std;

typedef struct _Player {

	int no;
	char name[16];
	char IPaddress[16];
	int  portNo;
	
	int card[6];
	
}Player;





class Game
{
       /*! \class Game
       * \brief This class represent the game application.
       *      It owns the different methods running the game.
       */
       public:
       
       /*! \brief Constructor */
       Game();
       
       /*! \brief Destructor */
       ~Game(){};
       
       /*! \brief Initialization methode. Some parametere are initialize inside */
       void init();
       
       /*! \brief Process the 'menu' state of the game. It's mainly a event loop */
       //void menu();
       
       
       /*! \brief launch an online game. The game have to be setted (through menu() )*/
       void start();
       
       
       
       
       
       private:
       
       /*! \brief Launch a sh13 server.   */
       //void startServer();
       
       //server related attributes
       int serv_portNo;            /*!< Liste des morceaux*/
       int serv_clientPortNo[4];
       char serv_clientIPAddr[4][16];
       int serv_sfd;
       int serv_cpsfd;
       struct sockaddr_in serv_addr;
       struct sockaddr_in serv_clientAddr[4];
       socklen_t serv_clilen;
       Buffer serv_buff;
       
       //general attributes
       float time;
       int m_state;
       int m_turn;
       
       int m_myNo;
       int m_myname;
       
       int m_host;
       int m_culprit;
       int m_charactersArray[13][3];
       int m_obj;
       int m_otherCharacters[10];
       
       //TCP related attributes
       Buffer m_buffer;
       
       /*! \brief Process a buffer filled by the tcp thread.   */
       //int processBuffer();
       pthread_t m_thread_server;
       
       /*! \brief Process the different connection to the client and fill up a shared buffer with incomming data.   
        *  \param p_data : the shared buffer.*/
       //static void * tcpWatchdog(void * p_data);
       
       //servers related attributes
       Server m_servers[5];
       int m_server;
       int m_nbOfServers;
       
       //Players related attributes
       int m_nbPlayer;
       Player m_players[4];
       
       
       
       /*! \brief Set new data in the players array.   
        *  \param no : the numero of the player.
        *  \param name : the name of the player.
        *  \param IPaddr : the IP address of the player.
        *  \param port : the port of the player.
        */
       int setPlayer(int no, char * name, char * IPaddr, int port);
       
       
       //Graphics related attributes
       Texture m_arrayConnectionTexture;
       
       
       char m_arrayPlayersNames[4][20];
       char m_arrayPlayersIPaddr[4][20];
       int m_arrayPlayersPort[4];
       
       
       
        
       
};




#endif
