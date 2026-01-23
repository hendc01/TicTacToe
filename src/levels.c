#include "levels.h"
#include "gameTypes.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include "win.h"
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
	/*If this functions is called after a win or draw(0 cell empties). 
	It results in a LEVEL_ERROR*/
	if( count == 0 )
	{
		ps.error = LEVEL_ERROR;
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
	ps->error = LEVEL_ERROR;
	return 0;
}
/*Return a random number within a range*/
int randomIndex( int max )
{
	return rand() % max;
}
