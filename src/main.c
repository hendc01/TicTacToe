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
	while(1)
	{	/*Return roundInfo*/

		py = game( &grid, 1, 
			  player1ScoreDB );

	}

	return 0;
}



