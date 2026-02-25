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


int main( void )
{
	sqlite3 *db = NULL;
	srand( time( NULL ) );
	/*Refresh every game round. PLAYER1 always played who logged in*/
	roundInfo py;
	board grid;
	userInfo user;
	/*First index stores PLAYER1 second PLAYER2*/
	ScoreInfo roundScore[2];
	
	if(authInitDB( &db ) != 0 )
	{
		printf(" DataBase Failed\n ");
	}
	
	/*Login Block*/
	authRun( &user, db, roundScore);
	while(1)
	{	/*Return roundInfo*/
		roundScore[PLAYER1].id = user.id; 
		py = game( &grid, menuControler( user, db ) );
		converterResult( py.winnerCell );
		printf("%d", scoreControler( db, py, roundScore ));
	}
	
	sqlite3_close( db );
	return 0;
}


