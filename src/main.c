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
	srand( time( NULL ) );
	roundInfo py;
	board grid;
	userInfo user;
	ScoreInfo roundScore[2];

	LoginSystem  authResult = LOGIN_FAILED;
	sqlite3 *db = NULL;
	if(authInitDB( &db ) != 0 )
	{
		printf("DataBase Failed\n");
	}
	
	while( authResult != LOGIN_OK )
	{
		/*-1 DEFAULT ARGUMENT. IT DOESNT HAVE A DIRECT EFFECT*/
		authResult = authRun( &user, -1, db );
		authOtpMsg( authResult );
		
		/*NAME_EXIST ONLY HAPPENS DURING REGISTER*/
		if( authResult == NAME_EXIST )
		{
			authResult = authRun( &user, REGISTER, db );
			roundScore[PLAYER1].id = user.id; 
			authOtpMsg( authResult );
		}	
	
	}
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


