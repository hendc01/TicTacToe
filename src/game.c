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
	playerSymbol py;
	GameResult winner = RESULT_NOT_WIN;
	int turn = 0;
	py = decideSymbol();
	do
	{
		switch ( py.firstPlayer ) 
		{
		case PLAYER1_START:
			winner = humanTurn( grid, &turn, py.player1 );
			playerSwitch( &py.firstPlayer );
			break;
		case PLAYER2_START:
			winner = humanTurn( grid, &turn, py.player2 );
			playerSwitch( &py.firstPlayer );
			break;
		default:
			break;
		}	
	}while( winner == RESULT_NOT_WIN  );
	return winner;
}

/*Controls the functions responsible for the Player vs Machine*/
GameResult gamePvEControler( board *grid, GameTypes level )
{
	playerSymbol py;
	py = decideSymbol();
	GameResult winner = RESULT_NOT_WIN;
	position ps;
	State moveResult;
	int turn = 0;
	printBoard( grid );
	/*it runs until a winner is found or a Unknow error appear*/
	while( winner == RESULT_NOT_WIN )
	{
		switch ( py.firstPlayer ) 
		{
		case PLAYER1_START:
			winner = humanTurn( grid, &turn, py.player1 );
			if( winner != RESULT_NOT_WIN  )
			{
				break;
			}
			playerSwitch( &py.firstPlayer );
			break;
		case PLAYER2_START:
			ps = levelControler( grid, level, turn  );
			if( ps.error == LEVEL_ERROR )
			{
				return RESULT_ERROR;
			}
			moveResult = doMove( grid, ps, &turn, py.player2 );
			if( moveResult != MOVE_OK )
			{
				displayMoveMsg( moveResult );
			}
			playerSwitch( &py.firstPlayer );
			winner = result( grid, turn );
			break;
		default:
			break;
		}
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
State doMove( board *grid, position ps, int *turn, Cell symbol)
{
	State result = gridAlloc( grid, ps.row, ps.collum, symbol );
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
GameResult humanTurn( board *grid, int *turn, Cell symbol )
{
	GameResult winner = RESULT_NOT_WIN;
	position ps;
	State moveResult = MOVE_OCCUPIED;
	/*It runs the loop until doMove return a valid move (MOVE_OK)*/
	while( moveResult != MOVE_OK  )
	{
		
		ps = gameInput();
		moveResult = doMove( grid, ps, turn, symbol );
		
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
playerSymbol decideSymbol( void )
{
	char pT;
	playerSymbol p;
	while(1)
	{
		printf( "Do you wanna play with X or O? Type(X or O): " );
		scanf(" %c", &pT);
		if( pT == 'X' || pT == 'x'  )
		{
			p.firstPlayer = PLAYER1_START;
			p.player1 = CELL_X;
			p.player2 = CELL_O;
			return p;
		}
		else if( pT == 'o' || pT == 'O' )
		{
			p.firstPlayer = PLAYER2_START;
			p.player2 = CELL_X;
			p.player1 = CELL_O;
			return p;
		}
		printf( " %c command not recognized. Please Type X or O \n"
			   , pT );
	}
}


void playerSwitch( Player *player )
{
	*player = ( *player == PLAYER1_START ) ? PLAYER2_START : 
	PLAYER1_START;
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





