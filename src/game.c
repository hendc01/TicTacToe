#include "levels.h"
#include "game.h"
#include "gameTypes.h"
#include "inputSafe.h"
#include "render.h"
#include "gameMode.h"
#include "menu.h"
#include <stdio.h>
#include "win.h"
#include "innit.h"
/*Main controler*/

/*Run one session of the game*/
roundInfo game ( board *grid, GameTypes gameChoice)
{
	/*Returns Game Option*/	
	roundInfo py;
	py = roundInit( );
	gridInnit( grid );
	
	switch ( gameChoice ) 
	{
	case PLAYER_VS_PLAYER:
		printBoard( grid );
		gamePVPControler( grid, &py );
		return py;
		
	case PLAYER_VS_MACHINE:
		gameChoice = pveMenu();
		gamePvEControler( grid, gameChoice, &py);
		return py;
		
	default:
		py.winnerCell = RESULT_ERROR;
		return py;
	}
}
/*
  Player vs Player controler
  -First turn is decided by decideSymbol(player1 choice), after it 
  by playerSwitch 
  -humanTurn populate the board with user input and check for a win with
   result() function, scanning all the board for a win return ENUM
  (GameResult)
*/
roundInfo gameModeControler( board *grid, position ps, roundInfo *py, 
							int *turn)
{
	State roundState;

    roundState = doMove( grid, ps ,turn, 
						symbolSwitch(*py) );
	if( roundState != MOVE_OK )
	{
		displayMoveMsg(roundState);
		return *py;
	}
	py->winnerCell = result( grid, *turn );
	if( py->winnerCell == RESULT_NOT_WIN )
	{
		playerSwitch( &py->playerTurn );
	}
	else if( py->winnerCell != RESULT_DRAW)
	{
		py->winnerPy = py->playerTurn;
		py->losserPy = (py->playerTurn == PLAYER1)? PLAYER2 : PLAYER1;
	}
	return *py;
}
void gamePVPControler( board *grid, roundInfo *rInfo )
{
	*rInfo = roundInit( );
	int turn = 0;
	position ps;
	decideSymbol( rInfo );
	while(rInfo->winnerCell == RESULT_NOT_WIN)
	{
		ps = gameInput();
		gameModeControler( grid, ps, rInfo, &turn );
	}
}

/*Controls the functions responsible for the Player vs Machine*/
void gamePvEControler( board *grid, GameTypes level, 
						   roundInfo *rInfo )
{
	int turn = 0;
	*rInfo = roundInit( );

	decideSymbol( rInfo );
	position ps;
	
	printBoard( grid );
	
	/*it runs until a winner is found or a Unknow error appear*/
	while( rInfo->winnerCell == RESULT_NOT_WIN )
	{
		switch ( rInfo->playerTurn ) 
		{
		case PLAYER1:
			ps = gameInput();
			break;
		case PLAYER2:
			ps = levelControler( grid, level, turn );
			break;
		default:
			break;
		}
	
		gameModeControler( grid, ps, rInfo, &turn );
	}
}
/*Control all the PVE levels by calling it according to the requested 
Level*/

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

/*It returns the X or O based if the turn is even or odd*/
Cell whoTurn( int turn )
{
	if( turn % 2 == 0 )
	{
		return CELL_X;
	}
	return CELL_O;
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

void playerSwitch( Player *playerTurn )
{
	*playerTurn = ( *playerTurn == PLAYER1 ) ? PLAYER2 : 
	PLAYER1;
}

Cell symbolSwitch( roundInfo r )
{
	return ( r.playerTurn == PLAYER1 ) ? r.player1 : r.player2;
}


