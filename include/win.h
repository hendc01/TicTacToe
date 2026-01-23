#ifndef WIN_H
#define WIN_H
#include "gameTypes.h"
Cell winChecker( const board *grid );
Cell diagonalChecker( const board *grid );
Cell horizontalChecker( const board *grid );
Cell verticalChecker( const board *grid );
#endif
