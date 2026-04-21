#include "inputSafe.h"
#include "game.h"
#include <string.h>
#include <stdio.h>
#include <sodium.h>

/*Validate integer input and add a domain*/
int intInput ( int min, int max )
{
	int num;
	printf( "Please, add a number: " );
	while(  scanf( "%d", &num ) == 0 ||  num < min || num > max  )
	{
		printf( "Invalid Number. Please type again.\n" );
		printf( "Please, add a number: \n ");
		cleanBuffer();
		continue;
	
	}
	cleanBuffer();
	return num;
}

/*Validate char*/
SafeString stringInput( char *buff, int size  )
{
	int overFlow = 0;
	if( !fgets( buff, size, stdin  ) ) 
	{
		return STRING_ERROR;
	}
	/*Cleaning buffer in case the buffer overflow*/
	if ( !strchr( buff, '\n' ))
	{
		cleanBuffer();
		overFlow = 1;
	}
	/*It replaces the new line with the null-operator*/
	buff[strcspn( buff, "\n" )] = 0;
	if( overFlow )
	{
		return STRING_TRUNCATED;
	}
	return STRING_OK;
}
/*Call stringInput and checks if it is in the correct size for the DB 
FOR REGISTER FUNCTIONS*/
void userInput( char *userInfo, int size )
{
	SafeString stringOut = STRING_ERROR;
	while( stringOut != STRING_OK )
	{
		stringOut = stringInput( userInfo, size );
		if( stringOut == STRING_TRUNCATED )
		{
			printf("Maximum %d characterer\n", size -1 );
			continue;
		}
		else if( stringOut == STRING_ERROR )
		{
			printf( "Error. Try Again\n" );
		}
	}
}
/*Allow the user to input the grids, and checks if that block is empty*/
position gameInput()
{
	position ps;
	
	printf("Type the row\n");
	ps.row = intInput(1,3) -1 ;
	
	printf("Type the collum:\n");
	ps.collum = intInput(1, 3) - 1;
	
	return ps;
}

/*Login and register credentials input*/
int loginInput( userInfo *user, int chose  )
{ 
	char hashed_password[crypto_pwhash_STRBYTES];
	printf( "Username:\n" );
	userInput( user->userName, sizeof( user->userName ) );
	printf( "Password\n" );
	userInput( user->userPass, sizeof( user->userPass ) );
	if( chose == 0 ){
		crypto_pwhash_str(hashed_password, user->userPass, 
				  strlen(user->userPass),
				  crypto_pwhash_OPSLIMIT_INTERACTIVE,
				  crypto_pwhash_MEMLIMIT_INTERACTIVE);
		strcpy( user->userPass, hashed_password );	
	}

	return 1;
}



/*Player1 Decides if wants to play with X or O(*/
void decideSymbol( roundInfo *p )
{
	char pT;
	while(1)
	{
		printf( "Do you wanna play with X or O? Type(X or O): " );
		scanf(" %c", &pT);
		if( pT == 'X' || pT == 'x'  )
		{
			p->playerTurn = PLAYER1;
			p->player1 = CELL_X;
			p->player2 = CELL_O;
			break;
		}
		else if( pT == 'o' || pT == 'O' || pT == '0' )
		{
			p->playerTurn = PLAYER2;
			p->player2 = CELL_X;
			p->player1 = CELL_O;
			break;
		}
		else
		{
			printf( " %c command not recognized. Please Type X or O \n"
				   , pT );	
		}
		
	}
}


void cleanBuffer ( void )
{
	int ch;
	while( (ch = getchar()) != '\n' && ch != EOF )
	{
		;
	}	
}
