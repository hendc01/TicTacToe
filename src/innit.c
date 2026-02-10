#include "innit.h"
#include "gameTypes.h"

roundInfo roundInit( void )
{
	roundInfo r;
	r.player1 = CELL_EMPTY;
	r.player2 = CELL_EMPTY;
	r.turnCell = CELL_O;
	r.playerTurn = BLANK;
	r.winnerPy = BLANK;
	r.winnerCell = RESULT_NOT_WIN;
	r.losserPy = BLANK;
	return r;
	
}

/*Initialize all the array grid to ENUM Cell_Empyty*/
void gridInnit ( board *grid )
{
	
	for( int r = 0 ; r < 3; r++ )
	{
		for( int c = 0 ; c < 3; c++ )
		{
			grid->boardGrid[r][c] = CELL_EMPTY;
		}
	}
}


