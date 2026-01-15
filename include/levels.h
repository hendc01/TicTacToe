#ifndef LEVELS_H
#define LEVELS_H
#include "gameTypes.h"
position level1( const board *grid );
int randomIndex( int max );
int isThereWin( const board *grid, position *ps, Cell currentPlayer );
position level2( const board *grid, Cell currentPlayer );

#endif
