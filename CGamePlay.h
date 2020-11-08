#pragma once
#include<cstdlib>
#include<ctime>
#include<iostream>


class CGamePlay
{
public:
	const void SetSeed() { srand(time(nullptr)); };
	const bool IsAIScoreValid() { return _currentAISum < 21; }
	const bool IsPlayerScoreValid() { return _currentSum < 21; }
	void AddPlayerSum(const int firstDie, const int secondDie) { _currentSum += firstDie + secondDie; }
	void AddAISum(const int firstDie, const int secondDie) { _currentAISum += firstDie + secondDie; }

	int HandleInput();

	int DiceRoll();
	void PlayerDiceRoll();
	void AIDiceRoll();
	const bool ShouldAIRoll();

	const bool ValidAmount(const int Bet) { return Bet <= _userCredits; }
	const bool ValidRange(const int Bet);
	bool PlaceBet();

	const void InitialRoll();

	void Clear();

	void HandleFinish();
	void HandleRound();

	const bool IsUserCreditsValid();
	int MainGame(bool& isGameOver);
	const int MainLoop();

	const int MAXJACK{ 21 };
	const int GOAL{ 300 };

private:
	const int DICEMIN{ 1 };
	const int DICEMAX{ 6 };
	const int MINBET{ 1 };
	const int MAXBET{ 50 };

	int _userCredits{ 100 };
	int _currentBet{ 0 };
	int _currentSum{ 0 };
	int _currentAISum{ 0 };

	bool IsHouseStuck{ false };
	bool IsPlayerStuck{ false };
	bool IsSeedSet{ false };

};

