#ifndef INPUTSAFE_H
#define INPUTSAFE_H
#include "gameTypes.h"
int loginInput( userInfo *user, int chose );
int intInput ( int min, int max );
void cleanBuffer (  );
void userInput( char *userInfo, int size );
SafeString stringInput( char *buff, int size  );
void decideSymbol( roundInfo *py );
void clearScreen();
void pressEnter();
#endif
