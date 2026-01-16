#include "auth.h"
#include "gameTypes.h"
#include "inputSafe.h"

int registerInput( userInfo *user  )
{ 
	printf( "Username:\n" );
	userInput( user->userName, sizeof( user->userName ) );
	printf( "Password\n" );
	userInput( user->userPass, sizeof( user->userPass ) );
}