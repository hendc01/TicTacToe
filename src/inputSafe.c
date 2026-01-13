#include "inputSafe.h"
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

void cleanBuffer ( void )
{
	while( getchar() != '\n' )
	{
		;
	}	
}


