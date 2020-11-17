#pragma once

#include "CCardGamePlay.h"

class CCardGamePlay;

class CGameState: public CCardGamePlay
{
public:
	void HandleRound();
	void MainGame(bool& IsGameOver);
	void MainLoop();

};

