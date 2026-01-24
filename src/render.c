#include "render.h"
#include "gameTypes.h"
#include <stdio.h>
#include "auth.h"

/*Board Render*/
/*Convert cell CELL_EMPTY, CELL_X and ...O for printboard() render*/
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
/*Converte GameResult to print the winner*/
void converterResult( GameResult result )
{
	switch ( result ) 
	{
	case RESULT_X_WINS:
		printf( "X" );
		break;
	case RESULT_O_WINS:
		printf( "O" );
		break;
	case RESULT_DRAW:
		printf( "DRAWN" );
		break;
	case RESULT_ERROR:
		printf( "WIN ERROR" );
		break;
	default:
		printf( "ERROR" );
		break;
	}
}
/*Prompt msg for Move attempt*/
void displayMoveMsg( State moveResult )
{
	switch ( moveResult ) 
	{
	case MOVE_OK:
		break;
	case MOVE_OCCUPIED:
		printf("Cell already occupied.\n");
		break;
	case MOVE_OUT_RANGE:
		printf("Move out of range. Chose 1-3.\n");
		break;	
	default:
		printf("MOVE ATTEMPT ERROR!\n");
		break;
	}
}

/*Prints the whole board for all board interations*/
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


/*LOGIN*/
/*Auth System output message*/
void authOtpMsg( LoginSystem result )
{
	switch ( result ) 
	{
	case LOGIN_OK:
		printf( "Welcome\n" );
		break;
	case REGISTER_OK:
		printf( "Your account has been successfully created.\n " );
		break;
	case REGISTER_ERROR:
		printf( "Registration error.\n" );
		break;
	case LOGIN_FAILED:
		printf( "Wrong credentials.\n" );
		break;
	case NAME_EXIST:
		printf( "Username unavailable.\n" );
		break;
	case DB_FAILED:
		printf( "Database failed.\n" );
		break;
	case DELETE_ERROR:
		printf( "Delete Error.\n" );
		break;
	case DELETE_OK:
		printf( "Account Deleted.\n" );
		break;
	case USER_NOT_FOUND:
		printf( "User not found.\n" );
		break;
	case USER_NOT_DELETED:
		printf( "Account not deleted.\n" );
		break;	
	default:
		printf( "Unknown error.\n" );
		break;
	}
}



