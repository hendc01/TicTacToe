#include "render.h"
#include "gameTypes.h"
#include <stdio.h>



void converterCell( Cell cell)
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
void converterState( State state )
{
	switch ( state ) 
	{
	case RESULT_X_WINS:
		printf( "X" );
		break;
	case RESULT_O_WINS:
		printf( "O" );
		break;
	case RESULT_DRAW:
		printBoard( "DRAWN" );
	case RESULT_ERROR:
		printf( "WIN ERROR" );
		break;
	default:
		printf( "ERROR" );
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
			converterCell( grid->boardGrid[r][c] );
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

/*Login Render*/
void loginMenu( void )
{
	printf( " TIC-TAC-TOE " );
	printf( "(1) Login" );
	printf( "(2)Register" );
	
}

