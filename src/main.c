#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gameTypes.h"
#include "inputSafe.h"
#include "game.h"
#include "menu.h"
#include "auth.h"
#include "render.h"


int main( void )
{
	srand( time( NULL ) );
	board grid;
	userInfo user;
	GameResult winner;
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
			authOtpMsg( authResult );
		}
		while(1)
		{
			winner = game( &grid, menuControler( user, db ) );
			converterResult( winner );
		}
		
	}
	
	sqlite3_close( db );
	return 0;
}


