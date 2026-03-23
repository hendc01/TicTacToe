#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include "lanSocket.h"
#include "inputSafe.h"
#include "game.h"
#include "menu.h"
#include "gameTypes.h"
#include "innit.h"

int lanPvPControler(){
	PvPModes opt = pvpSubMenu2();
	switch( opt ){
	case HOST_GAME:
		serverSocketRun();
		pvpRunClient();
		break;
	case ENTER_GAME:
		if((clientControler() == 1)){printf("Error connecting\n");}
		pvpRunClient();
		break;
	default:
		break;
	}
	return 0;
}

int pvpRunClient( SOCKET sckt, board *grid, roundInfo *rInfo ){
	/*First i need to send and receive a msg, then i start doing the loop
	logic*/
	unsigned char msg[3];
	
	*rInfo = roundInit( );
	int turn = 0;
	position ps;
	if( turn == 0 ) decideSymbol( rInfo );
	ps = gameInput();
	msg[0] = ps.row;
	msg[1] = ps.collum;
	msg[3] = rInfo->playerTurn;
	gameModeControler( grid, ps, rInfo, &turn );
	sendPosition( sckt, msg );
}

int pvpMoveLan( SOCKET sckt, board *grid, roundInfo *rInfo, int *turn 
				 ){
	position ps;
	unsigned char msg[3];
	if( turn == 0 ) decideSymbol( rInfo ), *rInfo = roundInit( );
	
	if( rInfo->playerTurn == PLAYER1 ){
		ps = gameInput();
		msg[0] = ps.row;
		msg[1] = ps.collum;
		msg[3] = rInfo->playerTurn;
		gameModeControler( grid, ps, rInfo, turn );
		sendPosition( sckt, msg );
		turn++;
	}
	else{
		getPosition( sckt ,&ps);
		gameModeControler( grid, ps, rInfo, turn );
		turn++;
	}
	return 1;
}
int serverSocketRun(){
	/*Initializing WinSock*/
	SOCKET serverSocket;
	SOCKET connectionSocket;
	struct sockaddr_in service = createSocketInfo( SERVER );
	
	if( wsaStartUp() == 1 ){ return 1;}
	
	if( createSocket( &serverSocket )  == 1 ){
		return 1; 
	}
	if( binding( serverSocket, &service ) == 1 ){
		return 1; 
	}
	if( activListen( serverSocket ) == 1 ){
		return 1;
	}
	if( AcceptConnect( serverSocket, &connectionSocket  ) == 1){
		return 1;
	}
	closesocket( serverSocket );
	closesocket( connectionSocket );
	WSACleanup();
	return 0;
}

int clientControler(){
	unsigned char row, column, player;
	unsigned char msg[3] = { row, column, player };
	
	SOCKET clientSocket;
	struct sockaddr_in serverAddr = createSocketInfo( CLIENT );
	
	if( wsaStartUp() == 1 ){ return 1; };
	
	if( createSocket( &clientSocket )  == 1 ){ return 1; }
	
	if( connect( clientSocket, ( struct sockaddr * ) &serverAddr, 
				sizeof( serverAddr ) ) == SOCKET_ERROR){ return 1; }
	if( sendPosition( clientSocket, msg ) == SOCKET_ERROR){ return 1; }

	return 0;
}


int createSocket(SOCKET *socketAdress ){
	*socketAdress = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if(*socketAdress == INVALID_SOCKET){
		printf( "Socket failed with error: %d\n", WSAGetLastError() );
		WSACleanup();
		return 1;
	}
	return 0;
}

struct sockaddr_in createSocketInfo(  int ip ){
	struct sockaddr_in service;
	serverAdress adress;
	char ipAdress[21] = "INADDR_ANY";
	service.sin_family = AF_INET;
	
	if( ip == 1 ){
		printf( "Digite the Ip adress and port of the server you want" 
			   "to connect. E.g: 192.168.1.1:9091\n " ); 
		stringInput( ipAdress, sizeof(ipAdress));
		adress = serveParssing( ipAdress );
		service.sin_addr.s_addr = inet_addr(adress.ipAdress);
		service.sin_port = htons( atoi(adress.portAdress) );
	}
	else{
		service.sin_port = htons(9091);	
		service.sin_addr.s_addr = INADDR_ANY;
	}
	return service;
}

int binding( SOCKET sckt, struct sockaddr_in *scktInfo ){
	if( bind( sckt, (struct sockaddr*) scktInfo, sizeof(*scktInfo) ) == 
	   SOCKET_ERROR){
		printf( "bind failed with error: %d", WSAGetLastError() );
		closesocket( sckt );
		return 1;
	}
	return 0;
}

int activListen( SOCKET sckt ){
	if( listen( sckt, 1 ) == SOCKET_ERROR  ){
		printf( "listen failed with error: %d\n", WSAGetLastError() );
		closesocket( sckt );
		return 1;
	}
	return 0;
}

int AcceptConnect( SOCKET sckt, SOCKET *clientSocket ){
	printf("Waiting for client to connect...\n");
	*clientSocket = accept( sckt, NULL, NULL );
	if( *clientSocket == INVALID_SOCKET	)
	{
		printf( "accept failed with error: %d\n", WSAGetLastError() );
		closesocket( sckt );
		return 1;
	}
	else{
		printf( "client connected.\n" );
	}
	return 0;
}

int ConnectToServer( SOCKET clientSckt, struct sockaddr_in *address ){
	return connect( clientSckt, ( struct sockaddr *) address, 
				    sizeof(*address) );
}
	
serverAdress serveParssing( char serverAdr[21]){
	serverAdress adress;
	int i = 0;
	int ipIndex = 0;
	int portIndex = 0;
	
	while( serverAdr[i] != ':' && serverAdr[i] != '\0' ){
		adress.ipAdress[ipIndex] = serverAdr[i];
		i++;
		ipIndex++;
	}
	adress.ipAdress[ipIndex] = '\0';
	i++;
	
	while( portIndex < 4 && serverAdr[i] != '\0' ){
		adress.portAdress[portIndex] = serverAdr[i];
		i++;
	}
	
	adress.portAdress[portIndex] = '\0';
	return adress;
}

int wsaStartUp(){
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD( 2, 2), &wsaData );
	if( result != 0 )
	{
		printf( "serve socket startup failed with error: %d", result );
		return 1;
	}
	return 0;
}

int sendPosition( SOCKET connectionSocket, unsigned char msg[4] ){
	int result;
	result = send( connectionSocket, (char*) msg, 4, 0 );
	if( result == SOCKET_ERROR ){
		printf( "send failed with error: %d\n", WSAGetLastError() );
	}
	return result;
}

int recvInfo( SOCKET skct,  char *buf, int size ){
	int total = 0;
	
	while( total < size ){
		int result = recv( skct, buf + total, size - total, 0 );
		if( result == 0 ){
			printf( " 0 bytes\n" );
		}
		else{
			printf( "receive position error: %d\n", WSAGetLastError() );
			return 1;
		}
		total += result;
	}
	return 0;	
}

int getPosition( SOCKET sckt,  position *lanPosition  ){
	unsigned char msg[4] = { 0 };
	if(recvInfo(sckt, (char*)msg, 4) == 1){return 1;};
	lanPosition->row = (int)msg[0];
	lanPosition->collum = (int)msg[1];
	lanPosition->currentPlayer = (Player)msg[2];
	lanPosition->isThereWin = msg[3];
	return 0;
}


