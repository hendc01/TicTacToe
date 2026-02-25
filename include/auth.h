#ifndef AUTH_H
#define AUTH_H
#include "sqlite3.h"
#include "gameTypes.h"

void authRun( userInfo *user, sqlite3 *db, ScoreInfo 
					roundScore[]  );
int authInitDB( sqlite3 **db );
LoginSystem authRegister( sqlite3 *db, int role,  
						 const char *username, const char *password );
LoginSystem authLogin( sqlite3 *db, userInfo *user );
LoginSystem authController( userInfo *user, int loginOpt, 
						   sqlite3 *db );

int admCount( sqlite3 *db );
#endif
