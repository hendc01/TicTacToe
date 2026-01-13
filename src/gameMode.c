#include "gameMode.h"
#include "game.h"
State level1( board *grid, int turn )
{
	
	State state;
	
	for( int r = 0; r < 3; r++ )
	{
		for(int c = 0; c < 3; c++ )
		{
			state = gridAlloc( grid, r, c, turn );
			if( state == MOVE_OK )
			{
				return MOVE_OK;
			}
		}
	}
	return MOVE_OCCUPIED;
}
