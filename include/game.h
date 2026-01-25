#ifndef GAME_H
#define GAME_H
#include "gameTypes.h"

roundInfo game ( board *grid, GameTypes gameChoices );
void initializer ( board *grid );
Cell isCellEmpty( const board *grid, int r, int c );
void menu1 ( void );
roundInfo gamePVPControler( board *grid	);
State gridAlloc( board *grid, int row, int collum, Cell currentPlayer );
Cell whoTurn( int turn );
position gameInput();
GameResult result( const board *grid, int turn );
roundInfo gamePvEControler( board *grid, GameTypes level );
GameResult humanTurn( board *grid, int *turn, Cell symbol );
State doMove(board *grid, position ps, int *turn, Cell symbol);
position levelControler( board *grid ,GameTypes level,
						int turn );
roundInfo roundInit( void );
void decideSymbol( roundInfo *py );
void playerSwitch( Player *player );
#endif
