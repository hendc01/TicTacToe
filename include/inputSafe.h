#ifndef INPUTSAFE_H
#define INPUTSAFE_H
#include "gameTypes.h"
int loginInput( userInfo *user );
int intInput ( int min, int max );
void cleanBuffer ( void );
void userInput( char *userInfo, int size );
SafeString stringInput( char *buff, int size  );
#endif
