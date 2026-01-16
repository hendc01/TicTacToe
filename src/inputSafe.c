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
int stringInput( char *buff, int size  )
{
	if( !fgets( buff , size, stdin  ) ) 
	{
		return 0;
	}
	/*Cleaning buffer in case the buffer overflow*/
	if ( strchr( buff, "\n" ))
	{
		cleanBuffer();
	}
	/*It replaces the new line with the null-operator*/
	buff[strcspn( buff, "\n" )] = 0;
	return 1;

	

}

void cleanBuffer ( void )
{
	int ch;
	while( (ch = getchar()) != '\n' && ch != EOF )
	{
		;
	}	
}


