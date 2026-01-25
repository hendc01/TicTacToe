#ifndef GAME_H
#define GAME_H
#include "gameTypes.h"

GameResult game ( board *grid, GameTypes gameChoices );
void initializer ( board *grid );
Cell isCellEmpty( const board *grid, int r, int c );
void menu1 ( void );
GameResult gamePVPControler( board *grid	);
State gridAlloc( board *grid, int row, int collum, Cell currentPlayer );
Cell whoTurn( int turn );
position gameInput();
GameResult result( const board *grid, int turn );
GameResult gamePvEControler( board *grid, GameTypes level );
GameResult humanTurn( board *grid, int *turn, Cell symbol );
State doMove(board *grid, position ps, int *turn, Cell symbol);
position levelControler( board *grid ,GameTypes level,
						int turn );
playerSymbol decideSymbol( void );
void playerSwitch( Player *player );
#endif
