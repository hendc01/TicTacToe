#ifndef RENDER_H
#define RENDER_H
#include "gameTypes.h"
void menu1 ( void );
void converterCell( Cell cell);
void printBoard( const board *grid );
void authOtpMsg( LoginSystem result );
void converterResult( GameResult result );
void displayMoveMsg( State moveResult );
#endif
