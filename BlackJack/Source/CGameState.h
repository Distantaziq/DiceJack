#pragma once

#include "CCardGamePlay.h"

class CCardGamePlay;

class CGameState: public CCardGamePlay
{
public:

	bool IsScoreValid() const { return GetAIScore() < 21 && GetUserScore() < 21; }

	void HandleFinish();

	void PrintUpdatedScore();
	bool IsUserCreditsValid() const;

	void HandleRound();
	void MainGame(bool& IsGameOver);
	void MainLoop();

};

