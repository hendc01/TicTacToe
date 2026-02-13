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
  Controls the round flow for PVP or PVE, and stores the Winner in 
  roundInfo struct 
*/
roundInfo gameModeControler( board *grid, position ps, roundInfo *py, 
							int *turn)
{
	State roundState;

    roundState = doMove( grid, ps ,turn, 
						currentPlayerCell(*py) );
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
		/*result()test the win for the current player,
		  so if the playerTurn win, that means the oposite of it
		  is the losser.*/
		py->losserPy = (py->playerTurn == PLAYER1)? PLAYER2 : PLAYER1;
	}
	return *py;
}
/*Runs PVP inputs until there's a winner or drawn*/
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

/*Run the functions for PVE mode until there is a winner or drawn*/
void gamePvEControler( board *grid, GameTypes level, 
						   roundInfo *rInfo )
{
	int turn = 0;
	*rInfo = roundInit( );

	decideSymbol( rInfo );
	position ps;
	
	printBoard( grid );
	
	/*Loop can also stop with RESULT_ERROR, appart from RESULT_WIN*/
	while( rInfo->winnerCell == RESULT_NOT_WIN )
	{
		switch ( rInfo->playerTurn ) 
		{
		case PLAYER1:
			ps = gameInput();
			break;
		case PLAYER2:
			/*Player2 always AI or Player Who hasnt log in their account
			(Guest), LevelControler calls the difficult availables in 
			PVE*/
			ps = levelControler( grid, level, *rInfo );
			break;
		default:
			break;
		}
		/*Moves the board according to the info gotten above in PLAYER1
		  Or PLAYER 2*/
		gameModeControler( grid, ps, rInfo, &turn );
	}
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
/*Switch the current player*/
void playerSwitch( Player *playerTurn )
{
	*playerTurn = ( *playerTurn == PLAYER1 ) ? PLAYER2 : 
	PLAYER1;
}
/*Returns the cell (X or O) of the current player*/
Cell currentPlayerCell( roundInfo r )
{
	return ( r.playerTurn == PLAYER1 ) ? r.player1 : r.player2;
}


