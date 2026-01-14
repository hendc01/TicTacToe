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

int randomIndex( int max )
{
	return rand() % max;
}

