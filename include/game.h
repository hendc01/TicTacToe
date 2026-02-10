#ifndef GAME_H
#define GAME_H
#include "gameTypes.h"

roundInfo game ( board *grid, GameTypes gameChoices );
Cell isCellEmpty( const board *grid, int r, int c );
void menu1 ( void );
void gamePVPControler( board *grid, roundInfo *rInfo	);
State gridAlloc( board *grid, int row, int collum, Cell currentPlayer );
Cell whoTurn( int turn );
position gameInput();
GameResult result( const board *grid, int turn );
void gamePvEControler( board *grid, GameTypes level, 
						   roundInfo *rInfo );
State doMove(board *grid, position ps, int *turn, Cell symbol);
void decideSymbol( roundInfo *py );
void playerSwitch( Player *playerTurn );
Cell symbolSwitch( roundInfo r );
#endif
