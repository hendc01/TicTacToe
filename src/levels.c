#include "levels.h"
#include "gameTypes.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include "render.h"
#include "win.h"
/*Controls the PVE level mode*/
position levelControler( board *grid ,GameTypes level,
						roundInfo round )
{
	position ps;
	printf("LEVEL CALLED: %d\n", level);
	switch ( level ) 
	{
	case LEVEL1:
		ps = level1( grid );
		if(ps.error == LV1_NO_CELL) displayLevelMsg(ps.error);
		break;
	case LEVEL2:
		ps = level2( grid, round.turnCell );
		break;
	case LEVEL3:
		ps = level4( grid, round, 3);
		break;
	case LEVEL4:
		ps = level4( grid, round, 9 );
		break;
	default:
		ps.row = -1;
		ps.collum = -1;
		ps.error = LEVEL_ERROR;
		break;
	}
	return ps;
}

/*Returns position(row and column) for AI level1 (pseudo-radom moves)*/
position level1( const board *grid )
{	
	int count = 0;
	int cell = -1;
	position ps;
	position empty[9];

	for( int r = 0; r < 3; r++  )
	{
		for ( int c = 0; c < 3; c++)
		{
			/*Checks ever cell and stores the emptys in a array of 
			empty cell possibilities*/
			if( grid->boardGrid[r][c] == CELL_EMPTY )
			{
				empty[count].row = r;
				empty[count].collum = c;
				count++;
			}
		}
	}	
	/*If this function is called after a win or draw(0 cell empties). 
	  It will tringger the msg error MOVE OUT OF RANGE, so always nece
	  ssary to call RESULT() after every movent on the board. If is not
	  know if the first movent is also True, call result before and 
	  after*/
	if( count == 0 )
	{
		ps.error = LV1_NO_CELL;
		ps.row = -1;
		ps.collum = -1;
		return ps;
	}
	/*Assign a random number to cell within the range(count) and chose
	a random array index*/
	cell = randomIndex( count );
	ps = empty[cell];
	ps.error = LEVEL_OK;
	return ps;
}
/*It returns a random position if there is no possible win available 
  or the win position if one exist*/
position level2( const board *grid, Cell currentPlayer )
{
	position ps;
	Cell opp;
	/*Checks a win for currentPlayer*/
	if ( isThereWin( grid, &ps, currentPlayer ))
	{
		return ps;
	}
	/*Checks a win for the adversary*/
	opp = ( currentPlayer == CELL_X ) ? CELL_O : CELL_X;
	if ( isThereWin( grid, &ps, opp ))
	{
		return ps;
	}
	return level1( grid );
}

position level4( const board *grid, roundInfo rf, int depth ){
	int valuation;
	position bestPosition;
	position secondBest;
	int drawn = 0;
	int foundMove = 0;
	
	for( int r = 0; r < 3; r++ ){
		for( int c = 0; c < 3; c++ ){
			
			if( isCellEmpty( grid, r, c ) == CELL_EMPTY ){
				foundMove = 1;
				board tempGrid = *grid;
				roundInfo tempInfo = rf;
				Cell aiSymbol = currentPlayerCell(rf);
				gridAlloc( &tempGrid, r, c, aiSymbol );
				playerSwitch( &tempInfo.playerTurn );
				valuation = minimax( tempGrid, aiSymbol,
					tempInfo, rf.turn + 1, depth );
					
				if( valuation == 1 ){
					bestPosition.row = r;
					bestPosition.collum = c;
					return bestPosition;
				}
				if( !drawn && valuation == 0 ){
					secondBest.row = r;
					secondBest.collum = c;
					drawn = 1;
				}	
			}	
		}
	}
	if( drawn ){
		return secondBest;	
	}
	else if( foundMove ){
		return level1( grid );	
	}
	else{
		bestPosition.row = -1;
		bestPosition.collum = -1;
		bestPosition.error = LV1_NO_CELL;
		return bestPosition;	
	}
	
}
/*Scan the board for a win for a specif player*/
int isThereWin( const board *grid, position *ps, Cell currentPlayer )
{
	board temp;
	/*Checks ever cell for a win by trying a move for each cell*/
	for( int r = 0; r < 3; r++ )
	{
		for( int c = 0; c < 3; c++ )
		{
			/*Refresh the board for each attempt*/
			temp = *grid;
			if( isCellEmpty( grid, r, c ) != CELL_EMPTY)
			{
				continue;
			} 
			gridAlloc( &temp, r, c, currentPlayer );

			if( winChecker( &temp ) == currentPlayer )
			{
				ps->row = r;
				ps->collum = c;
				ps->error = LEVEL_OK;
				return 1;
			}
		}
	}
	ps->row = -1;
	ps->collum = -1;
	ps->error = ISTHERE_WIN_ERROR;
	return 0;
}
/*Return a random number within a range*/
int randomIndex( int max )
{
	return rand() % max;
}
// !!!SYMBOL must me the AI symboy
int minimax( board grid, Cell symbol, roundInfo rf, int turn, int depth ){
	roundInfo depthInfo;
	Cell isEmpty;
	GameResult gameState;
	Cell aiSymbol = symbol;
	int evaluation;
	int bestScore;
	int foundMove = 0;
	//MAX PLAYER
	if( currentPlayerCell( rf ) == aiSymbol ){
		bestScore = -2;
	}
	else{
		bestScore = 2;
	}
	
	if (depth == 0) {
		return 10; // later we improve this
	}
	
	gameState = result( &grid, turn );
	if( gameState == RESULT_DRAW) {
		return 0;
	}
	if( (gameState == RESULT_X_WINS &&  aiSymbol == CELL_X)  ||
	     (gameState == RESULT_O_WINS && aiSymbol == CELL_O)){
		return 1;
	}
	if( (gameState == RESULT_X_WINS && aiSymbol == CELL_O) ||
	   (gameState == RESULT_O_WINS && aiSymbol == CELL_X) ){
		return -1;
	}
	for( int r = 0 ; r < 3 ; r++ ){
		for( int c = 0 ; c < 3 ; c++ ){
			isEmpty = isCellEmpty( &grid, r, c );
			if( isEmpty == CELL_EMPTY ){ 
				
				foundMove = 1;
				board tempGrid = grid;
				symbol = currentPlayerCell( rf );
				gridAlloc( &tempGrid, r, c, symbol );
				depthInfo = rf;
				playerSwitch( &depthInfo.playerTurn );
				evaluation = minimax( tempGrid, aiSymbol, 
					depthInfo, turn + 1, depth -1 );
				if( currentPlayerCell(rf) == 
				   aiSymbol ){
					if( evaluation > bestScore ){
						bestScore = evaluation;
					}	
				}
				else{
					if( evaluation < bestScore ){
						bestScore = evaluation;
					}	
				}
				
			
			}
		}
	}
	if( !foundMove ){
		return 0;
	}
	return bestScore;
}
void minimaxSwitch( Minimax *isMax )
{
	*isMax = ( *isMax == MIN_PLAYER ) ? MAX_PLAYER : 
	MIN_PLAYER;
}

