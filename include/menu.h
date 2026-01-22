#ifndef MENU_H
#define MENU_H 
#include "gameTypes.h"
AdmTypes admMenu( void );
GameTypes mainMenu( void );
GameTypes pveMenu( void );
LoginOpt loginMenu( void );
AdmTypes admMode( void );
int runLogin( userInfo *user );
void admModeController( AdmTypes admType );
GameTypes admMenuController();
GameTypes menuControler( userInfo user );
#endif
