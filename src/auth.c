#include "auth.h"
#include "gameTypes.h"
#include "inputSafe.h"
#include "sqlite3.h"
#include "sql.h"
#include "menu.h"
#include <stdio.h>
#include <string.h>
#include <render.h>
#include <sodium.h>


void authRun( userInfo *user, sqlite3 *db, ScoreInfo 
					roundScore[]  )
{
	LoginSystem  authResult = LOGIN_FAILED; 
	while( authResult != LOGIN_OK )
	{
		authResult = authController( user, LOGIN_MENU, db );
		authOtpMsg( authResult );
		
		/*NAME_EXIST ONLY HAPPENS DURING REGISTER*/
		if( authResult == NAME_EXIST )
		{
			/*user will stores the player1 identifier for DB 
			select/insert*/
			authResult = authController( user, REGISTER, db );
			/*The program uses roundScore to locate/create 
			a user log in the 
			DB for score table.*/
			roundScore[PLAYER1].id = user->id; 
			authOtpMsg( authResult );
		}
	}
}
/*Login system controler*/
LoginSystem authController( userInfo *user, int loginOpt, 
						   sqlite3 *db )
{
	LoginSystem result;
	/*Main Login Menu*/
	if( loginOpt == LOGIN_MENU )
	{
		loginOpt = loginMenu( );
	}
	
	
	/*Sub Menus from main login menu*/
	switch ( loginOpt )
	{
	case LOGIN:
		loginInput( user, LOGIN );
		result = authLogin( db, user );
		return result;
	case REGISTER:
		loginInput( user, REGISTER );
		/*If DB empty the first user account created is given 
		the role ADM*/
		if( admCount( db )  < 1 )
		{
			user->userRole = 0;
		}
		else{
			user->userRole = 1;
		}
	    result = authRegister( db, user->userRole, user->userName, 
							  user->userPass );
		return result;

	default:
		result = LOGIN_ERROR;
		return result;
	}	
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
	//Must be unique DB itself regects
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
LoginSystem authLogin( sqlite3 *db, userInfo *user )
{
	const char *sql;
	sqlite3_stmt *stmt;
	
	sql =
		"SELECT id, role, password FROM users WHERE username = ?;";
	stmt = NULL;
	int rc = sqlite3_prepare_v2( db, sql, -1, &stmt, NULL );
	if( rc != SQLITE_OK )
	{
		printf( "Prepare failed: %s\n", sqlite3_errmsg( db ) );
		return LOGIN_FAILED;
	}
	
	sqlite3_bind_text(stmt, 1, user->userName, -1, SQLITE_TRANSIENT );
	
	rc = sqlite3_step( stmt );
	
	if( rc == SQLITE_ROW )
	{
		const int idTemp = sqlite3_column_int( stmt, 0 );
		const int roleTemp = (Role)sqlite3_column_int( stmt, 1 );
		const char *stored =
			( const char *)sqlite3_column_text( stmt, 2 );
		/*sql owns the stored memory adress so its necessary to compare
		before .._finalize*/
		
		int result = crypto_pwhash_str_verify(stored, 
						      user->userPass, 
						      strlen(user->userPass));
		
		sqlite3_finalize( stmt );
		stmt = NULL;
		/*EQUAL*/
		if( result == 0 )
		{
			user->id = idTemp;
			user->userRole = roleTemp;
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


/*Encrypting Password*/

