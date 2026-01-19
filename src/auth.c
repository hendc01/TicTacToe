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

int authRun( userInfo *user )
{
	sqlite3 *db = NULL;
	if(authInitDB( &db ) != 0 )
	{
		return 1;
	}
	int loginOpt = -1; 
	loginOpt = loginMenu( );
	loginInput( user );
	switch ( loginOpt )
	{
	case LOGIN:
		
		authLogin( db, user->userName, user->userPass );
		sqlite3_close( db );
		break;
	case REGISTER:
		
		authRegister( db, user->userName, user->userPass );
		sqlite3_close( db );
		break;
	default:
		sqlite3_close( db );
		break;
	}
	return 0;
}


int authRegister( sqlite3 *db, const char *username, 
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
		return 1; 
	}
	if( rc != SQLITE_DONE )
	{
		/*Unknown error*/
		return -1;
	}
	return 0;
}

int authLogin( sqlite3 *db, const char *username, const char *password )
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
		
		if( result == 0 )
		{
			return 0;
		}
		else{
			return 1;
		}
	}
	
	sqlite3_finalize(stmt);
	stmt = NULL;
	
	return 2;
}
