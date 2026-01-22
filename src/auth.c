#include "auth.h"
#include "gameTypes.h"
#include "inputSafe.h"
#include "sqlite3.h"
#include "sql.h"
#include "menu.h"
#include <stdio.h>
#include <string.h>

int loginInput( userInfo *user  )
{ 
	printf( "Username:\n" );
	userInput( user->userName, sizeof( user->userName ) );
	printf( "Password\n" );
	userInput( user->userPass, sizeof( user->userPass ) );
	return 1;
}

LoginSystem authRun( userInfo *user, int loginOpt )
{
	LoginSystem result;
	sqlite3 *db = NULL;
	if(authInitDB( &db ) != 0 )
	{
		return DB_FAILED;
	}
	

	if( loginOpt == -1 )
	{
		loginOpt = loginMenu( );
	}

	loginInput( user );
	switch ( loginOpt )
	{
	case LOGIN:
		
		result = authLogin( db, user->userName, user->userPass );
		sqlite3_close( db );
		return result;
	case REGISTER:
		
	    result = authRegister( db, user->userName, user->userPass );
		sqlite3_close( db );
		return result;

	default:
		sqlite3_close( db );
		return result;
	}
	
}

/*Auth System output message*/
LoginSystem authOtp( LoginSystem result )
{
	if( result == LOGIN_OK || result == REGISTER_OK )
	{
		return LOGIN_OK;
	}
	else if( result == NAME_EXIST )
	{
		return NAME_EXIST;
	}
	return LOGIN_FAILED;

}

LoginSystem authRegister( sqlite3 *db, const char *username, 
				 const char *password) 
{
	int rc;
	sqlite3_stmt *stmt = NULL;
	const char *sql = 
		"INSERT INTO users(username, password) VALUES (?, ?);";
	rc = sqlite3_prepare_v2( db, sql, -1, &stmt, NULL);
	if ( rc != SQLITE_OK )
	{
		printf( "Prepare failed: %s\n", sqlite3_errmsg(db) );
		return -1;
	}
	
	sqlite3_bind_text(  stmt, 1, username, -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(  stmt, 2, password, -1, SQLITE_TRANSIENT);
	
	rc = sqlite3_step( stmt );
	
	sqlite3_finalize( stmt );
	stmt = NULL;
	
	if( rc == SQLITE_CONSTRAINT )
	{
		/*Name exist*/
		return NAME_EXIST; 
	}
	if( rc != SQLITE_DONE )
	{
		/*Unknown error*/
		return REGISTER_ERROR;
	}
	return REGISTER_OK;
}

LoginSystem authLogin( sqlite3 *db, const char *username, 
					  const char *password )
{
	const char *sql;
	sqlite3_stmt *stmt;
	
	sql =
		"SELECT password FROM users WHERE username = ?;";
	stmt = NULL;
	int rc = sqlite3_prepare_v2( db, sql, -1, &stmt, NULL );
	if( rc != SQLITE_OK )
	{
		printf("Prepare failed: %s\n", sqlite3_errmsg( db ));
		return -1;
	}
	
	sqlite3_bind_text(stmt, 1, username, -1, SQLITE_TRANSIENT );
	
	rc = sqlite3_step( stmt );
	
	if( rc == SQLITE_ROW )
	{
		const char *stored =
			( const char *)sqlite3_column_text( stmt, 0 );
			
		int result = strcmp( stored, password );
		
		sqlite3_finalize( stmt );
		stmt = NULL;
		/*EQUAL*/
		if( result == 0 )
		{
			return LOGIN_OK;
		}
		else{
			return LOGIN_FAILED;
		}
	}
	
	sqlite3_finalize(stmt);
	stmt = NULL;
	
	return LOGIN_ERROR;
}
