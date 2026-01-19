#include "levels.h"
#include "gameTypes.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
			if( grid->boardGrid[r][c] == CELL_EMPTY )
			{
				empty[count].row = r;
				empty[count].collum = c;
				count++;
			}
		}
	}	
	if( count == 0 )
	{
		ps.error = LEVEL1_ERROR;
		ps.row = -1;
		ps.collum = -1;
		return ps;
	}

	cell = randomIndex( count );
	ps = empty[cell];
	ps.error = LEVEL_OK;
	
	return ps;
}

position level2( const board *grid, Cell currentPlayer )
{
	position ps;
	Cell opp;
	
	if ( isThereWin( grid, &ps, currentPlayer ))
	{
		return ps;
	}
	opp = ( currentPlayer == CELL_X ) ? CELL_O : CELL_X;
	if ( isThereWin( grid, &ps, opp ))
	{
		return ps;
	}
	return level1( grid );
}

int isThereWin( const board *grid, position *ps, Cell currentPlayer )
{
	board temp;
	GameResult win;
	for( int r = 0; r < 3; r++ )
	{
		for( int c = 0; c < 3; c++ )
		{
			temp = *grid;
			if( isCellEmpty( grid, r, c ) != CELL_EMPTY)
			{
				continue;
			} 
			gridAlloc( &temp, r, c, currentPlayer );
			win = result( &temp, 0);
			if ((currentPlayer == CELL_X && win == RESULT_X_WINS) ||
                (currentPlayer == CELL_O && win == RESULT_O_WINS))
			{
				ps->row = r;
				ps->collum = c;
				ps->error = LEVEL_OK;
				return 1;
			}
		}
	}
	return 0;
}
int randomIndex( int max )
{
	return rand() % max;
}
