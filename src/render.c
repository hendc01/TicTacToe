#include "render.h"
#include "gameTypes.h"
#include <stdio.h>

void menu1 ( void )
{
	printf( "TicTacToe\n" );
	printf( " 1  2  3 \n" );
	printf( "__|__|__ 1\n" );
	printf( "__|__|__ 2\n" );
	printf( "__|  |__ 3\n" );
}

void converter( Cell cell)
{
	switch ( cell ) 
	{
	case CELL_X:
		printf( "X" );
		break;
	case CELL_O:
		printf( "O" );
		break;
	case CELL_EMPTY:
		printf( " " );
		break;
	default:
		printf( "?" );
		break;
	}
}

void printBoard( const board *grid )
{
	printf( "  1  2  3 \n" );
	for( int r = 0; r < 3; r++ )
	{
		printf( "%d ", r+1 );
		for( int c = 0; c < 3; c++)
		{
			converter( grid->boardGrid[r][c] );
			if( c < 2 )
			{
				printf( " |" );
			}
		}
		printf( "\n" );
		if( r < 2 )
		{
			printf( "------------\n" );	
		}
	}
}


