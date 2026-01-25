#ifndef AUTH_H
#define AUTH_H
#include "sqlite3.h"
#include "gameTypes.h"

int authInitDB( sqlite3 **db );
LoginSystem authRegister( sqlite3 *db, int role,  
						 const char *username, const char *password );
LoginSystem authLogin( sqlite3 *db, userInfo *user );
LoginSystem authRun( userInfo *user, int loginOpt, sqlite3 *db );
LoginSystem authOtp( LoginSystem result );
int admCount( sqlite3 *db );
#endif
