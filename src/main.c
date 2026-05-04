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
	if (sodium_init() < 0 ) {
		printf("libsodium init failed\n");
		return 1;
	}
	if(authInitDB( &db ) != 0 )
	{
		printf(" DataBase Failed\n ");
	}
	
	/*Login Block*/
	authRun( &user, db, roundScore);
	while(1)
	{	/*Return roundInfo*/
		roundScore[PLAYER1].id = user.id; 
		playerScoreInnit( player1ScoreDB, roundScore );
		py = game( &grid, menuControler( user, db ), 
			  player1ScoreDB );
		converterResult( py.winnerCell );
		player1ScoreDB[PLAYER1] = roundScore[PLAYER1];
		printf("%d", scoreControler( db, py, player1ScoreDB ));

	}
	

	sqlite3_close( db );
	return 0;
}


