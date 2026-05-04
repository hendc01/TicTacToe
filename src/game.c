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
#include "lanSocket.h"
/*Main controler*/

/*-Run one session of the game 
  - Calls the PVP and PVE functions controler.
  - Calls PVE sub-menu, sub-menu returns gameMode (level 1, Leve2..3 
    for the PVE controler.
  -PVP and PVE will rely on gameModeControle to move the board and 
   switch current player.
*/
roundInfo game( board *grid, GameTypes gameMode, ScoreInfo *dbInfo)
{
	/*Returns Game Option*/	
	roundInfo py;
	GameTypes level;
	py = roundInit( );
	gridInnit( grid );
	
	switch ( gameMode ) 
	{
	case PLAYER_VS_PLAYER:
		PvPModes pvpMenu = pvpSubMenu();
		if( pvpMenu == LAN){
			lanPvPControler();
			break;
		}
		// PVP LOCAL
		printBoard( grid );
		gamePVPControler( grid, &py );
		return py;
	
	case PLAYER_VS_MACHINE:
		level = pveMenu();
		if( level == DISPLAY ){
			displayScore( dbInfo );
		}
		else{
			gamePvEControler( grid, level, &py);	
		}
		
		py.level = level;
		return py;
	case MENU_ERROR:
		printf( "Game Mode menu read error. \n" );
		break;
	default:
		py.winnerCell = RESULT_ERROR;
		return py;
	}
}
/*
  Controls the round flow for PVP or PVE, and stores the Winner in 
  roundInfo struct 
  -Call doMove to move the board with the current player symbol
  -Switch the player
  -Call resul to check for a wil
*/
roundInfo gameModeControler( board *grid, position ps, roundInfo *py, 
							int *turn)
{
	State roundState;
	//doMove take a positon 
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
		/*result() test the win for the current player,
		  so if the playerTurn(currentPlayer) wins, the player opposite
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
void gamePvEControler( board *grid, GameTypes gameMode, 
						   roundInfo *rInfo )
{
	*rInfo = roundInit( );

	decideSymbol( rInfo );
	position ps;
	
	printBoard( grid );
	/*Loop can also stop with RESULT_ERROR, appart from RESULT_WIN*/
	while( rInfo->winnerCell == RESULT_NOT_WIN )
	{
		rInfo->winnerCell = result(grid, rInfo->turn);
		if (rInfo->winnerCell != RESULT_NOT_WIN) {
			break;
		}
		switch ( rInfo->playerTurn ) 
		{
		case PLAYER1:
			ps = gameInput();
			break;
		case PLAYER2:
			/*Player2 always the AI or Player Who hasnt log in their 
			account (Guest), LevelControler calls the difficult 
			availables in PVE*/
			ps = levelControler( grid, gameMode, *rInfo );
			break;
		default:
			break;
		}
		/*Moves the board according to the info gotten above, in PLAYER1
		  Or PLAYER 2*/
		gameModeControler( grid, ps, rInfo, &rInfo->turn );
	}
}

/*This move the board to next player by calling GridAlloc and iterating
  turn to pass the turn to the next player (x or o)
  Also, increment *turn. Which will be used to decide drawn in result()
  when == 9 */
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

// Check board to see if there is still cell empties
int isDrawn( board grid ){
	for(int r = 0; r < 3; r++){
		for(int c = 0; c < 3; c++){
			if(isCellEmpty(&grid, r, c) == CELL_EMPTY){
				return 0;
			}
		}
	}
	return 1; 

}
