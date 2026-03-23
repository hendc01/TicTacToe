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

int lanPvPControler( ){
	SOCKET connectionSocket;
	SOCKET clientSocket;
	board grid;
	roundInfo rf;
	PvPModes opt = pvpSubMenu2();
	switch( opt ){
	case HOST_GAME:
		serverSocketRun( &connectionSocket );
		hostController( &rf, &connectionSocket, &grid, PLAYER1 );
		break;
	case ENTER_GAME:
		if((clientControler( &grid, &rf, &clientSocket ) == 1)){
			printf("Error connecting\n");}
		hostController( &rf, &clientSocket, &grid, PLAYER2 );
		break;
	default:
		break;
	}
	return 0;
}

int hostController(roundInfo *rInfo, SOCKET *connectionSocket,
				   board *grid, Player player){
	gridInnit(grid);
	position lanPosition;
	int turn = 0;
	printf("Entered hostController as player %d\n", player);
	
	*rInfo = roundInit();
	printf("Aftter round innit");
	rInfo->playerTurn = PLAYER1;
	rInfo->player1 = CELL_X;
	rInfo->player2 = CELL_O;
	
	while(1){
		if(rInfo->playerTurn == player){
			printf("About to send move\n");
			if(pvpRunClient(*connectionSocket, grid, rInfo, &turn) == 1){
				printf("pvpRunClient failed\n");
				return 1;
			}
		}
		else{
			printf("About to receive move\n");
			if(getPosition(*connectionSocket, &lanPosition) == 1){
				printf("getPosition failed\n");
				return 1;
			}
			printf("Received move\n");
			gameModeControler(grid, lanPosition, rInfo, &turn);
		}
		
		if(rInfo->winnerCell != RESULT_NOT_WIN){
			printf("Player %d has won\n", rInfo->winnerPy);
			break;
		}
	}
	return 0;
}



int pvpRunClient( SOCKET sckt, board *grid, roundInfo *rInfo, int *turn ){
	unsigned char msg[4];
	position ps;
	Player playedBy = rInfo->playerTurn;
	
	ps = gameInput();
	printf("Before gameModeControler\n");
	gameModeControler( grid, ps, rInfo, turn );
	printf("After gameModeControler\n");
	
	msg[0] = ps.row;
	msg[1] = ps.collum;
	msg[2] = playedBy;
	msg[3] = rInfo->winnerCell;
	
	if(sendPosition( sckt, msg ) == SOCKET_ERROR){
		return 1;
	}
	return 0;
}

int pvpMoveLan( SOCKET sckt, board *grid, roundInfo *rInfo, int *turn 
				 ){
	position ps;
	unsigned char row, column, player, isThereWin;
	unsigned char msg[4] = { row, column, player, isThereWin };
	if( (*turn) == 0 ) decideSymbol( rInfo ), *rInfo = roundInit( );
	
	if( rInfo->playerTurn == PLAYER1 ){
		ps = gameInput();
		msg[0] = ps.row;
		msg[1] = ps.collum;
		msg[2] = rInfo->playerTurn;
		msg[3] = ps.isThereWin;
		gameModeControler( grid, ps, rInfo, turn );
		sendPosition( sckt, msg );
		(*turn)++;
	}
	else{
		getPosition( sckt ,&ps);
		gameModeControler( grid, ps, rInfo, turn );
		(*turn)++;
	}
	return 1;
}
int serverSocketRun( SOCKET *connectionSocket ){
	/*Initializing WinSock*/
	SOCKET serverSocket;
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
	if( AcceptConnect( serverSocket, connectionSocket  ) == 1){
		return 1;
	}
	/*
	closesocket( serverSocket );
	closesocket( *connectionSocket );
	WSACleanup();
	*/
	return 0;
}

int clientControler( board *grid, roundInfo *rf, SOCKET *clientSocket ){
	struct sockaddr_in serverAddr = createSocketInfo( CLIENT );
	
	if( wsaStartUp() == 1 ){ return 1; };
	
	if( createSocket( clientSocket )  == 1 ){ return 1; }
	
	if( connect( *clientSocket, ( struct sockaddr * ) &serverAddr, 
				sizeof( serverAddr ) ) == SOCKET_ERROR){ return 1; }
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
		portIndex++;
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

int recvInfo(SOCKET skct, char *buf, int size) {
	int total = 0;
	
	while (total < size) {
		int result = recv(skct, buf + total, size - total, 0);
		
		if (result > 0) {
			total += result;
		}
		else if (result == 0) {
			printf("connection closed by peer\n");
			return 1;
		}
		else {
			printf("receive failed with error: %d\n", WSAGetLastError());
			return 1;
		}
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


