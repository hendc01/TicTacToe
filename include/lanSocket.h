#ifndef LANSOCKET_H
#define LANSOCKET_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include "gameTypes.h"
int pvpSentInfo( SOCKET sckt, board *grid, roundInfo *rInfo, int *turn );
int pvpRunServer();
int lanPvPControler( int i );
int serverSocketMaker( SOCKET *connectionSocket );
int createSocket(SOCKET *socketAdress);
int binding( SOCKET sckt, struct sockaddr_in *scktInfo );
int activListen( SOCKET sckt );
int AcceptConnect( SOCKET sckt, SOCKET *clientSocket );
serverAdress serveParssing( char serverAdr[21]);
struct sockaddr_in createSocketInfo(  int ip );
int wsaStartUp();
int sendPosition( SOCKET connectionSocket, unsigned char msg[4] );
int getPosition( SOCKET sckt,  position *lanPosition  );
int recvInfo( SOCKET skct,  char *buf, int size );
int clientSocketMaker( SOCKET *clientSocket );
int pvpGameLoop( SOCKET sckt, roundInfo *rInfo, board *grid );
int runLanGame( roundInfo *rInfo, SOCKET *connectionSocket, 
	       board *grid, Player player);
#endif
