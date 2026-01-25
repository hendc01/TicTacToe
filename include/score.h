#ifndef SCORE_H
#define SCORE_H
#include "gameTypes.h"
#include "sqlite3.h"

ScoreDB scoreAdd( sqlite3 *db, ScoreInfo s );

#endif
