#include "levels.h"
#include "game.h"
#include "gameTypes.h"
#include "inputSafe.h"
#include "render.h"
#include "gameMode.h"
#include "menu.h"
#include <stdio.h>

void game ( void )
{
	GameTypes gameType;
	board grid;
	
	initializer( &grid );
	gameType = runMenu();
	switch ( gameType ) 
	{
	case PLAYER_VS_PLAYER:
		printBoard( &grid );
		gamePVPControler( &grid );
		break;
	case LEVEL1:
		printBoard( &grid );
		gamePvE( &grid, LEVEL1 );
		break;
	case LEVEL2:
		printBoard( &grid );
		
		converterState(gamePvE( &grid, LEVEL2 ));
	default:
		//TODO
		break;
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

GameResult gamePVPControler( board *grid )
{
	GameResult winner;
	position ps;
	State moveResult;
	int turn = 0;
	Cell currentPlayer;
	
	while( 1 )
	{
		ps = gameInput();
		currentPlayer = whoTurn( turn );
		moveResult = gridAlloc( grid, ps.row, ps.collum, currentPlayer );
		displayMoveMsg( moveResult );
		
		if( moveResult == MOVE_OK )
		{
			turn++; 
			printBoard( grid );
			winner = result( grid );
			if( winner != RESULT_NOT_WIN )
			{
				return winner;
			}
			else if( turn == 9 )
			{
				return RESULT_DRAW;
			}
		}
	}	
}

GameResult gamePvE( board *grid, GameTypes level )
{
	GameResult winner;
	position ps;
	State moveResult;
	int turn = 0;
	Cell currentPlayer;
	
	while( 1 )
	{
		printBoard( grid );
		ps = gameInput();
		moveResult = doMove( grid, ps, &turn );
		displayMoveMsg( moveResult );
		/*Human set*/
		if( moveResult != MOVE_OK )
		{ 
			continue;
		}
		winner = result( grid );
		if( winner != RESULT_NOT_WIN )
		{
			return winner;
		}
		if( turn == 9 )
		{
			return RESULT_DRAW;
		}
		/*Ai set*/
		if( level == LEVEL1 )
		{
			ps = level1( grid );
		}
		else if( level == LEVEL2 )
		{
			ps = level2( grid, whoTurn( turn ) );
		}
		if( ps.error == LEVEL_OK )
		{
			doMove( grid, ps, &turn );
		}
		winner = result( grid );
		if( winner != RESULT_NOT_WIN )
		{
			return winner;
		}
		if( turn == 9 )
		{
			return RESULT_DRAW;
		}
	} 
}

State doMove( board *grid, position ps, int *turn )
{
	Cell currentPlayer = whoTurn( *turn );
	State result = gridAlloc( grid, ps.row, ps.collum, currentPlayer );
	if( result == MOVE_OCCUPIED )
	{
		return MOVE_OCCUPIED;
	}
	printBoard( grid );
	(*turn)++;
	return MOVE_OK;
}

GameResult result( const board *grid )
{
	Cell winner = CELL_EMPTY;
	winner = winChecker( grid );
	
	switch ( winner )
	{
	case CELL_X:
		return RESULT_X_WINS;
	case CELL_O:
		return RESULT_O_WINS;
	case CELL_EMPTY:
		return RESULT_NOT_WIN;
	}
	return RESULT_ERROR;
	
}
/*Makes the board move*/
State gridAlloc( board *grid, int row, int column, Cell currentPlayer )
{
	if( isCellEmpty( grid, row, column ) == CELL_EMPTY )
	{
		grid->boardGrid[row][column] = currentPlayer;
		return MOVE_OK;
	}
	else if( row < 0 || column < 0 || row > 2 || column > 2 )
	{
		return MOVE_OUT_RANGE;
	}
	else
	{
		return MOVE_OCCUPIED;
	}
}
/*It return the value of the chose grid(Is a way to make the code more
readable, due to the function name)*/
Cell isCellEmpty( const board *grid, int r, int c )
{
	return grid->boardGrid[r][c];
}

Cell whoTurn( int turn )
{
	if( turn % 2 == 0 )
	{
		return CELL_X;
	}
	else if( turn % 2 != 0 )
	{
		return CELL_O;
	}
	return CELL_EMPTY;
}

void displayMoveMsg( State moveResult )
{
	switch ( moveResult ) 
	{
	case MOVE_OK:
		break;
	case MOVE_OCCUPIED:
		printf("Cell already occupied.\n");
		break;
	default:
		printf("MOVE ATTEMPT ERROR!\n");
		break;
	}
}
/*Initialize all the array grid to ENUM Cell_Empyty*/
void initializer ( board *grid )
{
	
	for( int r = 0 ; r < 3; r++ )
	{
		for( int c = 0 ; c < 3; c++ )
		{
			grid->boardGrid[r][c] = CELL_EMPTY;
		}
	}
}

Cell winChecker( const board *grid )
{
	/*Return cell_x, cell_o, cell_empty for drawn*/
	Cell dResult = diagonalChecker( grid );
	Cell vResult = verticalChecker( grid );
	Cell hResult = horizontalChecker( grid );
	if( dResult != CELL_EMPTY )
	{
		return dResult;
	}
	else if( vResult != CELL_EMPTY )
	{
		return vResult;
	}
	else if( hResult != CELL_EMPTY )
	{
		return hResult;
	}
	return CELL_EMPTY;
}


Cell horizontalChecker( const board *grid )
{
	for( int r = 0 ; r < 3 ; r++  )
	{
		Cell a = grid->boardGrid[r][0];
		/*Check the first first collum and 3 rows*/
		int standTest = ( a != CELL_EMPTY );
		/*If its false it wont return a*/
		for( int c = 1; c < 3 && standTest; c++ )
		{
			if (grid->boardGrid[r][c] != a)
			{
				standTest = 0;
			}
		}
		/*If standTest comes here been true it means it passed the test
		and i can return it without checking the other's(only one row
		can be true at time)*/
		if( standTest )
		{
			return a;
		}
	}
	return CELL_EMPTY;
}

Cell verticalChecker( const board *grid )
{
	for( int c = 0 ; c < 3 ; c++  )
	{
		Cell a = grid->boardGrid[0][c];
		/*Check the first first collum and 3 rows*/
		int standTest = ( a != CELL_EMPTY );
		/*If its false it wont return a*/
		for( int r = 1; r < 3 && standTest; r++ )
		{
			if (grid->boardGrid[r][c] != a)
			{
				standTest = 0;
			}
		}
		/*If standTest comes here been true it means it passed the test
		and i can return it without checking the other's(only one row
		can be true at time)*/
		if( standTest )
		{
			return a;
		}
	}
	return CELL_EMPTY;
}

Cell diagonalChecker(const board *grid)
{
	Cell a = grid->boardGrid[0][0]; 
	Cell b = grid->boardGrid[0][2];

	int standTest = (a != CELL_EMPTY);
	int inverseTest = (b != CELL_EMPTY);
	
	for (int j = 0, k = 2; j < 3; j++, k--)
	{
		/* Main diagonal: (0,0), (1,1), (2,2)*/
		if (grid->boardGrid[j][j] != a)
			standTest = 0;
		
		/*Second diagonal: (0,2), (1,1), (2,0)*/
		if (grid->boardGrid[j][k] != b)
			inverseTest = 0;
	}
	
	if (standTest)
		return a;
	
	if (inverseTest)
		return b;
	
	return CELL_EMPTY;
}




