// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <string>
//#include <chrono>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <memory>
#include <filesystem>

// We want max and min from the limits library
#define NOMINMAX
#include <windows.h>

#include <queue>
#include <limits>
//#include <assert.h>
//#include <stdlib.h>
//#include <ctime>

//#include <list>
//#include <stack>
//#include <unordered_set>
#include <set>
#include <map>

using namespace std;

// TODO: reference additional headers your program requires here

#include "Globals.h"
#include "Utils.h"
#include "Logger.h"
#include "Loggable.h"

#include "Card.h"
#include "Play.h"
#include "Deck.h"
#include "BoardState.h"

#include "Player.h"
#include "PlayerAIMonteCarlo.h"
#include "PlayerHuman.h"
#include "PlayerRandom.h"

#include "GameModel.h"
#include "Controller.h"
#include "View.h"

#include "GameManager.h"