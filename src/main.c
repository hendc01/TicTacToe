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
	LoginSystem  authResult = LOGIN_FAILED;
	while( authResult != LOGIN_OK )
	{
		/*-1 DEFAULT ARGUMENT. IT DOESNT HAVE A DIRECT EFFECT*/
		authResult = authRun( &user, -1 );
		authOtpMsg( authResult );
		if( authResult == NAME_EXIST )
		{
			authResult = authRun( &user, REGISTER );
			authOtpMsg( authResult );
		}
	}
	
	game( &grid );
	
	return 0;
}


