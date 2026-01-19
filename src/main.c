#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gameTypes.h"
#include "inputSafe.h"
#include "game.h"
#include "menu.h"
#include "auth.h"


int main( void )
{
	srand( time( NULL ) );
	board grid;
	userInfo user;
	printf("%d",authRun( &user ));
	game( &grid );
	
	return 0;
}


