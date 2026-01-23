#include "win.h"
#include "gameTypes.h"
/*Returns who won (X, O, DRAW and NOT WIN*/
GameResult result( const board *grid, int turn )
{
	Cell winner = CELL_EMPTY;
	winner = winChecker( grid );
	
	switch ( winner )
	{
	case CELL_X:
		return RESULT_X_WINS;
	case CELL_O:
		return RESULT_O_WINS;
	case CELL_EMPTY:
		if( turn == 9 )
		{
			return RESULT_DRAW;
			break;
		}
		return RESULT_NOT_WIN;
		break;
	}
	return RESULT_ERROR;
	
}
/*Call the checker functions and return whether someone won or 
Cell empty*/
Cell winChecker( const board *grid )
{
	/*Return cell_x, cell_o, cell_empty for drawn*/
	Cell dResult = diagonalChecker( grid );
	Cell vResult = verticalChecker( grid );
	Cell hResult = horizontalChecker( grid );
	if( dResult != CELL_EMPTY )
	{
		return dResult;
	}
	else if( vResult != CELL_EMPTY )
	{
		return vResult;
	}
	else if( hResult != CELL_EMPTY )
	{
		return hResult;
	}
	return CELL_EMPTY;
}

/*Checks for horizontal wins*/
Cell horizontalChecker( const board *grid )
{
	for( int r = 0 ; r < 3 ; r++  )
	{
		Cell a = grid->boardGrid[r][0];
		/*Check the first first collum and 3 rows*/
		int standTest = ( a != CELL_EMPTY );
		/*If its false it wont return a*/
		for( int c = 1; c < 3 && standTest; c++ )
		{
			if (grid->boardGrid[r][c] != a)
			{
				standTest = 0;
			}
		}
		/*If standTest comes here been true it means it passed the test
		and i can return it without checking the other's(only one row
		can be true at time)*/
		if( standTest )
		{
			return a;
		}
	}
	return CELL_EMPTY;
}
/*Checks for vertical wins*/
Cell verticalChecker( const board *grid )
{
	for( int c = 0 ; c < 3 ; c++  )
	{
		Cell a = grid->boardGrid[0][c];
		/*Check the first first collum and 3 rows*/
		int standTest = ( a != CELL_EMPTY );
		/*If its false it wont return a*/
		for( int r = 1; r < 3 && standTest; r++ )
		{
			if (grid->boardGrid[r][c] != a)
			{
				standTest = 0;
			}
		}
		/*If standTest comes here being true,it means it passed the test
		and i can return it without checking the other's(only one row
		can be true at time)*/
		if( standTest )
		{
			return a;
		}
	}
	return CELL_EMPTY;
}
/*Checks for diagonal wins */
Cell diagonalChecker(const board *grid)
{
	/*Checks the first cell of the two possible diagonals
	  IT returns X, O or CELL_EMPTY*/
	Cell a = grid->boardGrid[0][0]; 
	Cell b = grid->boardGrid[0][2];
	/*If a || b different from empty there is a chance of win, so it is
	assigned "TRUE (1)"
	*/
	int standTest = (a != CELL_EMPTY);
	int inverseTest = (b != CELL_EMPTY);
	
	/*It checks if the rest of the cells are equal to the first cell
	assigned to a and b*/
	for (int j = 0, k = 2; j < 3; j++, k--)
	{
		/* Main diagonal: (0,0), (1,1), (2,2)*/
		if (grid->boardGrid[j][j] != a)
			standTest = 0;
		
		/*Second diagonal: (0,2), (1,1), (2,0)*/
		if (grid->boardGrid[j][k] != b)
			inverseTest = 0;
	}
	/*Can only be true if the first cell checked is different from EMPTY
	and all the other two equal to the first*/
	if (standTest)
		return a;
	
	if (inverseTest)
		return b;
	
	return CELL_EMPTY;
}

