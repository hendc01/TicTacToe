#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gameTypes.h"
#include "inputSafe.h"
#include "game.h"


int main( void )
{
	srand( time( NULL ) );
	game();
	
	return 0;
}


