#ifndef SCORE_H
#define SCORE_H
#include "gameTypes.h"
#include "sqlite3.h"

ScoreDB scoreUpsert( sqlite3 *db, const ScoreInfo s, roundInfo py );
ScoreDB scoreControler( sqlite3 *db, roundInfo py, 
					   ScoreInfo *roundScore );
void updateRoundInfo( roundInfo py, ScoreInfo *roundScore );
void player1ScoreUpdate( const ScoreInfo *runScore, 
			const ScoreInfo *dbScore,  ScoreInfo *roundScore );
ScoreDB scoreSelect( sqlite3 *db, ScoreInfo *dbScore, roundInfo py );
void retrieveScoreInfo( sqlite3 *db, ScoreInfo *dbInfo );
#endif
