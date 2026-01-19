#include "inputSafe.h"
#include "game.h"
#include "string.h"
#include <stdio.h>

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
	if( !fgets( buff , size, stdin  ) ) 
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
void cleanBuffer ( void )
{
	int ch;
	while( (ch = getchar()) != '\n' && ch != EOF )
	{
		;
	}	
}


