#include "menu.h"
#include "inputSafe.h"
#include "gameTypes.h"
#include "auth.h"
#include <stdio.h>



GameTypes mainMenu( void )
{
	printf("TIC-TAC-TOE\n");
	printf( "(1) PLAYER VS PLAYER\n" );
	printf( "(2) PLAYER VS MACHINE\n" );
	printf( "TO BE UPDATE \n" );
	return (GameTypes)intInput( 1, 2 );
}

GameTypes playerMachine( void )
{
	int choice = 0;
	printf( "Player vs Machine\n" );
	printf( "(1) LEVEL 1 \n" );
	printf( "(2) LEVEL 2\n" );
	choice = intInput( 1, 2 );
	return ( choice == 1 ) ? LEVEL1 : LEVEL2;
}
/*Login*/

LoginOpt loginMenu( void )
{
	printf( " TIC-TAC-TOE \n" );
	printf( "(1) Login\n" );
	printf( "(2) Register\n" );
	return (LoginOpt)intInput( 1, 2 );
}



