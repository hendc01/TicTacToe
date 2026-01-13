#ifndef GAME_H
#define GAME_H
#include "gameTypes.h"

void game ( void );
void initializer ( board *grid );
Cell isCellEmpty( const board *grid, int r, int c );
void menu1 ( void );
GameResult gamePVPControler( board *grid	);
State gridAlloc( board *grid, int row, int collum, Cell currentPlayer );
void displayMoveMsg( State moveResult );
Cell winChecker( const board *grid );
Cell diagonalChecker( const board *grid );
Cell horizontalChecker( const board *grid );
Cell verticalChecker( const board *grid );
Cell whoTurn( int turn );
position gameInput();
GameResult result( const board *grid );
#endif
