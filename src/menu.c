#include "menu.h"
#include "inputSafe.h"
#include "gameTypes.h"
#include <stdio.h>

GameTypes runMenu( void )
{
	GameTypes level;
	GameTypes menuOpt = mainMenu();
	switch ( menuOpt ) 
	{
		case PLAYER_VS_PLAYER:
			return PLAYER_VS_PLAYER;
		case PLAYER_VS_MACHINE:
		
			level = playerMachine();
			if( level == LEVEL1)
			{
				return LEVEL1;
			}
			else if( level == LEVEL2 )
			{
				return LEVEL2;
			}
			return MENU_ERROR;
		default:
			return MENU_ERROR;
	}
}

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

/*Login Render*/
LoginOpt loginMenu( void )
{
	printf( " TIC-TAC-TOE " );
	printf( "(1) Login" );
	printf( "(2) Register" );
	return (LoginOpt)intInput( 1, 2 );
}



