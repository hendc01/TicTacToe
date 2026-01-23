#include "auth.h"
#include "gameTypes.h"
#include "inputSafe.h"
#include "sqlite3.h"
#include "sql.h"
#include "menu.h"
#include <stdio.h>
#include <string.h>

/*Login system controler*/
LoginSystem authRun( userInfo *user, int loginOpt, sqlite3 *db )
{
	LoginSystem result;

	if( loginOpt == -1 )
	{
		loginOpt = loginMenu( );
	}

	loginInput( user );
	switch ( loginOpt )
	{
	case LOGIN:
		
		result = authLogin( db, user->userName, user->userPass, 
						   &user->userRole );
		return result;
		
	case REGISTER:
		
	    result = authRegister( db, 1, user->userName, user->userPass );
		return result;

	default:
		result = LOGIN_ERROR;
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
/*Counts how many ADM there is in the DB users ( adm == 0)*/
int admCount( sqlite3 *db )
{
	int rc;
	sqlite3_stmt *stmt = NULL;
	int count = -1;
	const char *sql =
		"SELECT COUNT(role) FROM users WHERE role = 0;";
	rc = sqlite3_prepare_v2( db, sql, -1, &stmt, NULL );
	if( rc != SQLITE_OK )
	{
		printf( "Prepare failed: %s", sqlite3_errmsg( db ) );
		return -1;
	}
	if( sqlite3_step( stmt ) == SQLITE_ROW )
	{
		count = sqlite3_column_int( stmt, 0 );
	}
	sqlite3_finalize( stmt );
	return count;
}
/*Insert the User credential to the data-base*/
LoginSystem authRegister( sqlite3 *db, const int role,  
				const char *username, const char *password ) 
{
	int rc;
	sqlite3_stmt *stmt = NULL;
	const char *sql = 
		"INSERT INTO users(role, username, password) VALUES (?,?,?);";
	rc = sqlite3_prepare_v2( db, sql, -1, &stmt, NULL);
	if ( rc != SQLITE_OK )
	{
		printf( "Prepare failed: %s\n", sqlite3_errmsg(db) );
		return REGISTER_ERROR;
	}
					 
	sqlite3_bind_int(  stmt, 1, role);
	sqlite3_bind_text(  stmt, 2, username, -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(  stmt, 3, password, -1, SQLITE_TRANSIENT);
	
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
/*Select password from DB from Username and compare to the user 
password input*/
LoginSystem authLogin( sqlite3 *db, const char *username, 
					  const char *password, Role *role )
{
	const char *sql;
	sqlite3_stmt *stmt;
	
	sql =
		"SELECT role, password FROM users WHERE username = ?;";
	stmt = NULL;
	int rc = sqlite3_prepare_v2( db, sql, -1, &stmt, NULL );
	if( rc != SQLITE_OK )
	{
		printf("Prepare failed: %s\n", sqlite3_errmsg( db ));
		return LOGIN_FAILED;
	}
	
	sqlite3_bind_text(stmt, 1, username, -1, SQLITE_TRANSIENT );
	
	rc = sqlite3_step( stmt );
	
	if( rc == SQLITE_ROW )
	{
		const char roleTemp = (Role)sqlite3_column_int( stmt, 0 );
		const char *stored =
			( const char *)sqlite3_column_text( stmt, 1 );
		/*sql owns the stored memory adress so its necessary to compare
		before .._finalize*/
		int result = strcmp( stored, password );
		
		sqlite3_finalize( stmt );
		stmt = NULL;
		/*EQUAL*/
		if( result == 0 )
		{
			*role = roleTemp;
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

/*Delete user*/
LoginSystem deleteUser( sqlite3 *db  )
{
	const char *sql;
	sqlite3_stmt *stmt = NULL;
	
	sql = "DELETE FROM users WHERE username = ?;";
	int	rc = sqlite3_prepare_v2( db, sql, -1, &stmt, NULL );
	if( rc != SQLITE_OK )
	{
		printf("Prepare failed: %s\n", sqlite3_errmsg( db ));
		return LOGIN_FAILED;
	}
}
