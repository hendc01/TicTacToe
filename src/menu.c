#include "menu.h"
#include "inputSafe.h"
#include "gameTypes.h"
#include "auth.h"
#include <stdio.h>
		
		
GameTypes menuControler( userInfo user )
{
	/*Returns Game Option*/
	if( user.userRole != ADM )
	{
		return mainMenu();
	}
	else
	{
		/*Supervisor modes*/
		return admMenuController();
	}
	/*Supervisor Menus*/
	
}
GameTypes admMenuController()
{
	AdmTypes menuChoice, admChoice;
	
	while( 1 )
	{
		menuChoice = admMenu();
		switch ( menuChoice ) 
		{
			/*Normal modes*/
		case ADM_PVP:
			return PLAYER_VS_PLAYER;
		case ADM_PVE:
			return PLAYER_VS_MACHINE;
			/*Supervisor Only*/
		case ADM_MODE:
			admChoice = admMode();
			if( menuChoice == EXIT )
			{
				break;
			}
			admModeController( admChoice );
			break;
		default:
			break;
		}
	}	
}
		
void admModeController( AdmTypes admType )
{
	switch ( admType ) 
	{
	case DELETE_ACCOUNT:
		//TODO
		break;
	case ADD_ACCOUNT:
		//TODO
		break;
	case EXIT:
		break;
	default:
		//TODO
		break;
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
		
AdmTypes admMenu( void )
{
	printf("TIC-TAC-TOE\n");
	printf( "(1) PLAYER VS PLAYER\n" );
	printf( "(2) PLAYER VS MACHINE\n" );
	printf( "(3) SUPERVISOR MODE\n \n" );
	return (AdmTypes)intInput( 1, 3);
}
		
AdmTypes admMode( void )
{
	printf("SUPERVISOR MODE\n");
	printf( "(1) DELETE ACCOUNT\n" );
	printf( "(2) ADD ADM ACCOUNT\n" );
	printf( "(3) EXIT\n" );
	return (AdmTypes)intInput( 1, 3 );
}
		
GameTypes pveMenu( void )
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
		
		

