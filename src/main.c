#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gameTypes.h"
#include "inputSafe.h"
#include "game.h"
#include "menu.h"
#include "auth.h"
#include "render.h"
#include "score.h"
#include "lanSocket.h"
#include "innit.h"
#include <sodium.h>
#include "gui.h"

int main( void )
{
	
	//lanPvPControler();
	sqlite3 *db = NULL;
	srand( time( NULL ) );
	/*Refresh every game round. PLAYER1 always played who logged in*/
	roundInfo py;
	board grid;
	userInfo user;
	/*First index stores PLAYER1 second PLAYER2*/
	ScoreInfo roundScore[2];
	ScoreInfo player1ScoreDB[4];


	int gui = -1;
	if (sodium_init() < 0 ) {
		printf("libsodium init failed\n");
		return 1;
	}
	if(authInitDB( &db ) != 0 )
	{
		printf(" DataBase Failed\n ");
	}
	while(1){
		while( gui == -1 ){
			gui = versionChoice();	
			clearScreen();
		}
		/*Login Block*/
		if( gui == 1){
			authRun( &user, db, roundScore);
			pressEnter();
			clearScreen();
		}
		
		
		while( gui == 1)
		{	/*Return roundInfo*/
			int choice = 0;
			roundScore[PLAYER1].id = user.id; 
			playerScoreInnit( player1ScoreDB, roundScore );
			choice = menuControler( user, db );
			if( choice == 3 ){
				gui = 2;
				continue;
			}
			py = game( &grid, choice, 
				  player1ScoreDB );
			
			if( py.turn == 10 ){
				continue;
			}
			if( py.turn == 11){
				//LAN
				converterResult( py.winnerCell );
				pressEnter();
				continue;
			}
			converterResult( py.winnerCell );
			player1ScoreDB[PLAYER1] = roundScore[PLAYER1];
			printf("%d", scoreControler( db, py, player1ScoreDB ));
			
		}
		printf("%d", gui);
		while( gui == 2 )
		{	/*Return roundInfo*/
			guiControler( &grid, player1ScoreDB, db, &py );
			gui = 1;
			printf("test1");
			continue;
			
		}	
	}
	
	

	return 0;
}



