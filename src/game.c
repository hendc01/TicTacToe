#include "levels.h"
#include "game.h"
#include "gameTypes.h"
#include "inputSafe.h"
#include "render.h"
#include "gameMode.h"
#include "menu.h"
#include <stdio.h>
#include "win.h"
/*Main controler*/

/*Run one session of the game*/
GameResult game ( board *grid, GameTypes gameChoice)
{
	/*Returns Game Option*/	

	initializer( grid );
	
	switch ( gameChoice ) 
	{
	case PLAYER_VS_PLAYER:
		printBoard( grid );
		return gamePVPControler( grid );
	case PLAYER_VS_MACHINE:
		gameChoice = pveMenu();
		return gamePvEControler( grid, gameChoice );
	default:
		return RESULT_ERROR;
	}
}
/*Player vs Player controler*/
GameResult gamePVPControler( board *grid )
{
	GameResult winner = RESULT_NOT_WIN;
	int turn = 0;
	do
	{
		winner = humanTurn( grid, &turn );
		
	}while( winner == RESULT_NOT_WIN  );
	return winner;
}

/*Controls the functions responsible for the Player vs Machine*/
GameResult gamePvEControler( board *grid, GameTypes level )
{
	GameResult winner = RESULT_NOT_WIN;
	position ps;
	State moveResult;
	int turn = 0;
	printBoard( grid );
	/*it runs until a winner is found or a Unknow error appear*/
	while( winner == RESULT_NOT_WIN )
	{
		/*Takes the human input and checks if there has been a win*/
		winner = humanTurn( grid, &turn );
		if( winner != RESULT_NOT_WIN  )
		{
			break;
		}
		/*The level control switch to the correct LEVEL(AI dIFFICULT)
		  And it returns thte position of the grid*/
		ps = levelControler( grid, level, turn  );
		if( ps.error == LEVEL_ERROR )
		{
			return RESULT_ERROR;
		}
		/*doMove stores the position in the array, and knows the player
		by the number of turns played*/
		moveResult = doMove( grid, ps, &turn );
		if( moveResult != MOVE_OK )
		{
			displayMoveMsg( moveResult );
		}
		
		winner = result( grid, turn );
	} 
	return winner;
}
/*Control all the PVE levels by calling it according to the requested 
Level*/
position levelControler( board *grid ,GameTypes level,
						int turn )
{
	position ps;
	switch ( level ) 
	{
	case LEVEL1:
		ps = level1( grid );
		break;
	case LEVEL2:
		ps = level2( grid, whoTurn( turn ) );
		break;
	default:
		ps.row = -1;
		ps.collum = -1;
		ps.error = LEVEL_ERROR;
		break;
	}
	return ps;
}
/*This move the board to next player by calling GridAlloc and iterating
turn to pass the turn to the next player (x or o)*/
State doMove( board *grid, position ps, int *turn )
{
	Cell currentPlayer = whoTurn( *turn );
	State result = gridAlloc( grid, ps.row, ps.collum, currentPlayer );
	if( result == MOVE_OCCUPIED )
	{
		return MOVE_OCCUPIED;
	}
	else if( result == MOVE_OUT_RANGE )
	{
		return MOVE_OUT_RANGE;
	}
	
	printBoard( grid );
	(*turn)++;
	return MOVE_OK;
}


/*Assign to board cell array X or O, if within the range*/
State gridAlloc( board *grid, int row, int column, Cell currentPlayer )
{
	if( row < 0 || column < 0 || row > 2 || column > 2 )
	{
		return MOVE_OUT_RANGE;
	}
	else if( isCellEmpty( grid, row, column ) == CELL_EMPTY )
	{
		grid->boardGrid[row][column] = currentPlayer;
		return MOVE_OK;
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
/*Populate the board with Humans Input Moves, and returns the move 
result. Which can be drawn, x, o, not a win or error*/
GameResult humanTurn( board *grid, int *turn )
{
	GameResult winner = RESULT_NOT_WIN;
	position ps;
	State moveResult = MOVE_OCCUPIED;
	/*It runs the loop until doMove return a valid move (MOVE_OK)*/
	while( moveResult != MOVE_OK  )
	{
		
		ps = gameInput();
		moveResult = doMove( grid, ps, turn );
		
		if( moveResult == MOVE_OK )
		{
			winner = result( grid, *turn );
			break;
		}
		displayMoveMsg( moveResult );
		
	}
	return winner;
}

/*It returns the X or O based if the turn is even or odd*/
Cell whoTurn( int turn )
{
	if( turn % 2 == 0 )
	{
		return CELL_X;
	}
	return CELL_O;
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





